/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:43:13 by damendez          #+#    #+#             */
/*   Updated: 2024/09/06 16:46:19 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(): _clientfd(0), _nickname(), _username(), _fullname(), _ID(), _authenticated(false), _registered(false), _isOperator(false), _joinedChannels() {};
Client::Client( int fd ): _clientfd(fd), _nickname(), _username(), _fullname(), _ID(), _authenticated(false), _registered(false), _isOperator(false), _joinedChannels() {};
Client::Client( const Client& x ) { *this = x; };


Client & Client::operator=( const Client& rhs )
{
	if (this == &rhs)
		return (*this);
	this->_clientfd = rhs._clientfd;
	this->_registered = rhs._registered;
	this->_isOperator = rhs._isOperator;
	this->_nickname = rhs._nickname;
	this->_username = rhs._username;
	this->_fullname = rhs._fullname;
	this->_authenticated = rhs._authenticated;
	this->_ID = rhs._ID;
	this->_joinedChannels.insert(rhs._joinedChannels.begin(), rhs._joinedChannels.end());
	return (*this);
};

Client::~Client() {};

std::string	Client::getUserName()		const { return (this->_username); };
std::string	Client::getNickName()		const { return (this->_nickname); };
std::string	Client::getFullName()		const { return (this->_fullname); };
std::string Client::getID()				const { return (this->_ID); }
bool		Client::getAuth()			const { return (this->_authenticated); };
int			Client::getClientfd()		const { return (this->_clientfd); };
int			Client::getRegistered()		const { return (this->_registered); };
int			Client::getisOperator()		const { return (this->_isOperator); };

// int			Client::getMode(char mode)	const
// {
// 	if (mode == 'a')
// 		return this->_modes.away;
// 	else if (mode == 'i')
// 		return this->_modes.invisible;
// 	else if (mode == 'w')
// 		return this->_modes.wallops;
// 	else if (mode == 'r')
// 		return this->_modes.restricted;
// 	else if (mode == 'o')
// 		return this->_modes.op;
// 	else if (mode == 'O')
// 		return this->_modes.localOp;
// 	else if (mode == 's')
// 		return this->_modes.server;
// 	return (0);
// }


void		Client::setUserName(std::string UserName)	{ this->_username = UserName; };
void		Client::setNickName( std::string NickName )	{ this->_nickname = NickName; };
void		Client::setFullName( std::string FullName )	{ this->_fullname = FullName; };
void		Client::setID( std::string ID )				{ this->_ID = ID; };
void		Client::setClientfd( int Clientfd )			{ this->_clientfd = Clientfd; };
void		Client::setRegistered( int Registered )		{ this->_registered = Registered; };
void		Client::setAuth( int Auth )					{ this->_authenticated = Auth; };
void		Client::setIsOperator(int isOperator)
{
	this->_isOperator = isOperator;
	//this->_modes.op = isOperator;
	//this->_modes.localOp = isOperator;
};
// void		Client::setMode(int value, char mode)
// {
// 	if (mode == 'i')
// 		this->_modes.invisible = value;
// 	else if (mode == 'w')
// 		this->_modes.wallops = value;
// 	else if (mode == 'r' && value == 1)
// 		this->_modes.restricted = value;
// 	else if (mode == 'o' && value == 0)
// 		this->_modes.op = value;
// 	else if (mode == 'O' && value == 0)
// 		this->_modes.localOp = value;
// 	else if (mode == 's')
// 		this->_modes.server = value;
// };

int		Client::isJoined( std::string ChannelName ) const
{
	if (this->_joinedChannels.find(ChannelName) != this->_joinedChannels.end())
		return (1);
	return (0);
};

void	Client::joinChannel( std::string ChannelName, Channel *channel )
{
	if (!isJoined(ChannelName))
		this->_joinedChannels.insert(std::pair<std::string, Channel *>(ChannelName, channel));
};

std::string	Client::joinedChannels() const
{
	std::string	channels;
	std::map<std::string, Channel *>::const_iterator it = this->_joinedChannels.begin();
	while (it != this->_joinedChannels.end())
	{
		channels.append(it->first + ":\n");
		channels.append("\tChannel Name: " + it->first + "\n");
		channels.append("\tChannel Name inside class: " + it->second->getName() + "\n");
		channels.append("\tChannel Creator: " + it->second->getCreator()->getFullName() + "\n");
		it++;
	};
	return (channels);
};

void	Client::leaveChannel( std::string ChannelName )
{
	this->_joinedChannels.erase(ChannelName);
};

std::string	Client::leaveAllChannels()
{
	std::map<std::string, Channel *>::iterator it = this->_joinedChannels.begin();
	while( it != this->_joinedChannels.end())
	{
		std::pair<Client *, int> user(it->second->findUserRole(this->_clientfd)); // TO-DO
		if (user.second == 0)
			it->second->removeMember(this->_clientfd);
		else if (user.second == 1)
			it->second->removeOperator(this->_clientfd);
		else
			it->second->removeVoice(this->_clientfd);
		user.first->leaveChannel(it->second->getName());
		it = this->_joinedChannels.begin();
	}
	return ("");
};

std::string	Client::getUserInfo() const
{
	std::string	userInfo;
	userInfo.append("User Name: " + this->_username + "\n");
	userInfo.append("Full Name: " + this->_fullname + "\n");
	userInfo.append("Nick Name: " + this->_nickname + "\n");
	userInfo.append("Host: \n");
	//userInfo.append("Joined Channels: " + std::to_string(this->_joinedChannels.size()) + "\n");
	userInfo.append("\n");
	return (userInfo);
};

std::string		Client::getUserPerfix() const { return (":" + this->_nickname + "!" + this->_username + "@" + SERVER_NAME + " "); };

std::map<std::string, Channel *>	Client::getJoinedChannels() const { return (this->_joinedChannels); };
