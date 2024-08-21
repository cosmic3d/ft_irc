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

Request parse_request(const std::string& buffer) {
    std::string prefix;
    std::string command;
    std::vector<std::string> params;

    if (buffer.empty()) {
        std::cerr << "Empty buffer" << std::endl;
    }

    std::istringstream stream(buffer);
    std::string token;

    // Check if there is a prefix
    if (buffer[0] == ':') {
        std::getline(stream, prefix, ' ');
        if (prefix.length() > 1) {
            prefix = prefix.substr(1);  // Remove the leading ':'
        } else {
            std::cerr << "Invalid prefix found in the buffer" << std::endl;
        }
    }

    // Get the command
    if (!std::getline(stream, command, ' ') || command.empty()) {
        std::cerr << "No command found in the buffer" << std::endl;
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
std::string Server::execute_command(const Request& req, int client_fd) {
    if (req.command == "PASS") {
        // Manejar la autenticación con contraseña
        std::cout << "Handling PASS" << std::endl;
        return handlePass(req, client_fd);
    } else if (req.command == "CAP") {
        // Manejar el comando CAP
        std::cout << "Handling CAP" << std::endl;
        return handleCapabilites(req, client_fd);
    } else if (req.command == "NICK") {
        // Manejar el comando NICK
        std::cout << "Handling NICK" << std::endl;
        return handleNick(req, client_fd);
    } else if (req.command == "USER") {
        // Manejar el comando USER
        std::cout << "Handling USER" << std::endl;
        return handleUser(req, client_fd);
    }
    
        // SI NO ESTÁ AUTENTICADO NO PODRÁ EJECUTAR LOS SIGUIENTES COMANDOS
    if (_clients[client_fd]->isAuthenticated() == false) {
        //pass a vector list of parameters to the format_message function {"You have not registered"} DOES NOT WORK
        std::vector<std::string> params;
        params.push_back("You have not registered");
        return format_message(_name, "451", params);
    }
    
    if (req.command == "PING") {
        // Ejemplo de cómo manejar un comando PING
        std::cout << "Responding to PING" << std::endl;
        // Aquí se respondería con PONG al cliente
    } else if (req.command == "JOIN") {
        // Manejar el comando JOIN
        std::cout << "Handling JOIN" << std::endl;
        // TO-DO: Implementar el manejo del comando JOIN
    } else if (req.command == "PART") {
        // Manejar el comando PART
        std::cout << "Handling PART" << std::endl;
        // TO-DO: Implementar el manejo del comando PART
    } else if (req.command == "PRIVMSG") {
        // Manejar el comando PRIVMSG
        std::cout << "Handling PRIVMSG" << std::endl;
        // TO-DO: Implementar el manejo del comando PRIVMSG
    } else if (req.command == "QUIT") {
        // Manejar el comando QUIT
        std::cout << "Handling QUIT" << std::endl;
        // TO-DO: Implementar el manejo del comando QUIT
    } else if (req.command == "WHOIS") {
        // Manejar el comando WHOIS
        std::cout << "Handling WHOIS" << std::endl;
        // Responder con la información del usuario solicitado
    } else {
        std::cout << "Unknown command: " << req.command << std::endl;
        // Responder con un error al cliente
    }
}
//Format message to send to the client in the RFC 2812 format
std::string Server::format_message(const std::string& prefix, const std::string& command, const std::vector<std::string>& params) {
    std::string message = prefix.empty() ? "" : ":" + prefix + " ";
    message += command;
    for (size_t i = 0; i < params.size(); ++i) {
        message += " " + params[i];
    }
    message += "\r\n";
    return message;
}