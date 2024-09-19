/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:17:59 by damendez          #+#    #+#             */
/*   Updated: 2024/09/12 18:33:27 by jenavarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_sendmsg(int destfd, std::string message) {
    int sent = 0;
    int bytesLeft = message.length();
    int b;

    print_debug("SERVER (sending): " + message, colors::cyan, colors::on_bright_grey);
    while (sent < (int)message.length()) {
        b = send(destfd, message.c_str() + sent, bytesLeft, 0);
        if (b == -1)
            break;
        sent += b;
        bytesLeft -= b;
    }
    return (b == -1 ? -1 : 0);
}

std::string Server::_sendToAllUsers(Channel *channel, int senderFd, std::string message) {
    std::map<int, Client *> allusers = channel->getAllUsers();
    std::map<int, Client *>::iterator it = allusers.begin();
    while (it != allusers.end()) {
        if (senderFd != it->first) {
            if (_sendmsg(it->first, message) == -1) {
				std::cout << "_sendmsg() error: _sendToAllUsers" << std::endl;
				return ("");
            }
        }
        it++;
    }
    return ("");
};

std::string Server::_privmsg(Request request, int i) {
    if (request.params.size() < 2) {
        std::vector<std::string> params;
        params.push_back(this->_clients[i]->getNickname());
        params.push_back("Not enough parameters");
        return format_message(_name, ERR_NEEDMOREPARAMS, params); 
    }
    if (request.params.size() == 2)
    {
        if (request.params[0].find(",") != std::string::npos) {
            std::vector<std::string> params;
            params.push_back(this->_clients[i]->getNickname());
            params.push_back("Too many recipients.");
            return format_message(_name, ERR_TOOMANYTARGETS, params); 
        }
		if (request.params[0][0] != '&' && request.params[0][0] != '#' && request.params[0][0] != '+' && request.params[0][0] != '!')
            return (_privToUser(request.params[0], request.params[1], "PRIVMSG", i));
        return (_privToChannel(request.params[0], request.params[1], "PRIVMSG", i));
    }
    return ("");
};

std::string Server::_privToUser(std::string User, std::string message, std::string cmd, int i) {
    int userFd = this->getClientByName(User)->getClientfd();
    if (userFd == USERNOTFOUND) {
        std::vector<std::string> params;
        params.push_back(this->_clients[i]->getNickname());
        params.push_back(User.append(" :No such nick"));
        return format_message(_name, ERR_NOSUCHNICK, params); 
    }
    std::vector<std::string> params;
    params.push_back(this->_clients[userFd]->getNickname());
    params.push_back(message);
	if (_sendmsg(userFd, format_message(_clients[i]->mask(), cmd, params)) == -1)
				std::cout << "_sendmsg() error: _privToUser" << std::endl;
	return ("");
};

std::string 	Server::_privToChannel(std::string ChannelName, std::string message, std::string cmd, int i)
{
	std::map<std::string, Channel *>::iterator it = this->_channels.find(ChannelName);
	if (it != this->_channels.end())
	{
		if (this->_clients[i]->isJoined(ChannelName) == 0) {
            std::vector<std::string> params;
            params.push_back(this->_clients[i]->getNickname());
            params.push_back(ChannelName.append("Cannot send to channel"));
            return format_message(_name, ERR_CANNOTSENDTOCHAN, params); 
        }
        std::vector<std::string> params;
        params.push_back(ChannelName);
        params.push_back(message);
		std::string msg(format_message(_clients[i]->mask(), cmd, params));
		_sendToAllUsers(it->second, i, msg);
	}
	else {
        std::vector<std::string> params;
        params.push_back(this->_clients[i]->getNickname());
        params.push_back("No such nick/channel");
        return format_message(_name, ERR_NOSUCHNICK, params); 
    }
	return ("");
};