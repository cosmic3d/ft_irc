/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 16:20:18 by damendez          #+#    #+#             */
/*   Updated: 2024/09/17 14:18:59 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Method to handle the client's request to join one or more channels.
std::string Server::_joinChannel(Request request, int client_fd) {
		
		// IRC Numeric reply codes: https://www.rfc-editor.org/rfc/rfc2812
		// Join command: https://dd.ircdocs.horse/refs/commands/join
		int j = USERISJOINED;
				
		// Check that there are arguments provided (channel names)
		if (request.params.size() <= 0) {
		std::vector<std::string> params;
			params.push_back(this->_clients[client_fd]->getNickname());
			params.push_back(":Not enough parameters");
			return format_message(_name, ERR_NEEDMOREPARAMS, params);
	}
		if (request.params[0] == "0")
				return (this->_clients[client_fd]->leaveAllChannels());

		// Seperate channel names by comas
		std::vector<std::string> parsChannels(ft_split(request.params[0], ","));
		
		// If there are channel keys seperate by comas
		std::vector<std::string> parsKeys;
		if (request.params.size() >= 2)
				parsKeys = ft_split(request.params[1], ",");

		std::vector<std::string>::iterator itChannels = parsChannels.begin();
		std::vector<std::string>::iterator itKeys = parsKeys.begin();
		
		// Iterate through list of channels to be joined.
		while (itChannels != parsChannels.end() && j == USERISJOINED) {
				if (itKeys != parsKeys.end())
					j = _createChannel(*itChannels, client_fd, *itKeys);
				else
					j = _createChannel(*itChannels, client_fd, "");

				if (j == BADCHANMASK) {
						std::vector<std::string> params;
						params.push_back(this->_clients[client_fd]->getNickname());
						params.push_back(":Bad Channel Mask");
						return format_message(_name, ERR_BADCHANNELMASK, params);
				}
				if (j == BANNEDFROMCHAN) {
					std::vector<std::string> params;
						params.push_back(this->_clients[client_fd]->getNickname());
						params.push_back(" :Cannot join channel (+b)");
						return format_message(_name, ERR_BANNEDFROMCHAN, params);
				}
				if (j == TOOMANYCHANNELS ) {
					std::vector<std::string> params;
						params.push_back(this->_clients[client_fd]->getNickname());
						params.push_back(" :You have joined too many channels");
						return format_message(_name, ERR_TOOMANYCHANNELS, params);		
				}
				if (j == BADCHANNELKEY ) {
					std::vector<std::string> params;
						params.push_back(this->_clients[client_fd]->getNickname());
						params.push_back(" :Cannot join channel (+k)");
					return format_message(_name, ERR_BADCHANNELKEY, params);
				}
				if (j == CHANNELISFULL ) {
					std::vector<std::string> params;
						params.push_back(this->_clients[client_fd]->getNickname());
						params.push_back(" :Cannot join channel (+l)");
					return format_message(_name, ERR_CHANNELISFULL, params);
				}
				if (j == NOSUCHCHANNEL) {
					std::vector<std::string> params;
						params.push_back(this->_clients[client_fd]->getNickname());
						params.push_back(" :No such channel");
					return format_message(_name, ERR_NOSUCHCHANNEL, params);	
				}
				if (j == NOTINVITED) {
					std::vector<std::string> params;
						params.push_back(this->_clients[client_fd]->getNickname());
						params.push_back(" :Cannot join channel (+i)");
					return format_message(_name, ERR_INVITEONLYCHAN, params);
				}
						if (itKeys != parsKeys.end())
								itKeys++;
						itChannels++;
				}
				// --itChannels; ?
				return ("");
}

int	Server::_createChannel( std::string ChannelName, int CreatorFd, std::string ChannelKey) {
		// Check if channel already exists
	std::map<std::string, Channel *>::iterator it = this->_channels.find(ChannelName);

		// If channel doesnt exist, create it
	if (it == this->_channels.end())
	{
		print_debug("Creating channel", colors::blue, colors::on_bright_magenta);
		if (ChannelName[0] != '&' && ChannelName[0] != '#' && ChannelName[0] != '+' && ChannelName[0] != '!')
			return (BADCHANMASK);
		Channel *channel = new Channel(ChannelName, ChannelKey, this->_clients[CreatorFd]); // Llamamos al constructor de Channel con key
		this->_channels.insert(std::pair<std::string, Channel *>(ChannelName, channel));
		this->_clients[CreatorFd]->joinChannel( ChannelName, channel );
		std::vector<std::string> params;
		params.push_back(ChannelName);
		std::string acknoledgement = format_message(_clients[CreatorFd]->mask(), "JOIN", params);
		_sendToAllUsers(channel, CreatorFd, acknoledgement);
		_sendmsg(CreatorFd, acknoledgement);
	}    
		// join client to already existing channel
	else
	{
		if (it->second->getKey().empty() || it->second->getKey() == ChannelKey)
		{
			int i = 0;
			i = it->second->addMember(this->_clients[CreatorFd]); 
			if (i == USERISJOINED)
				this->_clients[CreatorFd]->joinChannel( it->first, it->second ); 
			else if (i == USERALREADYJOINED)
				return (USERALREADYJOINED);
			else if (i == BANNEDFROMCHAN)
				return (BANNEDFROMCHAN);
			else if (i == NOTINVITED)
				return (NOTINVITED);
			else if (i == CHANNELISFULL)
				return (CHANNELISFULL);
			std::vector<std::string> params;
			params.push_back(ChannelName);
			_sendToAllUsers(it->second, CreatorFd, format_message(_clients[CreatorFd]->mask(), "JOIN", params));
			_sendmsg(CreatorFd, format_message(_clients[CreatorFd]->mask(), "JOIN", params));

			params.clear();
			params.push_back(this->_clients[CreatorFd]->getNickname());
			params.push_back(ChannelName);
			params.push_back(it->second->getTopic());
			_sendmsg(CreatorFd, format_message(_name, RPL_TOPIC, params));
			
			params.clear();
			params.push_back(this->_clients[CreatorFd]->getNickname());
				params.push_back(ChannelName);
			params.push_back(it->second->listAllUsers());
			_sendmsg(CreatorFd, format_message(_name, RPL_NAMREPLY, params));

			params.clear();
			params.push_back(this->_clients[CreatorFd]->getNickname());
				params.push_back(ChannelName);
			params.push_back("End of NAMES list");
			_sendmsg(CreatorFd, format_message(_name, RPL_ENDOFNAMES, params));
			return (USERISJOINED);
		}
		else
		{
			return (BADCHANNELKEY);
		}
	}
	return (USERISJOINED);
}