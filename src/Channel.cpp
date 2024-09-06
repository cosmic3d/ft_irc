/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:43:13 by damendez          #+#    #+#             */
/*   Updated: 2024/09/06 17:48:36 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

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
Client*							       Channel::getCreator() 		const { return (this->_creator); };

void	Channel::setPrefix(char prefix)			{ this->_prefix = prefix; };
void	Channel::setOnlineUsers(int online)		{ this->_onlineUsers = online; };
void	Channel::setName(std::string name)		{ this->_name = name; };
void	Channel::setKey(std::string key)		{ this->_key = key; };
void	Channel::setTopic(std::string topic)	{ this->_topic = topic; };

std::pair<Client *, int> Channel::findUserRole( int i )
{
	std::map<int, Client *>::iterator it = this->_members.find(i);
	if (it != this->_members.end())
		return (std::pair<Client *, int>(it->second, 0));
	it = this->_operators.find(i);
	if (it != this->_operators.end())
		return (std::pair<Client *, int>(it->second, 1));
	it = this->_voice.find(i);
	if (it != this->_voice.end())
		return (std::pair<Client *, int>(it->second, 2));
	return (std::pair<Client *, int>(NULL, -1));
};

int	Channel::addMember( Client *member )
{
	if (std::find(this->_banned.begin(), this->_banned.end(), member->getNickname()) != this->_banned.end())
		return (BANNEDFROMCHAN);
	if (this->_members.find(member->getClientfd()) == this->_members.end())
	{
		this->_members.insert(std::pair<int, Client *>(member->getClientfd(), member));
		this->_onlineUsers++;
		return (USERISJOINED);
	};
	return (-1);
};

int	Channel::addOperator( Client *member )
{
	if (std::find(this->_banned.begin(), this->_banned.end(), member->getNickname()) != this->_banned.end())
		return (BANNEDFROMCHAN);
	if (this->_operators.find(member->getClientfd()) == this->_operators.end())
	{
		this->_operators.insert(std::pair<int, Client *>(member->getClientfd(), member));
		this->_onlineUsers++;
		return (USERISJOINED);
	};
	return (-1);
};

int	Channel::banUser( Client *member )
{
	if (std::find(this->_banned.begin(), this->_banned.end(), member->getNickname()) != this->_banned.end())
		return (BANNEDFROMCHAN);
	this->_banned.push_back(member->getNickname());
	return (USERISBANNED);
};

void	Channel::removeOperator( int i)
{
	this->_operators.erase(i);
	this->_onlineUsers--;
};

void	Channel::removeVoice( int i)
{
	this->_voice.erase(i);
	this->_onlineUsers--;
};

void	Channel::removeBanned( std::string NickName )
{
	if (std::find(this->_banned.begin(), this->_banned.end(), NickName) != this->_banned.end())
		return ;
	this->_banned.erase(std::find(this->_banned.begin(), this->_banned.end(), NickName));
};

void	Channel::removeMember( int i)
{
	this->_members.erase(i);
	this->_onlineUsers--;
};

std::string		Channel::listAllUsers() const
{
	std::string		AllUsers(":");
	std::map<int, Client *>::const_iterator it = this->_operators.begin();
	while (it != this->_operators.end())
	{
		AllUsers.append("@" + it->second->getNickname() + " ");
		it++;
	}
	it = this->_members.begin();
	while (it != this->_members.end())
	{
		AllUsers.append(it->second->getNickname() + " ");
		it++;
	}
	it = this->_voice.begin();
	while (it != this->_voice.end())
	{
		AllUsers.append("+" + it->second->getNickname() + " ");
		it++;
	}
	return (AllUsers);
};

std::map<int, Client *>	Channel::getAllUsers() const
{
	std::map<int, Client *>	allUsers(this->_members.begin(), this->_members.end());
	allUsers.insert(this->_operators.begin(), this->_operators.end());
	allUsers.insert(this->_voice.begin(), this->_voice.end());
	return (allUsers);
};