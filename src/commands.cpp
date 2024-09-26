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
    if (req.params.size() < 1) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("Not enough parameters");
        return format_message(_name, ERR_NEEDMOREPARAMS, params);
    }

    //Encuentra el canal por nombre
    std::map<std::string, Channel *>::iterator channelPair = _channels.find(req.params[0]);
    if (channelPair == _channels.end()) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("No such channel");
        return format_message(_name, ERR_NOSUCHCHANNEL, params);
    }
    Channel *channel = channelPair->second;
    //check if user is in channel
    if (!channel->isMember(client_fd) && !channel->isOperator(client_fd)) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("You're not on that channel");
        return format_message(_name, ERR_USERNOTINCHANNEL, params);
    }
    if (req.params.size() == 1) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back(channel->getName());
        std::string modes = channel->getModes();
        params.push_back(modes.empty() ? "No modes set" : "+" + modes);
        if (!channel->getKey().empty())
            params.push_back(channel->getKey());
        return format_message(_name, RPL_CHANNELMODEIS, params);
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
    size_t arg_count = 2; //index of the first argument. This will be incremented if the mode requires an argument

    for (size_t i = i_size; i < req.params[1].length(); i++) {
        if (req.params[1][i] == 'o')
        {
            if (req.params.size() > arg_count && !req.params[arg_count].empty())
            {
                Client *client = this->getClientByName(req.params[arg_count++]);
                if (action != '-')
                {
                    channel->addOperator(client);
                    channel->removeMember(client->getClientfd());
                    std::vector<std::string> params;
                    params.push_back(channel->getName());
                    params.push_back("+o");
                    params.push_back(client->getNickname());
                    std::string message = format_message(_clients[client_fd]->mask(), "MODE", params);
                    _sendmsg(client_fd, message);
                    _sendToAllUsers(channel, client_fd, message);
                }
                else if (action == '-')
                {
                    channel->removeOperator(client->getClientfd());
                    channel->addMember(client); //PONER FLAG DE SEGUNDO ARGUMENTO PARA AÑADIR SIN COMPROBAR, YA QUE AHORA VOLVERÁ A COMPROBAR SI ESTÁ BANEADO Y ERRONEAMENTE NO LO AÑADIRÁ
                    std::vector<std::string> params;
                    params.push_back(channel->getName());
                    params.push_back("-o");
                    params.push_back(client->getNickname());
                    std::string message = format_message(_clients[client_fd]->mask(), "MODE", params);
                    _sendmsg(client_fd, message);
                    _sendToAllUsers(channel, client_fd, message);
                }
            }
            else
            {
                std::vector<std::string> params;
                params.push_back(_clients[client_fd]->getNickname());
                params.push_back("Not enough parameters");
                std::string message = format_message(_name, ERR_NEEDMOREPARAMS, params);
                _sendmsg(client_fd, message);
            }
        }
        else if (req.params[1][i] == 'b')
        {
            if (req.params.size() > arg_count && !req.params[arg_count].empty())
            {
                if (action != '-')
                {
                    channel->setBanMask(req.params[arg_count]);
                    std::vector<std::string> params;
                    params.push_back(channel->getName());
                    params.push_back("+b");
                    params.push_back(req.params[arg_count++]);
                    std::string message = format_message(_clients[client_fd]->mask(), "MODE", params);
                    _sendmsg(client_fd, message);
                    _sendToAllUsers(channel, client_fd, message);
                }
                else
                {
                    channel->removeBanMask(req.params[arg_count]);
                    std::vector<std::string> params;
                    params.push_back(channel->getName());
                    params.push_back("-b");
                    params.push_back(req.params[arg_count++]);
                    std::string message = format_message(_clients[client_fd]->mask(), "MODE", params);
                    _sendmsg(client_fd, message);
                    _sendToAllUsers(channel, client_fd, message);
                }
            }
            else
            {
                std::vector<std::string> params;
                params.push_back(_clients[client_fd]->getNickname());
                params.push_back("Not enough parameters");
                std::string message = format_message(_name, ERR_NEEDMOREPARAMS, params);
                _sendmsg(client_fd, message);
            }
        }
        else if (req.params[1][i] == 'k')
        {
            if (action != '-')
            {
                if (req.params.size() > arg_count && !req.params[arg_count].empty())
                {
                    channel->setKey(req.params[arg_count]);
                    std::vector<std::string> params;
                    params.push_back(channel->getName());
                    params.push_back("+k");
                    params.push_back(req.params[arg_count++]);
                    std::string message = format_message(_clients[client_fd]->mask(), "MODE", params);
                    _sendmsg(client_fd, message);
                    _sendToAllUsers(channel, client_fd, message);
                }
                else
                {
                    //Send key value as an echo of the MODE command
                    std::vector<std::string> params;
                    params.push_back(channel->getName());
                    params.push_back("+k");
                    params.push_back(channel->getKey());
                    std::string message = format_message(_clients[client_fd]->mask(), "MODE", params);
                    _sendmsg(client_fd, message);
                }
            }
            else
            {
                channel->setKey("");
                std::vector<std::string> params;
                params.push_back(channel->getName());
                params.push_back("-k");
                std::string message = format_message(_clients[client_fd]->mask(), "MODE", params);
                _sendmsg(client_fd, message);
                _sendToAllUsers(channel, client_fd, message);
            }
        }
        else if (req.params[1][i] == 'l')
        {
            if (action != '-')
            {
                if (req.params.size() > arg_count && !req.params[arg_count].empty())
                {
                    //Check before setting the limit if the limit is a valid number
                    if (isNumeric(req.params[arg_count]))
                    {
                        channel->setUserLimit(stoi(req.params[arg_count]));
                        std::cout << "User limit set to: " << channel->getUserLimit() << std::endl;
                        //print online users
                        std::cout << "Online users: " << channel->getOnlineUsers() << std::endl;
                        std::vector<std::string> params;
                        params.push_back(channel->getName());
                        params.push_back("+l");
                        params.push_back(req.params[arg_count++]);
                        std::string message = format_message(_clients[client_fd]->mask(), "MODE", params);
                        _sendmsg(client_fd, message);
                        _sendToAllUsers(channel, client_fd, message);
                    }
                    else
                    {
                        std::vector<std::string> params;
                        params.push_back(_clients[client_fd]->getNickname());
                        params.push_back("Invalid channel limit");
                        std::string message = format_message(_name, ERR_NEEDMOREPARAMS, params);
                        _sendmsg(client_fd, message);
                    }
                }
                else
                {
                    //Send limit value as an echo of the MODE command
                    std::vector<std::string> params;
                    params.push_back(channel->getName());
                    params.push_back("+l");
                    params.push_back(itos(channel->getUserLimit()));
                    std::string message = format_message(_clients[client_fd]->mask(), "MODE", params);
                    _sendmsg(client_fd, message);
                }
            }
            else if (action == '-')
            {
                channel->setUserLimit(0);
                std::vector<std::string> params;
                params.push_back(channel->getName());
                params.push_back("-l");
                std::string message = format_message(_clients[client_fd]->mask(), "MODE", params);
                _sendmsg(client_fd, message);
                _sendToAllUsers(channel, client_fd, message);
            }
        }
        else if (req.params[1][i] == 'i')
        {
            //send confirmstion messages as an echo of the MODE commands
            if (action == '+')
                channel->setInviteOnly(true);
            else if (action == '-')
                channel->setInviteOnly(false);

            std::vector<std::string> params;
            params.push_back(channel->getName());
            //use ternary operator to determine the action message
            params.push_back(action == '+' ? "+i" : "-i");
            std::string message = format_message(_clients[client_fd]->mask(), "MODE", params);
            _sendmsg(client_fd, message);
            _sendToAllUsers(channel, client_fd, message);
        }
        else if (req.params[1][i] == 't')
        {
            //send confirmstion messages as an echo of the MODE commands
            if (action == '+')
                channel->setTopicRestricted(true);
            else if (action == '-')
                channel->setTopicRestricted(false);

            std::vector<std::string> params;
            params.push_back(channel->getName());
            //use ternary operator to determine the action message
            params.push_back(action == '+' ? "+t" : "-t");
            std::string message = format_message(_clients[client_fd]->mask(), "MODE", params);
            _sendmsg(client_fd, message);
            _sendToAllUsers(channel, client_fd, message);
        }
        else
        {
            std::vector<std::string> params;
            params.push_back(_clients[client_fd]->getNickname());
            params.push_back("Unknown MODE flag");
            std::string message = format_message(_name, ERR_UNKNOWNMODE, params);
            _sendmsg(client_fd, message);
        }
    }
    return "";
}

