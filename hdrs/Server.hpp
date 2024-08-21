/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:45:32 by damendez          #+#    #+#             */
/*   Updated: 2024/08/21 08:49:54 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <map>
#include <arpa/inet.h>
#include <poll.h>
#include <cstring>
#include "Client.hpp"

#define PORT 8080
#define MAX_CLIENTS 10

class Server {
    private:
        int                     _serverSocket; // fd for listening socket
        int                     _port; // port number (identifies processes)
        std::string             _password;
        std::vector<pollfd>     _pollFds; // monitored by the server for events using poll()
        std::map<int, Client*>            _clients; // TO-DO
        //std::map<std::string, Channel*>   _channels: // TO-DO
        
    public:
        Server(int port, const std::string &password);
        ~Server();
        
        void    start();
        void    init();
        void    run();
        
        void    handleConnection();
        void    handleClient(int clientSocket); // TO-DO
        void    handleAuthentication(int clientSocket, const std::string &message); // TO-DO
        void    parseCommand(int clientSocket, const std::string &message);// TO-DO

        //command handlers for IRC commands pending
        
};

#endif