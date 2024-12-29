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
#include <iostream>

class Client {
	int client_socket = 0;
	struct sockaddr_in server_info;
	bool is_connected = false;

public:
	bool ConnectToServer();
	void SetUpSocket();
	void HandleConnection();

	void Announcement(std::string msg) {
		std::cout << "[+] " << msg << "." << "\n";
	}

	void Warning(std::string msg) {
		std::cout << "[!] " << msg << "." << "\n";
	}
	
	void Error(std::string msg) {
		std::cout << "[!!] " << msg << "." << "\n";
		exit(1);
	}
};