/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:17:59 by damendez          #+#    #+#             */
/*   Updated: 2024/09/04 16:14:59 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::_sendmsg(int destfd, std::string message) {
    int sent = 0;
    int bytesLeft = message.length();
    int b;

    while (sent < (int)message.length) {
        b = send(destfd, message.c_str() + sent, bytesLeft, 0);
        if (b == -1)
            break;
        sent += b;
        bytesLeft -= b;
    }
    return (b == -1 ? -1 : 0);
}

std::string Server::_sendToAllUsers(Channel *channel, int senderFd, std::string message) {
    std::map<int, Client *> allusers = channel->getAllUsers(); // TO-DO
    std::map<int, Client *>::iterator it = allusers.begin();
    std::string reply = this->_clients[senderFd]->getUserPerfix(); // TO-DO
    reply.append(message);
    while (it != allusers.end()) {
        if (senderFd != it->first) {
            if (_sendmsg(it->first, reply) == -1) {
				std::cout << "_sendall() error: " << strerror(errno) << std::endl;
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
        return (_privToChannel(request.params[0], request.params[1], i));
    }
    return ("");
};