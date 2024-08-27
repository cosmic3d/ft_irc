/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:15:01 by damendez          #+#    #+#             */
/*   Updated: 2024/08/27 15:49:07 by jenavarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(const std::string &name, int port, const std::string &password) {
    this->_port = port;
    this->_password = password;
    this->_serverSocket = -1;
    this->_name = name;
    this->_capabilitites = std::vector<std::string>();
    this->_capabilitites.push_back("invite-notify");
    print_debug("Server created", colors::green, colors::italic);
    print_debug("Server name: " + this->_name, colors::green, colors::reset);
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
    while (42) {
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
    print_debug("New client connected: " + itos(clientSocket), colors::green, colors::bold);
    _clients[clientSocket] = new Client(clientSocket);
}

void    Server::handleClient(int clientSocket) {
    // Size follows IRC protocol max message length and is memory efficient
    char buffer[512];

    // Recieve data from client
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
       if (bytesRead <= 0) {
        print_debug("Client disconnected", colors::red, colors::bold);
        // If the client disconnected or an error occurred, close the connection
        close(clientSocket);
        //remove client socket from poll list
        for (size_t i = 0; i < _pollFds.size(); ++i) {
            if (_pollFds[i].fd == clientSocket) {
                _pollFds.erase(_pollFds.begin() + i);
                break;
            }
        }
        delete _clients[clientSocket];
        _clients.erase(clientSocket);
        return;
    }

    buffer[bytesRead] = '\0';
    std::string message(buffer);
    print_debug("[" + itos(clientSocket) + "]", colors::cyan, colors::bold);
    print_debug("CLIENT: " + message, colors::grey, colors::on_bright_cyan);
    //split buffer using CR-LF as delimiter. without split function
    std::vector<std::string> messages;
    std::string delimiter = "\r\n";
    size_t pos = 0;
    std::string token;
    while ((pos = message.find(delimiter)) != std::string::npos) {
        token = message.substr(0, pos);
        messages.push_back(token);
        message.erase(0, pos + delimiter.length());
    }
    messages.push_back(message);

    //parse, execute and send response for each message
    for (size_t i = 0; i < messages.size(); i++) {
        Request req = parse_request(messages[i]);
        // req.print();
        std::string response = execute_command(req, clientSocket);
        if (response.empty()) {
            continue;
        }
        print_debug("SERVER: " + response, colors::cyan, colors::on_bright_grey);
        send(clientSocket, response.c_str(), response.length(), 0);
    }
}