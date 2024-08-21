/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 14:36:28 by jenavarr          #+#    #+#             */
/*   Updated: 2024/08/21 18:02:08 by jenavarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string Server::handleCapabilites(const Request& req, int client_fd) {
	//we handle every CAP subcommand here and return the appropriate response (capababilities)

if (req.params.size() < 1) {
		return format_message(_name, "410", {"CAP", "Not enough parameters"});
	}
	if (req.params[0] == "LS") {
		return format_message(_name, "001", {"CAP", "LS"});
	}
	if (req.params[0] == "REQ") {
		return format_message(_name, "001", {"CAP", "REQ"});
	}
	if (req.params[0] == "END") {
		return format_message(_name, "001", {"CAP", "END"});
	}
	return format_message(_name, "410", {"CAP", "Invalid subcommand"});
}

std::string Server::handlePass(const Request& req, int client_fd) {
	if (req.params.size() < 1) {
		return format_message(_name, "461", {"PASS", "Not enough parameters"});
	}
	if (_clients[client_fd]->isAuthenticated()) {
		return format_message(_name, "462", {"PASS", "Already registered"});
	}
	if (req.params[0] != _password) {
		return format_message(_name, "464", {"PASS", "Password incorrect"});
	}
		_clients[client_fd]->setAuthenticated(true);
    std::cout << "clientSocket: " << client_fd << " has been authenticated" << std::endl;
		return format_message(_name, "001", {"PASS", "Welcome to the IRC server!"});
}

std::string Server::handleNick(const Request& req, int client_fd) {
	//Check if the nickname is given
	if (req.params.size() < 1) {
		return format_message(_name, "431", {"NICK", "No nickname given"});
	}
	//Check if the nickname is already in use
	for (auto it = _clients.begin(); it != _clients.end(); it++) {
		if (it->second->getNickname() == req.params[0]) {
			return format_message(_name, "433", {"NICK", "Nickname is already in use"});
		}
	}
	//Check if the nickname is valid
	if (req.params[0].length() > 9) {
		return format_message(_name, "432", {"NICK", "Erroneous nickname"});
	}
	//Set the nickname or change it
	_clients[client_fd]->setNickname(req.params[0]);
	return format_message(_name, "001", {"NICK", "Nickname set"});
}

std::string Server::handleUser(const Request& req, int client_fd) {
	//Check if the user is already authenticated
	if (_clients[client_fd]->isAuthenticated()) {
		return format_message(_name, "462", {"USER", "Already registered"});
	}
	//Check if the user has given all the parameters
	if (req.params.size() < 4) {
		return format_message(_name, "461", {"USER", "Not enough parameters"});
	}
	//Set the username
	_clients[client_fd]->setUsername(req.params[0]);
	return format_message(_name, "001", {"USER", "Username set"});
}