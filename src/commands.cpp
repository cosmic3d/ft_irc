/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:48:20 by damendez          #+#    #+#             */
/*   Updated: 2024/09/06 17:48:20 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Request.hpp"

std::string Server::_handlePass(const Request& req, int client_fd) {
    if (req.params.size() < 1) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("Not enough parameters");
        return format_message(_name, ERR_NEEDMOREPARAMS, params);
    }
    if (_clients[client_fd]->getRegistered()) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("Already registered");
        return format_message(_name, ERR_ALREADYREGISTRED, params);
    }
    if (req.params[0] != _password) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("Password incorrect");
        return format_message(_name, ERR_PASSWDMISMATCH, params);
    }
    _clients[client_fd]->setAuth(true);
    print_debug("Client authenticated: " + itos(client_fd), colors::green, colors::bold);
    // Establecer el hostname y enviar mensaje de bienvenida con formato RPL_WELCOME
    std::vector<std::string> params;
    params.push_back(_clients[client_fd]->getNickname());
    params.push_back("Welcome to " + _name + " " + _clients[client_fd]->mask());
    return format_message(_name, RPL_WELCOME, params);
}

std::string Server::_handleNick(const Request& req, int client_fd) {
    // Verificar si se ha dado un nickname
    if (req.params.size() < 1) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("No nickname given");
        return format_message(_name, ERR_NONICKNAMEGIVEN, params);
    }
    // Verificar si el nickname ya está en uso
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second->getNickname() == req.params[0] && it->first != client_fd) {
            std::vector<std::string> params;
            params.push_back(_clients[client_fd]->getNickname());
            params.push_back("Nickname is already in use");
            return format_message(_name, ERR_NICKNAMEINUSE, params);
        }
    }
    // Verificar si el nickname es válido (que tampoco contenga ! ni @)
    if (req.params[0].length() > 9 || !isAlphaNumeric(req.params[0]) || req.params[0].find("!") != std::string::npos || req.params[0].find("@") != std::string::npos || req.params[0].find("*") != std::string::npos || req.params[0].find("?") != std::string::npos) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("Erroneous nickname");
        return format_message(_name, ERR_ERRONEUSNICKNAME, params);
    }
    //Guardar el format message del anterior nickname para notificar al cliente el cambio de nickname
    std::vector<std::string> params;
    params.push_back(req.params[0]);
    std::string acknoledgement = format_message(_clients[client_fd]->mask(), "NICK", params);
    // Establecer el nickname o cambiarlo
    _clients[client_fd]->setNickname(req.params[0]);
    if (_clients[client_fd]->getRegistered()) {
        return acknoledgement;
    }
    // Establecer el hostname y enviar mensaje de bienvenida con formato RPL_WELCOME
    params.clear();
    params.push_back(_clients[client_fd]->getNickname());
    params.push_back("Welcome to " + _name + " " + _clients[client_fd]->mask());
    return format_message(_name, RPL_WELCOME, params);
}

std::string Server::_handleUser(const Request& req, int client_fd) {
    // Verificar si el usuario ya está autenticado
    if (_clients[client_fd]->getRegistered()) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("Already registered");
        return format_message(_name, ERR_ALREADYREGISTRED, params);
    }
    // Verificar si el usuario ha dado todos los parámetros
    if (req.params.size() < 4  || !isAlphaNumeric(req.params[0]) || req.params[0].find("!") != std::string::npos || req.params[0].find("@") != std::string::npos || req.params[0].find("*") != std::string::npos || req.params[0].find("?") != std::string::npos) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("Not enough parameters");
        return format_message(_name, ERR_NEEDMOREPARAMS, params);
    }
    // Establecer el username
    _clients[client_fd]->setUsername(req.params[0]); //RETRIBUIR AQUÍ TAMBIÉN EL MODE
    // Establecer el hostname y enviar mensaje de bienvenida con formato RPL_WELCOME
    std::vector<std::string> params;
    params.push_back(_clients[client_fd]->getNickname());
    params.push_back("Welcome to " + _name + " " + _clients[client_fd]->mask());
    return format_message(_name, RPL_WELCOME, params);
}

std::string Server::_handleQuit(const Request& req, int client_fd) {
    //Enviar al usuario un reconocimiento de que se ha desconectado
    std::string server_response = format_message(_clients[client_fd]->mask(), "QUIT", req.params);
    send(client_fd, server_response.c_str(), server_response.length(), 0);
    // Desconectar al cliente
    _handleDisconnection(client_fd);
    return "";
}

std::string Server::_handleMode(const Request& req, int client_fd)
{
    if (req.params.size() < 2) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("Not enough parameters");
        return format_message(_name, ERR_NEEDMOREPARAMS, params);
    }

    //Encuentra el canal por nombre
    Channel *channel = _channels.find(req.params[0])->second; //Esto me da miedo
    if (!channel) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("No such channel");
        return format_message(_name, ERR_NOSUCHCHANNEL, params);
    }
    if (channel->isOperator(client_fd) == false) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("You're not channel operator");
        return format_message(_name, ERR_CHANOPRIVSNEEDED, params);
    }
    if (req.params[1].length() < 1 || req.params[1].length() > 4) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("Unknown MODE flag");
        return format_message(_name, ERR_UNKNOWNMODE, params);
    }
    char action = req.params[1][0];
    size_t i_size;
    if (action == '+' || action == '-')
        i_size = 1;
    else
        i_size = 0;

    for (size_t i = i_size; i < req.params[1].length(); i++) {
        if (req.params[1][i] == 'o')
        {
            if (req.params.size() > 2 && !req.params[2].empty())
            {
                Client *client = this->getClientByName(req.params[2]);
                if (action == '+')
                {
                    channel->addOperator(client);
                    channel->removeMember(client->getClientfd());
                    std::vector<std::string> params;
                    params.push_back(req.params[2]);
                    params.push_back("+o");
                    return format_message(_name, "MODE", params);
                }
                else if (action == '-')
                {
                    channel->removeOperator(client->getClientfd());
                    channel->addMember(client);
                    std::vector<std::string> params;
                    params.push_back(req.params[2]);
                    params.push_back("-o");
                    return format_message(_name, "MODE", params);
                }
            }
            else
            {
                std::vector<std::string> params;
                params.push_back(_clients[client_fd]->getNickname());
                params.push_back("Not enough parameters");
                return format_message(_name, ERR_NEEDMOREPARAMS, params);
            }
        }
        // else if (req.params[1][i] == 'k')
        // {
        //     if (req.params.length() > 2 && !req.params[2].empty())
        //     {
        //         if (action == '+')
        //         channel->_key = req.params[2];
        //         else if (action == '-')
        //             channel->_key = "";
        //     }
        //     else
        //     {
        //         //Send the key as a response to the member
        //         std::vector<std::string> params;
        //         params.push_back(_clients[client_fd]->getNickname());
        //         params.push_back(channel->_key);
        //         return format_message(_name, "MODE", params);
        //     }
        // }
        // else if (req.params[1][i] == 'l' && req.params.length() > 2 && !req.params[2].empty())
        // {
        //     if (action == '+')
        //         channel->_userLimit = std::stoi(req.params[2]);
        //     else if (action == '-')
        //         channel->_userLimit = 0;
        // }
        // else if (req.params[1][i] == 'b' && req.params.length() > 2 && !req.params[2].empty())
        // {
        //     if (action == '+')
        //         channel->banUser(this->_clients[req.params[2]]);
        //     else if (action == '-')
        //         channel->removeBanned(req.params[2]);
        // }
        else if (req.params[1][i] == 'i')
        {
            if (action == '+')
                channel->setInviteOnly(true);
            else if (action == '-')
                channel->setInviteOnly(false);
        }
        else
        {
            std::vector<std::string> params;
            params.push_back(_clients[client_fd]->getNickname());
            params.push_back("Unknown MODE flag");
            return format_message(_name, ERR_UNKNOWNMODE, params);
        }
    }
    return "";
}