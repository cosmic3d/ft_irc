#include "Request.hpp"

Request parse_request(const std::string& buffer) {
    std::string prefix, command;
    std::vector<std::string> params;

    std::istringstream iss(buffer);
    std::string token;

    std::cout << "Parsing request: " << buffer << std::endl;

    // Primero verificamos si hay un prefijo
    if (buffer[0] == ':') {
        std::getline(iss, token, ' ');
        prefix = token.substr(1); // Eliminar el ':' inicial
    }

    // Luego tomamos el comando
    std::getline(iss, command, ' ');

    // Finalmente, obtenemos todos los parámetros
    while (std::getline(iss, token, ' ')) {
        // Si un parámetro comienza con ':', es el último parámetro (puede contener espacios)
        if (token[0] == ':') {
            //Eliminar CR-LF si es que lo hay
            if (token[token.size() - 1] == '\n') {
                token = token.substr(1, token.size() - 3);
            } else {
                token = token.substr(1);
            }
            params.push_back(token);
            break;
        } else {
            //Eliminar CR-LF si es que lo hay
            if (token[token.size() - 1] == '\n') {
                token = token.substr(0, token.size() - 2);
            }
            params.push_back(token);
        }
    }

    // Crear y devolver el objeto Request
    return Request(prefix, command, params);
}

//Esto no va aquí pero me la suda
void execute_command(const Request& req, int client_fd) {
    if (req.command == "PING") {
        // Ejemplo de cómo manejar un comando PING
        std::cout << "Responding to PING" << std::endl;
        // Aquí se respondería con PONG al cliente
    } else if (req.command == "CAP") {
        // Manejar el comando CAP
        if (!req.params.empty() && req.params[0] == "LS") {
            // Manejar CAP LS
            std::cout << "Handling CAP LS" << std::endl;
            // Responder con la lista de capacidades soportadas
            std::string message = format_message("servimierda", "CAP", std::vector<std::string>(1, "LS account-notify"));
            std::cout << "Sending message: " << message;
            send(client_fd, message.c_str(), message.size(), 0);

        } else if (!req.params.empty() && req.params[0] == "REQ") {
            // Manejar CAP REQ
            std::cout << "Handling CAP REQ" << std::endl;
            // Responder con la lista de capacidades solicitadas
            std::string message = format_message("servimierda", "CAP", std::vector<std::string>(1, "ACK account-notify"));
            std::cout << "Sending message: " << message;
            send(client_fd, message.c_str(), message.size(), 0);
        } else if (!req.params.empty() && req.params[0] == "END") {
            // Manejar CAP END
            std::cout << "Handling CAP END" << std::endl;
            // Responder con CAP ACK si se aceptaron las capacidades
            std::string message = format_message("servimierda", "CAP", std::vector<std::string>(1, "ACK account-notify"));
            std::cout << "Sending message: " << message;
            send(client_fd, message.c_str(), message.size(), 0);
        }
    } else if (req.command == "PASS") {
        // Manejar la autenticación con contraseña
        std::cout << "Handling PASS" << std::endl;
        // Verificar la contraseña y responder en consecuencia
    } else if (req.command == "WHOIS") {
        // Manejar el comando WHOIS
        std::cout << "Handling WHOIS" << std::endl;
        // Responder con la información del usuario solicitado
    } else {
        std::cout << "Unknown command: " << req.command << std::endl;
        // Responder con un error al cliente
    }
}
//Format message to send to the client in the RFC 1459 format.
std::string format_message(const std::string& prefix, const std::string& command, const std::vector<std::string>& params) {
    std::string message;
    if (!prefix.empty()) {
        message += ":" + prefix + " ";
    }
    message += command;
    for (size_t i = 0; i < params.size(); ++i) {
        message += " " + params[i];
        if (i < params.size() - 1) {
            message += " ";
        }
    }
    message += "\r\n";
    return message;
}