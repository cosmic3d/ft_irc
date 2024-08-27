/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 16:20:18 by damendez          #+#    #+#             */
/*   Updated: 2024/08/27 10:16:34 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Method to handle the client's request to join one or more channels.

//void Server::joinChannel(Client& client, const std::string& channelName)
std::string Server::_joinChannel(Request request, int i) {
    
    // IRC Numeric reply codes: https://www.rfc-editor.org/rfc/rfc2812
    // Join command: https://dd.ircdocs.horse/refs/commands/join
    int j = 1;
    
    // Check if client is registered to server
    if (!this->_clients[i]->isAuthenticated())
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
            j = _createPrvChannel(*itChannels, i);
    }
}

int	Server::_createPrvChannel( std::string ChannelName, std::string ChannelKey, int CreatorFd) {
	std::map<std::string, Channel *>::iterator it = this->_allChannels.find(ChannelName);

    // If channel doesnt exist, create it

    // Else join client to already existing channel checking channelkey
}

int	Server::_createChannel( std::string ChannelName, int CreatorFd ) {
	std::map<std::string, Channel *>::iterator it = this->_allChannels.find(ChannelName);

    // If channel doesnt exist, create it

    // Else join client to already existing channel
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