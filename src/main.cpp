#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

const std::vector<std::string> supported_capabilities = 
		std::vector<std::string>(1, "account-notify");

void handle_client(int client_fd, const std::string& password) {
		char buffer[1024];
		std::string client_message;
		
		// Build the list of supported capabilities
		std::string capabilities;
		for (std::vector<std::string>::const_iterator it = supported_capabilities.begin(); it != supported_capabilities.end(); ++it) {
				if (!capabilities.empty()) {
						capabilities += " ";
				}
				capabilities += *it;
		}
		
		std::string cap_ls_response = ":server.capabilities CAP * LS :" + capabilities + "\r\n";
		send(client_fd, cap_ls_response.c_str(), cap_ls_response.length(), 0);

		// Read messages from the client
		while (true) {
				memset(buffer, 0, sizeof(buffer));
				ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

				if (bytes_received <= 0) {
						std::cerr << "Client disconnected or error occurred" << std::endl;
						close(client_fd);
						break;
				}

				client_message = std::string(buffer);

				if (client_message.find("CAP LS") != std::string::npos) {
						// Handle CAP LS command
						std::string cap_response = ":server.capabilities CAP * LS :" + capabilities + "\r\n";
						send(client_fd, cap_response.c_str(), cap_response.length(), 0);
				} else if (client_message.find("CAP REQ") != std::string::npos) {
						// Handle CAP REQ command
						std::string cap_req_response = ":server.capabilities CAP * ACK :" + client_message.substr(8) + "\r\n";
						send(client_fd, cap_req_response.c_str(), cap_req_response.length(), 0);
				} else if (client_message.find("PASS") != std::string::npos) { //NO FUNCIONA
						// Handle password command
						if (client_message.substr(5) == password) {
								std::string pass_response = ":server 001 :Password accepted\r\n";
								send(client_fd, pass_response.c_str(), pass_response.length(), 0);
						} else {
								std::string pass_fail_response = ":server 464 :Password incorrect\r\n";
								send(client_fd, pass_fail_response.c_str(), pass_fail_response.length(), 0);
								close(client_fd);
								break;
						}
				} else {
					std::cout << client_message << std::endl;
						std::string msg_response = ":server 001 : Unknown holas\r\n";
						send(client_fd, msg_response.c_str(), msg_response.length(), 0);
				}
		}
}

int main(int argc, char *argv[]) {
		if (argc != 3) {
				std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
				return 1;
		}

		int port;
		std::stringstream(argv[1]) >> port;
		std::string password = argv[2];

		int server_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (server_fd < 0) {
				std::cerr << "Failed to create socket" << std::endl;
				return 1;
		}

		sockaddr_in server_addr;
		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = INADDR_ANY;
		server_addr.sin_port = htons(port);

		if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
				std::cerr << "Bind failed" << std::endl;
				close(server_fd);
				return 1;
		}

		if (listen(server_fd, 3) < 0) {
				std::cerr << "Listen failed" << std::endl;
				close(server_fd);
				return 1;
		}

		std::cout << "Server listening on port " << port << std::endl;

		while (true) {
				sockaddr_in client_addr;
				socklen_t client_addr_len = sizeof(client_addr);
				int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);

				if (client_fd < 0) {
						std::cerr << "Accept failed" << std::endl;
						continue;
				}

				std::cout << "Client connected" << std::endl;

				handle_client(client_fd, password);
		}

		close(server_fd);
		return 0;
}