std::string Server::_handleInvite(const Request& req, int client_fd)
{
    if (req.params.size() < 2)
    {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("Not enough parameters");
        return format_message(_name, ERR_NEEDMOREPARAMS, params);
    }
    Client *client = this->getClientByName(req.params[0]);
    if (!client)
    {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("No such nick");
        return format_message(_name, ERR_NOSUCHNICK, params);
    }
    std::map<std::string, Channel *>::iterator channelPair = _channels.find(req.params[1]);
    if (channelPair == _channels.end())
    {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("No such channel");
        return format_message(_name, ERR_NOSUCHCHANNEL, params);
    }
    Channel *channel = channelPair->second;
    //check if user is in channel
    if (!channel->isMember(client_fd) && !channel->isOperator(client_fd)) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("You're not on that channel");
        return format_message(_name, ERR_NOTONCHANNEL, params);
    }
    //check if invited user is already in channel
    if (channel->isMember(client->getClientfd()) || channel->isOperator(client->getClientfd()))
    {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back(client->getNickname());
        params.push_back(channel->getName());
        params.push_back("User is already in channel");
        return format_message(_name, ERR_USERONCHANNEL, params);
    }
    if (channel->isOperator(client_fd) == false)
    {
        if (channel->getInviteOnly())
        {
            std::vector<std::string> params;
            params.push_back(_clients[client_fd]->getNickname());
            params.push_back("You're not channel operator");
            return format_message(_name, ERR_CHANOPRIVSNEEDED, params);
        }
        if (channel->isMember(client_fd) == false)
        {
            std::vector<std::string> params;
            params.push_back(_clients[client_fd]->getNickname());
            params.push_back("You're not on that channel");
            return format_message(_name, ERR_NOTONCHANNEL, params);
        }
    }
    channel->setInviteMask(client->mask());
    std::vector<std::string> params;
    params.push_back(req.params[0]); //nickname
    params.push_back(req.params[1]); //channel
    std::string message = format_message(_clients[client_fd]->mask(), "INVITE", params);
    _sendmsg(client->getClientfd(), message);
    params.clear();
    params.push_back(_clients[client_fd]->getNickname());
    params.push_back(req.params[0]); //nickname
    params.push_back(req.params[1]); //channel
    return format_message(_clients[client_fd]->mask(), RPL_INVITING, params);
}

