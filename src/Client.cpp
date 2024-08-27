/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:43:13 by damendez          #+#    #+#             */
/*   Updated: 2024/08/27 15:16:19 by jenavarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int socket) : _socket(socket), _authenticated(false) {
    std::cout << "_client[" << socket << "] created" << std::endl;
}

//destructor
Client::~Client() {
    // this->_socket = -1;
    std::cout << "_client[" << _socket << "] destroyed" << std::endl;
}

int Client::getSocket() const {
    return _socket;
}

void Client::setNickname(const std::string &nickname) {
    _nickname = nickname;
}

void Client::setUsername(const std::string &username) {
    _username = username;
}

std::string Client::getNickname() const {
    return _nickname.empty() ? "*" : _nickname; // return "*" if nickname is empty, else return nickname
}

std::string Client::getUsername() const {
    return _username;
}

bool Client::isAuthenticated() const {
    return _authenticated;
}

void Client::setAuthenticated(bool status) {
    _authenticated = status;
}

bool Client::isRegistered() const {
    return _registered;
}

void Client::setRegistered(bool status) {
    _registered = status;
}

bool Client::checkRegistered() {
    this->setRegistered(!this->getNickname().empty() && !this->getUsername().empty() && this->isAuthenticated());
    return this->isRegistered();
}