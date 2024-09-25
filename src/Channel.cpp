/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:43:13 by damendez          #+#    #+#             */
/*   Updated: 2024/09/12 18:58:43 by jenavarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string channelName, Client *Creator) : _prefix(), _creator(Creator), _onlineUsers(1), _userLimit(0), _name(channelName), _key(), _topic(), _members(), _operators(), _banMasks(), _inviteMasks(), _inviteOnly(false), _topicRestricted(false)
{
	this->_operators.insert(std::pair<int, Client *>(Creator->getClientfd(), Creator));
};
Channel::Channel(std::string channelName, std::string channelKey, Client *Creator) : _prefix(), _creator(Creator), _onlineUsers(1), _userLimit(0), _name(channelName), _key(channelKey), _topic(), _members(), _operators(), _banMasks(), _inviteMasks(), _inviteOnly(false), _topicRestricted(false)
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
	this->_userLimit = rhs._userLimit;
	this->_name = rhs._name;
	//insert some other way
	this->_key = rhs._key;
	this->_topic = rhs._topic;
	this->_members = rhs._members;
	this->_operators = rhs._operators;
	this->_banMasks = rhs._banMasks;
	this->_inviteMasks = rhs._inviteMasks;
	this->_inviteOnly = rhs._inviteOnly;
	this->_topicRestricted = rhs._topicRestricted;
	return (*this);
};

char							const &Channel::getPrefix()			const { return this->_prefix; };
int								const &Channel::getOnlineUsers()	const { return this->_onlineUsers; };
std::string						const &Channel::getName() 			const { return this->_name; };
std::string						const &Channel::getKey()			const { return this->_key; };
std::string						const &Channel::getTopic()			const { return this->_topic; };
std::map<int, Client *>			const &Channel::getMembers()		const { return this->_members; };
std::map<int, Client *>			const &Channel::getOperators()		const { return this->_operators; };
Client*							       Channel::getCreator() 		const { return (this->_creator); };
bool												Channel::getTopicRestricted() const { return this->_topicRestricted; };
bool												Channel::getInviteOnly()	const { return this->_inviteOnly; };
std::string								Channel::getModes() const
{
	std::string modes;
	if (this->_inviteOnly)
		modes.append("i");
	if (this->_topicRestricted)
		modes.append("t");
	if (this->_userLimit > 0)
		modes.append("l");
	if (this->_key.length() > 0)
		modes.append("k");

	if (this->_userLimit > 0)
	{
		modes.append(" ");
		modes.append(itos(this->_userLimit));
	}
	return (modes);
};

void	Channel::setPrefix(char prefix)			{ this->_prefix = prefix; };
void	Channel::setOnlineUsers(int online)		{ this->_onlineUsers = online; };
void	Channel::setName(std::string name)		{ this->_name = name; };
void	Channel::setKey(std::string key)		{ this->_key = key; };
void	Channel::setTopic(std::string topic)	{ this->_topic = topic; };
void	Channel::setInviteOnly(bool inviteOnly) { this->_inviteOnly = inviteOnly; };
void	Channel::setTopicRestricted(bool topicRestricted) { this->_topicRestricted = topicRestricted; };

bool	Channel::isOperator( int i ) const
{
	if (this->_operators.find(i) != this->_operators.end())
		return (true);
	return (false);
};

bool	Channel::isMember( int i ) const
{
	if (this->_members.find(i) != this->_members.end())
		return (true);
	return (false);
};

int	Channel::addMember( Client *member )
{
	if (this->_banMasks.size() > 0 && matchMaskList(this->_banMasks, member->mask())) { //Está banneado por máscara
		return (BANNEDFROMCHAN);
	}

	if (this->_inviteOnly)
	{
		if (matchMaskList(this->_inviteMasks, member->mask()) == false) { //No está invitado por máscara
			return (NOTINVITED);
		}
	}

	if (this->_userLimit > 0 && this->_onlineUsers >= this->_userLimit) //Está lleno
		return (CHANNELISFULL);
	
	if (this->_members.find(member->getClientfd()) == this->_members.end() && this->_operators.find(member->getClientfd()) == this->_operators.end())
	{
		this->_members.insert(std::pair<int, Client *>(member->getClientfd(), member));
		this->_onlineUsers++;
		return (USERISJOINED);
	};
	return (-1);
};

int	Channel::addOperator( Client *member )
{
	if (!this->isOperator(member->getClientfd()) && this->isMember(member->getClientfd()))
	{
		this->_operators.insert(std::pair<int, Client *>(member->getClientfd(), member));
		this->_onlineUsers++;
		this->_members.erase(member->getClientfd());
		return (USERISJOINED);
	};
	return (-1);
};

void	Channel::removeOperator( int i)
{
	this->_operators.erase(i);
	this->_onlineUsers--;
};

void	Channel::removeMember( int i)
{
	this->_members.erase(i);
	this->_onlineUsers--;
};

std::string		Channel::listAllUsers() const
{
	std::string		AllUsers;
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
	return (AllUsers);
};

std::map<int, Client *>	Channel::getAllUsers() const
{
	std::map<int, Client *>	allUsers(this->_members.begin(), this->_members.end());
	allUsers.insert(this->_operators.begin(), this->_operators.end());
	return (allUsers);
};

// Función auxiliar para verificar si una máscara coincide con la dirección
bool Channel::matchMask(const std::string& mask, const std::string& address) const{
		// Comprobación de errores: si la máscara o la dirección están vacías
		if (mask.empty() || address.empty()) {
				print_debug("Error: mask or address is empty", colors::red, colors::bold);
				return false;
		}

		int m = mask.length();
		int n = address.length();

		// Creamos una matriz para almacenar los resultados de subproblemas
		std::vector<std::vector<bool> > dp(m + 1, std::vector<bool>(n + 1, false));

		// La máscara vacía coincide solo con la cadena vacía
		dp[0][0] = true;

		// Si la máscara comienza con '*', puede coincidir con una cadena vacía
		for (int i = 1; i <= m; i++) {
				if (mask[i - 1] == '*') {
						dp[i][0] = dp[i - 1][0];
				}
		}

		// Comenzamos a rellenar la matriz para las demás combinaciones
		for (int i = 1; i <= m; i++) {
				for (int j = 1; j <= n; j++) {
						// Si el carácter actual de la máscara es '?', o coincide con el de la dirección
						if (mask[i - 1] == '?' || mask[i - 1] == address[j - 1]) {
								dp[i][j] = dp[i - 1][j - 1];
						}
						// Si el carácter actual de la máscara es '*', puede coincidir con cualquier secuencia de caracteres
						else if (mask[i - 1] == '*') {
								dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
						}
				}
		}

		return dp[m][n];
}

bool Channel::matchMaskList(const std::vector<std::string>& maskList, const std::string& address) const {
		//for in a c++98 manner
		for (std::vector<std::string>::const_iterator it = maskList.begin(); it != maskList.end(); ++it) {
				if (matchMask(*it, address)) {
						return true;
				}
		}
		return false;
}