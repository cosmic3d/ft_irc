/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:17:59 by damendez          #+#    #+#             */
/*   Updated: 2024/09/04 15:19:31 by damendez         ###   ########.fr       */
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