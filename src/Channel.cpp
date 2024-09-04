/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:43:13 by damendez          #+#    #+#             */
/*   Updated: 2024/09/04 14:39:54 by damendez         ###   ########.fr       */
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

char							const &Channel::getPrefix()			const { return this->_prefix; };
int								const &Channel::getOnlineUsers()	const { return this->_onlineUsers; };
std::string						const &Channel::getName() 			const { return this->_name; };
std::string						const &Channel::getKey()			const { return this->_key; };
std::string						const &Channel::getTopic()			const { return this->_topic; };
std::map<int, Client *>			const &Channel::getMembers()		const { return this->_members; };
std::map<int, Client *>			const &Channel::getOperators()		const { return this->_operators; };
std::map<int, Client *>			const &Channel::getVoice()			const { return this->_voice; };

Client*		Channel::getCreator() const { return (this->_creator); };

void	Channel::setPrefix(char prefix)			{ this->_prefix = prefix; };
void	Channel::setOnlineUsers(int online)		{ this->_onlineUsers = online; };
void	Channel::setName(std::string name)		{ this->_name = name; };
void	Channel::setKey(std::string key)		{ this->_key = key; };
void	Channel::setTopic(std::string topic)	{ this->_topic = topic; };