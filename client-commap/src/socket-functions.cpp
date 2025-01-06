#include "../headers/window.h"

void Window::SetUpSocket() {
	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(4534);
	server_info.sin_addr.s_addr = inet_addr("192.168.1.15");

	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket < 0) Error("Failed to create socket");

	int opt = 1;
	if (setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0)
		Error("Failed to reuse the socket");

	std::cout << "Socket is up" << '\n';
	std::thread con([this](){this->ConnectToServer();});
	con.detach();
}

void Window::ConnectToServer() {
	while (!is_connected) {
		int res = connect(client_socket, (struct sockaddr*) &server_info, sizeof(server_info)); 

		if (res == 0) {
			is_connected = true;
			std::string id = std::to_string(my_id);
			send(client_socket, id.c_str(), id.size(), 0);
			break;
		}

		std::cout << "[!] Failed to connect. Trying again" << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait before retrying
	}

	std::cout << "[+] Connected to the server." << '\n';
	HandleConnection();
}

void Window::HandleConnection() {
	do {
		char buffer[1024];
		int received = recv(client_socket, buffer, sizeof(buffer)-1, 0);

		if (received > 0) {
			buffer[received] = '\0';
			std::cout << "You received this: " << buffer << '\n';
			wxLogMessage(buffer);

			std::string msg; int id;
			char del = '\n';

			size_t pos = std::string(buffer).find(del);

		    if (pos != std::string::npos) {
		        id = std::stoi(std::string(buffer, pos));        
  				msg = std::string(buffer).substr(pos + 1);
  			}

  			SaveConversation(msg, id, my_id);
		}
	} while (is_connected);
}

bool Window::SendMessage(wxString msg, int id) {
	std::string message = std::to_string(id) + "\n" + msg.ToStdString();
	std::cout << "message: \n" << message << '\n';

	int res = send(client_socket, message.c_str(), message.size(), 0);
	if (res < 0) {
		Error("Failed to send message");
		return false;
	} else {
		std::cout << "Message sent." << '\n';
		SaveConversation(msg, my_id, id);
	} return true;
}