#include "../hdrs/Request.hpp"

Request parse_request(const std::string& buffer) {
    std::string prefix, command;
    std::vector<std::string> params;

    std::istringstream iss(buffer);
    std::string token;

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
            params.push_back(token.substr(1)); // Añadir el resto del mensaje como un único parámetro
            break;
        } else {
            params.push_back(token);
        }
    }

    // Crear y devolver el objeto Request
    return Request(prefix, command, params);
}

//Esto no va aquí pero me la suda
void execute_command(const Request& req) {
    if (req.command == "PING") {
        // Ejemplo de cómo manejar un comando PING
        std::cout << "Responding to PING" << std::endl;
        // Aquí se respondería con PONG al cliente
    } else if (req.command == "CAP") {
        // Manejar el comando CAP
        if (!req.params.empty() && req.params[0] == "LS") {
            std::cout << "Handling CAP LS" << std::endl;
            // Aquí se respondería con la lista de capacidades
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

