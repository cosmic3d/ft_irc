/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:18:10 by jenavarr          #+#    #+#             */
/*   Updated: 2024/08/21 13:18:10 by jenavarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "utils.hpp"

Request parse_request(const std::string& buffer) {
    std::string prefix;
    std::string command;
    std::vector<std::string> params;

    if (buffer.empty()) {
        print_debug("Empty buffer", colors::red, colors::on_bright_white);
        return Request();
    }

    std::istringstream stream(buffer);
    std::string token;

    // Check if there is a prefix
    if (buffer[0] == ':') {
        std::getline(stream, prefix, ' ');
        if (prefix.length() > 1) {
            prefix = prefix.substr(1);  // Remove the leading ':'
        } else {
            print_debug("Empty prefix", colors::on_yellow, colors::bold);
            return Request();
        }
    }

    // Get the command
    if (!std::getline(stream, command, ' ') || command.empty()) {
        print_debug("Empty command", colors::on_yellow, colors::bold);
        return Request();
    }

    // Get the parameters
    while (std::getline(stream, token, ' ')) {
        if (!token.empty() && token[0] == ':') {
            // Add the rest of the stream as a single parameter
            std::string trailing_param;
            std::getline(stream, trailing_param);
            trailing_param = token.substr(1) + (trailing_param.empty() ? "" : " " + trailing_param);
            params.push_back(trailing_param.substr(0, trailing_param.length() - 2));  // Remove the trailing \r\n
            break;
        } else {
            params.push_back(token);
        }
    }

    return Request(prefix, command, params);
}

//Esto no va aquí pero me la suda
std::string Server::_execute_command(const Request& req, int client_fd) {
    if (!req.valid) {
        return "";
    } else if (req.command == "PASS") {
        // Manejar la autenticación con contraseña
        std::cout << "Handling PASS" << std::endl;
        return _handlePass(req, client_fd);
    } if (req.command == "NICK") {
        // Manejar el comando NICK
        std::cout << "Handling NICK" << std::endl;
        return _handleNick(req, client_fd);
    } else if (req.command == "USER") {
        // Manejar el comando USER
        std::cout << "Handling USER" << std::endl;
        std::string response = _handleUser(req, client_fd);
        bool checkRegistration = _clients[client_fd]->checkRegistered();
        return checkRegistration ? response : "";
    }
        // SI NO ESTÁ AUTENTICADO NO PODRÁ EJECUTAR LOS SIGUIENTES COMANDOS
    if (_clients[client_fd]->isRegistered() == false) {
        //pass a vector list of parameters to the format_message function {"You have not registered"} DOES NOT WORK
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("You have not registered. Please use the PASS command to authenticate.");
        return format_message(_name, ERR_PASSWDMISMATCH, params);
    }

    if (req.command == "PING") {
        // Ejemplo de cómo manejar un comando PING
        std::cout << "Responding to PING" << std::endl;
        return ""; // TO_DO
        // Aquí se respondería con PONG al cliente
    } else if (req.command == "OPER") {
        return (_handleOperator(req, client_fd)); // TO-DO
    } else if (req.command == "JOIN") {
        // Manejar el comando JOIN
        std::cout << "Handling JOIN" << std::endl;
        return ""; // TO_DO
        // TO-DO: Implementar el manejo del comando JOIN
    } else if (req.command == "PART") {
        // Manejar el comando PART
        std::cout << "Handling PART" << std::endl;
        return ""; // TO_DO
        // TO-DO: Implementar el manejo del comando PART
    } else if (req.command == "PRIVMSG") {
        // Manejar el comando PRIVMSG
        std::cout << "Handling PRIVMSG" << std::endl;
        return ""; // TO_DO
        // TO-DO: Implementar el manejo del comando PRIVMSG
    } else if (req.command == "QUIT") {
        // Manejar el comando QUIT
        std::cout << "Handling QUIT" << std::endl;
        return ""; // TO_DO
        // TO-DO: Implementar el manejo del comando QUIT
    } else if (req.command == "WHOIS") {
        // Manejar el comando WHOIS
        std::cout << "Handling WHOIS" << std::endl;
        return ""; // TO_DO
        // Responder con la información del usuario solicitado
    } else {
        // Responder con un error al cliente
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("Unknown command " + req.command);
        return format_message(_name, ERR_UNKNOWNCOMMAND, params);
    }
    return "";
}
//Format message to send to the client in the RFC 2812 format
std::string Server::format_message(const std::string& prefix, const std::string& command, const std::vector<std::string>& params) {
    std::string message = prefix.empty() ? "" : ":" + prefix + " ";
    message += command;
    for (size_t i = 0; i < params.size(); ++i) {
        message += (i == params.size() - 1 ? " :" : " ") + params[i];
    }
    message += "\r\n";
    return message;
}