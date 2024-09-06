/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:17:59 by damendez          #+#    #+#             */
/*   Updated: 2024/09/06 19:19:41 by damendez         ###   ########.fr       */
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
    std::string reply = this->_clients[senderFd]->formatPrefix();
    reply.append(message);
    while (it != allusers.end()) {
        if (senderFd != it->first) {
            if (_sendmsg(it->first, reply) == -1) {
				std::cout << "_sendmsg() error: _sendToAllUsers" << std::endl;
				return ("");
            }
        }
        it++;
    }
    return ("");
};

std::string Server::_privmsg(Request request, int i) {
    if (!this->_clients[i]->getRegistered()) {
        std::vector<std::string> params;
        params.push_back(this->_clients[i]->getNickname());
        params.push_back("You have not registered");
        return format_message(_name, ERR_NOTREGISTERED, params);
    }
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
    int userFd = _findFdByNickName(User);
    if (userFd == USERNOTFOUND) {
        std::vector<std::string> params;
        params.push_back(this->_clients[i]->getNickname());
        params.push_back(User.append(" :No such nick"));
        return format_message(_name, ERR_NOSUCHNICK, params); 
    }   
	std::string reply = this->_clients[i]->formatPrefix();
	reply.append(cmd + " " + User + " :" + message + "\n");
	if (_sendmsg(userFd, reply) == -1)
				std::cout << "_sendmsg() error: _privToUser" << std::endl;
	return ("");
};

std::string 	Server::_privToChannel(std::string ChannelName, std::string message, std::string cmd, int i)
{
	std::map<std::string, Channel *>::iterator it = this->_allChannels.find(ChannelName);
	if (it != this->_allChannels.end())
	{
		std::pair<Client *, int> user = it->second->findUserRole(i);
		if (user.second == -1 ) {
            std::vector<std::string> params;
            params.push_back(this->_clients[i]->getNickname());
            params.push_back(ChannelName.append(" :Cannot send to channel"));
            return format_message(_name, ERR_CANNOTSENDTOCHAN, params); 
        }
		std::string msg(cmd + ChannelName + " :" + message + "\n");
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

int		Server::_findFdByNickName(std::string NickName)
{
	std::map<int, Client *>::iterator it = this->_clients.begin();
	while(it != this->_clients.end())
	{
		if (it->second->getNickname() == NickName)
			return (it->second->getClientfd());
		it++;
	}
	return (USERNOTINCHANNEL);
};