/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:43:13 by damendez          #+#    #+#             */
/*   Updated: 2024/08/21 08:51:07 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int socket) : _socket(socket), _authenticated(false) {
    std::cout << "_client[" << socket << "] created" << std::endl;
}

int Client::getSocket() const {
    return _socket;
}

void Client::setNickname(const std::string &nickname) {
    _nickname = nickname;
}

std::string Client::getNickname() const {
    return _nickname;
}

bool    isAuthenticated() const {
    return _authenticated;
}

void setAuthenticated(bool status) {
    _authenticated = status;
}
