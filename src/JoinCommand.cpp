/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 16:20:18 by damendez          #+#    #+#             */
/*   Updated: 2024/08/29 14:45:23 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Method to handle the client's request to join one or more channels.
std::string Server::_joinChannel(Request request, int i) {
    
    // IRC Numeric reply codes: https://www.rfc-editor.org/rfc/rfc2812
    // Join command: https://dd.ircdocs.horse/refs/commands/join
    int j = 1;
    
    // Check if client is registered to server
    if (!this->_clients[i]->isRegistered()) // TO-DO
        return (_printMessage("451", this->_clients[i]->getNickname(), ":You have not registered"));
        
    // Check that there are arguments provided (channel names)
    if (request.args.size() == 0) // TO-DO
        return (_printMessage("461", this->_clients[i]->getNickname(), ":Not enough parameters"));

    if (request.args[0] == "0")
        return (this->_clients[i]->leaveAllChannels()); // TO-DO

    // Seperate channel names by comas
    std::vector<std::string> parsChannels(_commaSeparator(request.args[0]));
    
    // If there are channel keys seperate by comas
    std::vector<std::string> parsKeys;
    if (request.args.size() == 2)
        parsKeys = _commaSeparator(request.args[1]);

    std::vector<std::string>::iterator itChannels = parsChannels.begin();
    std::vector<std::string>::iterator itKeys = parsKeys.begin();
    
    // Iterate through list of channels to be joined.
    while (itChannels != parsChannels.end() && j == 1) {
        if (itKeys != parsKeys.end())
            j = _createPrvChannel(*itChannels, *itKeys, i);
        else
            j = _createChannel(*itChannels, i);
    }
}

int	Server::_createPrvChannel( std::string ChannelName, std::string ChannelKey, int CreatorFd) {
	std::map<std::string, Channel *>::iterator it = this->_allChannels.find(ChannelName);

    // If channel doesnt exist, create it

    // Else join client to already existing channel checking channelkey
}

int	Server::_createChannel( std::string ChannelName, int CreatorFd ) {
	std::map<std::string, Channel *>::iterator it = this->_channels.find(ChannelName);

    // If channel doesnt exist, create it
	if (it == this->_channels.end())
	{
		if (ChannelName[0] != '&' && ChannelName[0] != '#' && ChannelName[0] != '+' && ChannelName[0] != '!')
			return (BADCHANMASK);
		Channel *channel = new Channel(ChannelName, this->_clients[CreatorFd]);
		this->_allChannels.insert(std::pair<std::string, Channel *>(ChannelName, channel));
		this->_clients[CreatorFd]->joinChannel( ChannelName, channel );
	}    
    // join client to already existing channel
	else
	{
		if (it->second->getKey().empty())
		{
			int i = 0;
			if (this->_clients[CreatorFd]->getisOperator() == true)
				i = it->second->addOperator(this->_clients[CreatorFd]);
			else
				i = it->second->addMember(this->_clients[CreatorFd]);
			if (i == USERISJOINED)
				this->_clients[CreatorFd]->joinChannel( it->first, it->second );
			else if (i == USERALREADYJOINED)
				return (USERALREADYJOINED);
			else if (i == BANNEDFROMCHAN)
				return (BANNEDFROMCHAN);
			_sendall(CreatorFd, this->_clients[CreatorFd]->getUserPerfix() + "JOIN " + ChannelName + "\n");
			_sendall(CreatorFd, _printMessage("332", this->_clients[CreatorFd]->getNickName(), ChannelName + " :" + it->second->getTopic()));
			_sendall(CreatorFd, _printMessage("353", this->_clients[CreatorFd]->getNickName() + " = " + ChannelName, it->second->listAllUsers()));
			_sendall(CreatorFd, _printMessage("353", this->_clients[CreatorFd]->getNickName() + " " + ChannelName, ":End of NAMES list"));
			std::string reply = "JOIN " + ChannelName + "\n";
			_sendToAllUsers(it->second, CreatorFd, reply);
			return (USERISJOINED);
		}
	}
	return (USERISJOINED);
}

std::vector<std::string> Server::_commaSeparator(std::string arg) {
    std::vector<std::string> ret;
    size_t  pos = 0;
    while ((pos = arg.find(",")) != std::string::npos) {
        ret.push_back(arg.substr(0, pos));
        arg.erase(0, pos + 1);
    }
    ret.push_back(arg.substr(0, pos));
    return (ret);
}