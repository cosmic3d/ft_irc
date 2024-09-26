/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:11:08 by damendez          #+#    #+#             */
/*   Updated: 2024/09/19 19:06:11 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	Server::_partChannel( std::string ChannelName, int client_fd, std::string message, int isPart )
{
	std::map<std::string, Channel *>::iterator itCh= this->_channels.find(ChannelName);
	if (itCh == this->_channels.end() /* No such channel */)
		return (NOSUCHCHANNEL);
	else
	{
        // Check if user is in channel
        int user = this->_clients[client_fd]->isJoined(itCh->first);
        if (user == 0)
            return (NOTINCHANNEL);
		else
		{
			if (itCh->second->isMember(client_fd))
				itCh->second->removeMember(client_fd);
			else if (itCh->second->isOperator(client_fd))
				itCh->second->removeOperator(client_fd);
			this->_clients[client_fd]->leaveChannel(itCh->first);
			if (isPart == 1)
			{
                std::vector<std::string> params;
                if (message.empty())
                    params.push_back(ChannelName);
                else
                    params.push_back(ChannelName + " :" + message);
                std::string reply = format_message(_clients[client_fd]->mask(), "PART ", params);
				_sendToAllUsers(itCh->second, client_fd, reply);
			}
		}
	}
	return (0);
}

std::string	Server::_handlePart( Request request, int client_fd )
{
	if (!this->_clients[client_fd]->getRegistered()) {
        std::vector<std::string> params;
        params.push_back(this->_clients[client_fd]->getNickname());
        params.push_back("You have not registered");
        return format_message(_name, ERR_NOTREGISTERED, params);
    }
	if (request.params.size() <= 0) {
		std::vector<std::string> params;
		params.push_back(this->_clients[client_fd]->getNickname());
		params.push_back("Not enough parameters");
		return format_message(_name, ERR_NEEDMOREPARAMS, params);
	}
	std::vector<std::string>            parsChannels(ft_split(request.params[0], ","));
	std::vector<std::string>::iterator  it = parsChannels.begin();
	while (it != parsChannels.end())
	{
		int j = 0;
		if (request.params.size() == 2)
			j = _partChannel(*it, client_fd, request.params[1], 1);
		else
			j = _partChannel(*it, client_fd, "", 1);
		if (j == NOSUCHCHANNEL) {
            std::vector<std::string> params;
            params.push_back(this->_clients[client_fd]->getNickname());
            params.push_back(*it + " :No such channel");
            return format_message(_name, ERR_NOSUCHCHANNEL, params);
        }
		if (j == NOTINCHANNEL) {
            std::vector<std::string> params;
            params.push_back(this->_clients[client_fd]->getNickname());
            params.push_back(*it + " :You're not on that channel");
            return format_message(_name, ERR_NOTONCHANNEL, params);
        }
		it++;
	}
	return ("");
};