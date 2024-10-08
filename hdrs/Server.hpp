/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:45:32 by damendez          #+#    #+#             */
/*   Updated: 2024/10/01 14:14:46 by jenavarr         ###   ########.fr       */
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
// #define MAX_CLIENTS 10

class Request;
class Client;
class Channel;

class Server {
    private:
        std::string                         _name; // server name
        int                                 _serverSocket; // fd for listening socket
        int                                 _port; // port number (identifies processes)
        std::string                         _password;
        std::vector<pollfd>                 _pollFds; // monitored by the server for events using poll()
        std::map<int, Client*>              _clients;
        std::map<std::string, Channel *>    _channels;

    private:
        std::string                 _joinChannel(Request request, int client_fd);
        int                         _createChannel( std::string ChannelName, int CreatorFd,  std::string ChannelKey = "" );
        void    _handleConnection(Server *server);
        void    _handleDisconnection(int clientSocket);
        void    _handleClient(int clientSocket);
        std::string _execute_command(const Request& req, int client_fd);
        std::string _handlePass(const Request& req, int client_fd);
        std::string _handleNick(const Request& req, int client_fd);
        std::string _handleUser(const Request& req, int client_fd);
        std::string _handleQuit(const Request& req, int client_fd);
        std::string _handleMode(const Request& req, int client_fd);
        std::string _handleInvite(const Request& req, int client_fd);
        std::string _handleTopic(const Request& req, int client_fd);
        std::string _handleKick( Request request, int client_fd );
        std::string _kickedFromChannel(std::string ChannelName, std::string message, std::vector<std::string> users, int client_fd);
        std::string	_printMessage(std::string num, std::string nickname, std::string message);

    // messages
    private:
        int		    _sendmsg(int destfd, std::string message);

        std::string	_privmsg(Request request, int i);
        std::string _privToUser(std::string User, std::string message, std::string cmd, int i);
        std::string _privToChannel(std::string ChannelName, std::string message, std::string cmd, int i);      
        Client      *getClientByName(const std::string &name) const;
        Channel     *getChannelByName(const std::string &name) const;
        int		    _findFdByNickName(std::string NickName);
        std::string	_notice(Request request, int i);
        std::string _sendToAllUsers(Channel *channel, int senderFd, std::string message);
        std::string _handlePart( Request request, int client_fd );

    public:
        Server(const std::string &name, int port, const std::string &password);
        ~Server();
        
        void    start();
        void    init();
        void    run();
        
        void    handleConnection();
        void    handleClient(int clientSocket); 
        void    handleAuthentication(int clientSocket, const std::string &message); 
        void    parseCommand(int clientSocket, const std::string &message);
        std::string execute_command(const Request& req, int client_fd);
        std::string format_message(const std::string& prefix, const std::string& command, const std::vector<std::string>& params);
        int         _partChannel( std::string ChannelName, int i, std::string message);

        //command handlers for IRC commands pending
        
};

#include "Request.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include "responses.hpp"

#endif