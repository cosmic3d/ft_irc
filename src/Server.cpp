/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:15:01 by damendez          #+#    #+#             */
/*   Updated: 2024/08/21 12:49:51 by damendez         ###   ########.fr       */
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

void    Server::start() {
    init();
    run();
}

void    Server::init() {
    // socket for listening for client connection requests
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket < 0) {
        throw std::runtime_error("Failed to create socket");
    }
    
    // Configure the server address structure (network address and port number)
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;  // Use IPv4, the server knows what kind of address to expect
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // IP address of the server that clients use to connect to the server
    serverAddr.sin_port = htons(_port);  // Convert port number to network byte order   

    // Bind the server socket to the specified IP address and port
    // Binding tells the os that serverSocket will handle connections to the IP address and port
    if (bind(_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Bind failed");
    }

    // mark serverSocket as the socket for accepting incoming connections
    if (listen(_serverSocket, 10) < 0) {
        throw std::runtime_error("listen failed");
    }

    // Prepare the server socket for polling (monitoring multiple clients)
    pollfd serverPollFd;
    serverPollFd.fd = _serverSocket;
    serverPollFd.events = POLLIN;  // Bitmask for incoming data events
    _pollFds.push_back(serverPollFd);
}

/*
 * Method to run the server and handle events 
 * pollCount tells the server how many of the monitored sockets 
 * have events that need handling. 
 * The server will then loop through _pollFds to find out which 
 * sockets have activity and respond accordingly.
*/
void    Server::run() {
    while (true) {
        // Poll the set of file descriptors to see which ones are ready
        // data() returns a pointer to the first element of _pollFds, which is how poll() expects to receive the list of pollfd structures.
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
    int clientSocket = accept(_serverSocket, NULL, NULL);
    if (clientSocket < 0) {
        std::cerr<< "Failed to accept client" << std::endl;
        return;
    }
    
    // Add new client to poll list
    pollfd clientPollfd;
    clientPollfd.fd = clientSocket;
    clientPollfd.events = POLLIN;
    _pollFds.push_back(clientPollfd);

    // Add new client to client class list (adds to clSo index for constant time access)
    // _clients[clientSocket] = new Client(clientSocket);
}

void    Server::handleClient(int clientSocket) {
    // Size follows IRC protocol max message length and is memory efficient
    char buffer[512];

    // Recieve data from client
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
       if (bytesRead <= 0) {
        // If the client disconnected or an error occurred, close the connection
        close(clientSocket);
        _pollFds.erase(std::remove_if(_pollFds.begin(), _pollFds.end(), 
                    [clientSocket](pollfd pfd){ return pfd.fd == clientSocket; }), _pollFds.end());
        delete _clients[clientSocket];
        _clients.erase(clientSocket);
        return;
    }

    buffer[bytesRead] = '\0';
    std::string message(buffer);

    // Check if client is authenticated before parsing command
    if (!_clients[clientSocket]->isAuthenticated()) {
        handleAuthentication(clientSocket, message);  // Handle authentication first
        return;
    }

    // Parse and process the recieved command
    parseCommand(clientSocket, message);
}

void    Server::handleAuthentication(int clientSocket, const std::string &message) {
    if (message == _password) {
        _clients[clientSocket]->setAuthenticated(true);
        std::cout << "clientSocket: " << clientSocket << " has been authenticated" << std::endl;
    } else {
        // Send an error message and close the connection if the password is incorrect
        close(clientSocket);
        _pollFds.erase(std::remove_if(_pollFds.begin(), _pollFds.end(), 
                    [clientSocket](pollfd pfd){ return pfd.fd == clientSocket; }), _pollFds.end());
        delete _clients[clientSocket];
        _clients.erase(clientSocket);
    }
}