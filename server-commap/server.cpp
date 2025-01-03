#include "server.h"

void Server::StartServer() {
	Announcement("Server is starting");
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(4534);
	server_address.sin_addr.s_addr = INADDR_ANY;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) Error("Failed to initialize socket");

	int opt = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		Error("Failed to set the socket reusable");

	if (bind(server_socket, (sockaddr*) &server_address, sizeof(server_address)) != 0)
		Error("Failed to bind the server socket");

	if (listen(server_socket, 1) < 0)
		Error("Server socket failed to listen for connection");

	Announcement("Server has started");

	std::thread con_handler_obj([this](){this->HandleConnectionReq();});
	con_handler_obj.join();
}

void Server::HandleConnectionReq() {
	while (true) {
		int client_socket = accept(server_socket, nullptr, nullptr);
		if (client_socket < 0) Error("Failed to accept the incoming connection");
		Announcement("A client is connected");

		{
			client_mutex.lock();
			client_sockets.push_back(client_socket);
			client_count++;
			client_mutex.unlock();
		}

		for (int d : client_sockets) std::cout << "socket: " << d << '\n';

		std::thread client_con_handler(&Server::HandleClientConnection, this, client_socket);
		client_con_handler.detach();
	}
}

void Server::HandleClientConnection(int socket) {
	while (true) {
		char buffer[1024];
		int received = recv(socket, buffer, sizeof(buffer)-1, 0);
		
		if (received > 0) {
			buffer[received] = '\0';

			bool isID = std::any_of(std::begin(buffer), std::end(buffer), [](unsigned char c) {return std::isdigit(c);});
			if (std::string(buffer).size() == 5 && isID) {
				user_id.push_back(std::stoi(buffer));
				Announcement("Client id added");
				std::cout << "id: " << buffer << '\n';
			} else {
				std::string msg; int id;
				std::string token;
				char del = '\n';

				 size_t pos = std::string(buffer).find(del);

			    if (pos != std::string::npos) {
			        id = std::stoi(std::string(buffer, pos));        
      				msg = std::string(buffer).substr(pos + 1);
      			}

		        std::cout << "id of recepient: " << id << '\n';
		        std::cout << "message for the recepient: " << msg << '\n';

		        int socket = 0;
		       	for (int s = 0; s < client_sockets.size(); s++) {
		       		if (user_id[s] == id) {
		       			socket = client_sockets[s];
		       			break;
		       		}
		       	}

		       	std::cout << "socket to be redirected the msg at: " << socket << '\n';
		        if (send(socket, msg.c_str(), msg.size(), 0) > 0) std::cout << "Message redirected." << '\n';
			}
		}
	}
}