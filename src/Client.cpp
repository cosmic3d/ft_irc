/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:43:13 by damendez          #+#    #+#             */
/*   Updated: 2024/09/12 18:55:18 by jenavarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(): _clientfd(0), _nickname(), _username(), _hostname(retrieveHostnameIp()), _authenticated(false), _registered(false), _joinedChannels() {};
Client::Client( int fd ): _clientfd(fd), _nickname(), _username(), _hostname(retrieveHostnameIp()), _authenticated(false), _registered(false), _joinedChannels() {};
Client::Client( const Client& x ) { *this = x; };


Client & Client::operator=( const Client& rhs )
{
	if (this == &rhs)
		return (*this);
	this->_clientfd = rhs._clientfd;
	this->_registered = rhs._registered;
	this->_nickname = rhs._nickname;
	this->_username = rhs._username;
	this->_hostname = retrieveHostnameIp();
	this->_authenticated = rhs._authenticated;
	this->_joinedChannels.insert(rhs._joinedChannels.begin(), rhs._joinedChannels.end());
	return (*this);
};

Client::~Client() {};

std::string	Client::getUsername()		const { return (this->_username); };
std::string	Client::getNickname()		const { return (this->_nickname.empty() ? "*" : this->_nickname); };
std::string	Client::getHostname()		const { return  (this->_hostname); };
bool		Client::getAuth()			const { return (this->_authenticated); };
int			Client::getClientfd()		const { return (this->_clientfd); };
int			Client::getRegistered()		const { return (this->_registered); };
std::string &Client::getReceiveBuffer() { return _receiveBuffer; };
void Client::appendToReceiveBuffer(const std::string &data) { _receiveBuffer += data; };

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


void		Client::setUsername(std::string username)	{ this->_username = username; };
void		Client::setNickname( std::string nickname )	{ this->_nickname = nickname; };
void		Client::setClientfd( int Clientfd )			{ this->_clientfd = Clientfd; };
void		Client::setRegistered( int Registered )		{ this->_registered = Registered; };
void		Client::setAuth( int Auth )					{ this->_authenticated = Auth; };

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
		channels.append("\tChannel Creator: " + it->second->getCreator()->getNickname() + "\n");
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
		else
			it->second->removeOperator(this->_clientfd);
		user.first->leaveChannel(it->second->getName());
		it = this->_joinedChannels.begin();
	}
	return ("");
};

std::string	Client::getUserInfo() const
{
	std::string	userInfo;
	userInfo.append("User Name: " + this->_username + "\n");
	userInfo.append("Nick Name: " + this->_nickname + "\n");
	userInfo.append("Host: \n");
	//userInfo.append("Joined Channels: " + std::to_string(this->_joinedChannels.size()) + "\n");
	userInfo.append("\n");
	return (userInfo);
};

std::string Client::mask() const {
    return getNickname() + "!" + getUsername() + "@" + getHostname();
}

bool Client::checkRegistered() {
    this->setRegistered(!this->getNickname().empty() && !this->getUsername().empty() && this->getAuth());
    return this->getRegistered();
}
//use getsockname with _socket to retrieve hostname (ip address)
std::string Client::retrieveHostnameIp() const {
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int res = getsockname(this->getClientfd(), (struct sockaddr *)&addr, &addr_size);
    if (res == -1)
    {
        return "Unknown";
    }
    return inet_ntoa(addr.sin_addr);
}

std::map<std::string, Channel *>	Client::getJoinedChannels() const { return (this->_joinedChannels); };
