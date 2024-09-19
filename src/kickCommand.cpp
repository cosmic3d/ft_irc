/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 20:10:09 by damendez          #+#    #+#             */
/*   Updated: 2024/09/19 20:33:31 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string		Server::_kickedFromChannel(std::string ChannelName, std::string message, std::vector<std::string> users, int client_fd)
{
	std::map<std::string, Channel *>::iterator it = this->_channels.find(ChannelName);
	if (it != this->_channels.end())
	{
		std::pair<Client *, int> user = it->second->findUserRole(i);
		if (user.second == 1)
		{
			std::vector<std::string>::iterator user = users.begin();
			int ret = 0;
			while (user != users.end())
			{
				ret = _findFdByNickName(*user);
				if (ret == USERNOTINCHANNEL)
					return (_printMessage("441", this->_clients[i]->getNickName(), (*user).append(" " + ChannelName + " :They aren't on that channel")));
				std::string reply = "KICK " + ChannelName;
				if (message.empty())
					reply.append("\n");
				else
					reply.append(" " + message + "\n");
				_sendToAllUsers(it->second, i, reply);
				it->second->banUser(this->_clients[ret]);
				ret = _partChannel(ChannelName, ret, "", 0);
				user++;
			}
		}
		else if (user.second == -1  /* Not in channel */)
			return (_printMessage("442", this->_clients[i]->getNickName(), ChannelName + " :You're not on that channel"));
		else
			return (_printMessage("482", this->_clients[i]->getNickName(), ChannelName + " :You're not channel operator"));
		return ("");
	}
	return (_printMessage("403", this->_clients[i]->getNickName(), ChannelName.append(" :No such channel")));
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
            ret = _kickedFromChannel(*it, request.args[2], users, client_fd);
        else
            ret = _kickedFromChannel(*it, "", users, client_fd);
        if (!ret.empty())
            return(ret);
        it++;
    }
    return ("");
}