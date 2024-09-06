/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:15:01 by damendez          #+#    #+#             */
/*   Updated: 2024/08/21 12:49:51 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(const std::string &name, int port, const std::string &password) {
Server::Server(const std::string &name, int port, const std::string &password) {
    this->_port = port;
    this->_password = password;
    this->_serverSocket = -1;
    this->_name = name;
    print_debug("Server created", colors::green, colors::italic);
    print_debug("Server name: " + this->_name, colors::green, colors::reset);
    this->_name = name;
    print_debug("Server created", colors::green, colors::italic);
    print_debug("Server name: " + this->_name, colors::green, colors::reset);
}

Server::~Server() {
    // Check if server socket was opened
    if (_serverSocket != -1) {
        close(_serverSocket);
    }

    // Limpiar memoria de clientes
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        delete it->second;
    }
    _clients.clear();
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
                    _handleConnection(); // TO-DO Handle a new incoming connection
                } else {
                    _handleClient(_pollFds[i].fd); // TO-DO Handle data from existing client
                }
            }
        }
    }
}

void    Server::_handleConnection() {
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
    print_debug("New client connected: " + itos(clientSocket), colors::green, colors::bold);
    _clients[clientSocket] = new Client(clientSocket);
}

void    Server::handleDisconnection(int clientSocket) {
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
    //remove client from client list, freeing memory
    delete _clients[clientSocket];
    _clients.erase(clientSocket);
    return;
}



void Server::handleClient(int clientSocket) {
    char buffer[512];

    // Recibir datos del cliente de forma no bloqueante
    std::cout << "Receiving data from client" << std::endl;
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead == 0) {
        handleDisconnection(clientSocket);
        return;
    }
    else if (bytesRead < 0) {
        //see value of errno and print error message
        std::string error_msg = strerror(errno);
        print_debug("Failed to receive data from client: " + error_msg, colors::red, colors::bold);
        return;
    }

    buffer[bytesRead] = '\0'; // Asegura que el buffer es un string válido
    std::string receivedData(buffer);

    // Agregar los datos recibidos al buffer persistente del cliente
    _clients[clientSocket]->appendToReceiveBuffer(receivedData);
    print_debug("Received data from client: " + receivedData, colors::cyan, colors::bold);
    print_debug("Client buffer: " + _clients[clientSocket]->getReceiveBuffer(), colors::bright_magenta, colors::bold);

    // Obtener el buffer de recepción actual del cliente
    std::string &clientBuffer = _clients[clientSocket]->getReceiveBuffer();

    // Procesar comandos completos en el buffer
    size_t pos;
    while ((pos = clientBuffer.find("\r\n")) != std::string::npos) {
        std::string command = clientBuffer.substr(0, pos);
        clientBuffer.erase(0, pos + 2); // Eliminar comando del buffer

        print_debug("[" + itos(clientSocket) + "]", colors::cyan, colors::bold);
        print_debug("CLIENT: " + command, colors::grey, colors::on_bright_cyan);

        Request req = parse_request(command);
        std::string response = execute_command(req, clientSocket);

        if (!response.empty()) {
            print_debug("SERVER: " + response, colors::cyan, colors::on_bright_grey);
            if (send(clientSocket, response.c_str(), response.length(), 0) < 0) {
                std::string error_msg = strerror(errno);
                print_debug("Failed to send data to client: " + error_msg, colors::red, colors::bold);
                return;
            }
        }
    }
}
