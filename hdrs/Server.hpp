/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:45:32 by damendez          #+#    #+#             */
/*   Updated: 2024/09/06 19:06:19 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
#include <map>
#include <arpa/inet.h>
#include <poll.h>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>

#include <cerrno>

#define SERVER_NAME "LittleBigServer"
#define PORT 8080
#define MAX_CLIENTS 10

class Request;
class Client;
class Channel;

class Server {
    private:
        std::string             _name; // server name
        int                     _serverSocket; // fd for listening socket
        int                     _port; // port number (identifies processes)
        std::string             _password;
        std::vector<pollfd>     _pollFds; // monitored by the server for events using poll()
        std::map<int, Client*>            _clients; // TO-DO
        std::map<std::string, Channel*>   _channels; // TO-DO
        std::map<std::string, Channel *>    _allChannels;

    private:
        std::string                 _joinChannel(Request request, int client_fd);
        int                         _createPrvChannel( std::string ChannelName, std::string ChannelKey, int CreatorFd); // TO-DO
        int                         _createChannel( std::string ChannelName, int CreatorFd ); // TO-DO
        void    _handleConnection();
        void    _handleDisconnection(int clientSocket);
        void    _handleClient(int clientSocket); // TO-DO
        std::string _execute_command(const Request& req, int client_fd);
        std::string _handlePass(const Request& req, int client_fd);
        std::string _handleNick(const Request& req, int client_fd);
        std::string _handleUser(const Request& req, int client_fd);
        std::string _handleQuit(const Request& req, int client_fd);
        std::string _handleOperator(const Request& req, int client_fd);
        std::string	_printMessage(std::string num, std::string nickname, std::string message);

    // messages
    private:
        int		    _sendmsg(int destfd, std::string message);
        std::string _sendToAllUsers(Channel *channel, int senderFd, std::string message);

        std::string	_privmsg(Request request, int i);
        std::string _privToUser(std::string User, std::string message, std::string cmd, int i);
        std::string _privToChannel(std::string ChannelName, std::string message, std::string cmd, int i);      
        int         _findFdByNickName(std::string NickName);

    public:
        Server(const std::string &name, int port, const std::string &password);
        ~Server();
        
        void    start();
        void    init();
        void    run();
        
        void    handleConnection();
        void    handleClient(int clientSocket); // TO-DO
        void    handleAuthentication(int clientSocket, const std::string &message); // TO-DO
        void    parseCommand(int clientSocket, const std::string &message);// TO-DO
        std::string execute_command(const Request& req, int client_fd);
        std::string format_message(const std::string& prefix, const std::string& command, const std::vector<std::string>& params);

        //command handlers for IRC commands pending
        
};

#include "Request.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include "responses.hpp"

#endif