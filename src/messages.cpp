/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:17:59 by damendez          #+#    #+#             */
/*   Updated: 2024/09/06 13:34:50 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_sendmsg(int destfd, std::string message) {
    int sent = 0;
    int bytesLeft = message.length();
    int b;

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
    std::string reply = this->_clients[senderFd]->getUserPerfix();
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
    if (!this->_clients[i]->getRegistered())
        return (_printMessage("451", this->_clients[i]->getNickName(), ":You have not registered"));
    if (request.params.size() < 2)
        return (_printMessage("461", this->_clients[i]->getNickName(), ":Not enough parameters"));
    if (request.params.size() == 2)
    {
        if (request.params[0].find(",") != std::string::npos)
            return (_printMessage("401", this->_clients[i]->getNickName(), request.params[0].append(" :Too many recipients.")));
		if (request.params[0][0] != '&' && request.params[0][0] != '#' && request.params[0][0] != '+' && request.params[0][0] != '!')
            return (_privToUser(request.params[0], request.params[1], "PRIVMSG", i));
        return (_privToChannel(request.params[0], request.params[1], "PRIVMSG", i));
    }
    return ("");
};

std::string Server::_privToUser(std::string User, std::string message, std::string cmd, int i) {
    int userFd = _findFdByNickName(User);
    if (userFd == USERNOTFOUND) {
        return (_printMessage("401", this->_clients[i]->getNickName(), User.append(" :No such nick")));
    }   
	std::string reply = this->_clients[i]->getUserPerfix();
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
		if (user.second == -1 )
			return (_printMessage("404", this->_clients[i]->getNickName(), ChannelName.append(" :Cannot send to channel")));
		std::string msg(cmd + ChannelName + " :" + message + "\n");
		_sendToAllUsers(it->second, i, msg);
	}
	else
		return (_printMessage("401", this->_clients[i]->getNickName(), ChannelName.append(" :No such nick/channel")));
	return ("");
};

int		Server::_findFdByNickName(std::string NickName)
{
	std::map<int, Client *>::iterator it = this->_clients.begin();
	while(it != this->_clients.end())
	{
		if (it->second->getNickName() == NickName)
			return (it->second->getClientfd());
		it++;
	}
	return (USERNOTINCHANNEL);
};