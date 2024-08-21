/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 14:19:12 by damendez          #+#    #+#             */
/*   Updated: 2024/08/21 16:15:04 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string &name, Client *mod) : _name(name), _moderator(mod){
    std::cout << name << " Channel has been created" << std::endl;
}

Channel::~Channel() {
       std::cout << _name << " Channel has been deleted" << std::endl;
}

void    Channel::addClient(Client *client) {
    _clients.push_back(client);
    broadcastMessage(client->getNickname() + " has joined the channnel.", client);
}

void    Channel::removeClient(Client *client) {
    _clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());
    broadcastMessage(client->getNickname() + " has left the channel", client); 
}

void    Channel::broadcastMessage(const std::string &message, Client *sender) {
    for (size_t i = 0; i < _clients.size(); ++i) {
        if (_clients[i] != sender) {
            send(_clients[i]->getSocket(), message.c_str(), message.length(), 0);
        }
    }
}

void Channel::setTopic(const std::string &topic) {
    _topic = topic;
}

bool Channel::isOperator(Client *client) {
    return client == _moderator;
}

const std::string& Channel::getName() const {
    return _name;
}

const std::string& Channel::getTopic() const {
    return _topic;
}