/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:15:01 by damendez          #+#    #+#             */
/*   Updated: 2024/08/16 12:20:38 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port, const std::string &password) {
    this->_port = port;
    this->_password = password;
    this->_serverSocket = -1;
    std::cout << "Server constructor called" << std::endl;
}

Server::~Server() {
    // Check if server socket was opened
    if (_serverSocket != -1) {
        close(_serverSocket);
    }

    // TO-DO: Clean up allocated Client/Channel objects
}

void    Server::init() {
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket < 0) {
        throw std::runtime_error("Failed to create socket");
    }
    
    // Configure the server address structure
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;  // Use IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Bind to any available network interface
    serverAddr.sin_port = htons(_port);  // Convert port number to network byte order   

    // Bind the server socket to the specified IP address and port
    if (bind(_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Bind failed");
    }

    // listen for incoming connections
    if (listen(_serverSocket, 10) < 0) {
        throw std::runtime_error("listen failed");
    }

    // Prepare the server socket for polling
    pollfd serverPollFd;
    serverPollFd.fd = _serverSocket;
    serverPollFd.events = POLLIN;  // Monitor the socket for incoming connection requests
    _pollFds.push_back(serverPollFd);
}

// Method to run the server and handle events
void    Server::run() {
    while (true) {
        // Poll the set of file descriptors to see which ones are ready
        int pollCount = poll(_pollFds.data(), _pollFds.size(), -1);
        if (pollCount < 0) {
            throw std::runtime_error("Poll failed");
        }

        for (size_t i = 0; i < _pollFds.size(); ++i) {
            if (_pollFds[i].revents & POLLIN) { // check if there's data to read
                if (_pollFds[i].fd == _serverSocket) {
                    handleConnection(); // TO-DO Handle a new incoming connection
                } else {
                    handleClient(_pollFds[i].fd); // TO-DO Handle data from existing client
                }
            }
        }
    }
}

void    Server::handleConnection() {
    
}

void    Server::handleClient(int clientSocket) {
    
}

void    Server::handleAuthentication(int clientSocket, const std::string &message) {

}

void    Server::parseCommand(int clientSocket, const std::string &message) {
    
}