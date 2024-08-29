/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:43:13 by damendez          #+#    #+#             */
/*   Updated: 2024/08/29 15:43:15 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string &name, Client *creator) {
    this->_name = name;
    this->_creator = creator;
    this->_onlineUsers = 1;
}

Channel::Channel(std::string channelName, Client *Creator) : _prefix(), _creator(Creator), _onlineUsers(1), _name(channelName), _key(), _topic(), _members(), _operators(), _voice(), _banned()
{
	this->_operators.insert(std::pair<int, Client *>(Creator->getClientfd(), Creator));
};
Channel::Channel(std::string channelName, std::string channelKey, Client *Creator) : _prefix(), _creator(Creator), _onlineUsers(1), _name(channelName), _key(channelKey), _topic(), _members(), _operators(), _voice(), _banned()
{
	this->_operators.insert(std::pair<int, Client *>(Creator->getClientfd(), Creator));
};

Channel::~Channel() {};

Channel& Channel::operator=( const Channel& rhs )
{
	if (this == &rhs)
		return (*this);
	this->_prefix = rhs._prefix;
	this->_onlineUsers = rhs._onlineUsers;
	this->_name = rhs._name;
	this->_members.insert(rhs._members.begin(), rhs._members.end());
	this->_operators.insert(rhs._operators.begin(), rhs._operators.end());
	this->_voice.insert(rhs._voice.begin(), rhs._voice.end());
	return (*this);
};

