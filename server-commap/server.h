#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <iostream>

class Server {
	int server_socket = 0;
	struct sockaddr_in server_address;
	int client_count = 0;
	std::vector<int> user_id;
	std::vector<int> client_sockets;
	std::mutex client_mutex;

public:
	void StartServer();
	void CloseServer();
	void HandleConnectionReq();
	void HandleClientConnection(int socket);
	void ConnectClientToUser(int socket, int id);

	void Announcement(std::string msg) {
		std::cout << "[+] " << msg << "." << "\n";
	}

	void Warning(std::string msg) {
		std::cout << "[!] " << msg << "." << "\n";
	}
	
	void Error(std::string msg) {
		std::cout << "[!] " << msg << "." << "\n";
		exit(1);
	}
};