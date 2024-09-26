/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 20:10:09 by damendez          #+#    #+#             */
/*   Updated: 2024/09/26 15:34:56 by jenavarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string		Server::_kickedFromChannel(std::string ChannelName, std::string message, std::vector<std::string> users, int client_fd)
{
	std::map<std::string, Channel *>::iterator it = this->_channels.find(ChannelName);
	if (it != this->_channels.end())
	{
		if (it->second->isOperator(client_fd) == true)
		{
			std::vector<std::string>::iterator user = users.begin();
			int ret = 0;
			while (user != users.end())
			{
				ret = _findFdByNickName(*user);
				if (ret == USERNOTINCHANNEL) {
					std::vector<std::string> params;
					params.push_back(this->_clients[client_fd]->getNickname());
					params.push_back(*user);
					params.push_back(ChannelName + " :They aren't on that channel");
					return format_message(_name, ERR_USERNOTINCHANNEL, params);
				}
				std::vector<std::string> params;
				params.push_back(ChannelName);
				params.push_back(*user);
				std::string reply = format_message(_clients[client_fd]->mask(), "KICK", params);
				if (!message.empty())
					reply.append(" " + message);
				_sendToAllUsers(it->second, client_fd, reply);
				_sendmsg(client_fd, reply.c_str());

				it->second->setBanMask(getClientByName(*user)->mask());
				ret = _partChannel(ChannelName, ret, message);
				user++;
			}
		}
		else if (it->second->isOperator(client_fd) == false) {
			std::vector<std::string> params;
            params.push_back(this->_clients[client_fd]->getNickname());
            params.push_back(ChannelName + " :You're not channel operator");
            return format_message(_name, ERR_CHANOPRIVSNEEDED, params);
		}
		else if (it->second->isMember(client_fd) == false) {
			std::vector<std::string> params;
			params.push_back(this->_clients[client_fd]->getNickname());
			params.push_back(ChannelName + " :You're not on that channel");
			return format_message(_name, ERR_NOTONCHANNEL, params);
		}
		return ("");
	}
	std::vector<std::string> params;
	params.push_back(this->_clients[client_fd]->getNickname());
	params.push_back(ChannelName + " :No such channel");
	return format_message(_name, ERR_NOSUCHCHANNEL, params);
};


std::string Server::_handleKick( Request request, int client_fd ) {
    if (request.params.size() < 2) {
        std::vector<std::string> params;
        params.push_back(this->_clients[client_fd]->getNickname());
        params.push_back("Not enough parameters");
        return format_message(_name, ERR_NEEDMOREPARAMS, params);
    }
    std::vector<std::string> channels(ft_split(request.params[0], ","));
	std::vector<std::string> users(ft_split(request.params[1], ","));
    std::vector<std::string>::iterator it = channels.begin();
    while (it != channels.end())
    {
        std::string ret;
        if (request.params.size() == 3)
            ret = _kickedFromChannel(*it, request.params[2], users, client_fd);
        else
            ret = _kickedFromChannel(*it, "", users, client_fd);
        if (!ret.empty())
            return(ret);
        it++;
    }
    return ("");
}