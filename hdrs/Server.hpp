/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:45:32 by damendez          #+#    #+#             */
/*   Updated: 2024/08/27 10:17:19 by damendez         ###   ########.fr       */
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

class Server 
{
    private:
        int                     _serverSocket; // fd for listening socket
        int                     _port; // port number (identifies processes)
        std::string             _name;
        std::string             _password;
        std::vector<pollfd>     _pollFds; // monitored by the server for events using poll()
        std::map<int, Client*>            _clients;
        //std::map<std::string, Channel*>   _channels;
        
    private:
        std::string                 _joinChannel(Request request, int i); // TO-DO
        int                         _createPrvChannel( std::string ChannelName, std::string ChannelKey, int CreatorFd); // TO-DO
        int                         _createChannel( std::string ChannelName, int CreatorFd ); // TO-DO
        std::vector<std::string>    _commaSeparator(std::string arg)


        std::string             _printMessage(std::string num, std::string nickname, std::string message);
        
    public:
        Server(int port, const std::string &password);
        ~Server();
        
        void    start();
        void    init();
        void    run();
        
        void    handleConnection();
        void    handleClient(int clientSocket);
        void    handleAuthentication(int clientSocket, const std::string &message); // TO-DO
        void    parseCommand(int clientSocket, const std::string &message);// TO-DO

        //command handlers for IRC commands pending
        
};

#endif