std::string Server::_handleTopic(const Request& req, int client_fd)
{
    if (req.params.size() < 1)
    {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("Not enough parameters");
        return format_message(_name, ERR_NEEDMOREPARAMS, params);
    }
    std::map<std::string, Channel *>::iterator channelPair = _channels.find(req.params[0]);
    if (channelPair == _channels.end())
    {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("No such channel");
        return format_message(_name, ERR_NOSUCHCHANNEL, params);
    }
    Channel *channel = channelPair->second;
    //check if user is in channel
    if (!channel->isMember(client_fd) && !channel->isOperator(client_fd)) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("You're not on that channel");
        return format_message(_name, ERR_USERNOTINCHANNEL, params);
    }
    if (req.params.size() == 1)
    {
        std::vector<std::string> params;
        params.push_back(channel->getName());
        params.push_back(channel->getTopic());
        if (channel->getTopic().empty())
            return format_message(_name, RPL_NOTOPIC, params);
        return format_message(_name, RPL_TOPIC, params);
    }
    if (req.params.size() == 2)
    {
        if (channel->getTopicRestricted() && channel->isOperator(client_fd) == false)
        {
            std::vector<std::string> params;
            params.push_back(_clients[client_fd]->getNickname());
            params.push_back("You're not channel operator");
            return format_message(_name, ERR_CHANOPRIVSNEEDED, params);
        }
        channel->setTopic(req.params[1]);
        std::vector<std::string> params;
        params.push_back(channel->getName());
        params.push_back(req.params[1]);
        std::string message = format_message(_clients[client_fd]->mask(), "TOPIC", params);
        _sendToAllUsers(channel, client_fd, message);
        return message;
    }
    return "";
}