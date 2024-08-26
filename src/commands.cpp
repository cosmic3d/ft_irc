#include "Server.hpp"
#include <vector>

std::string Server::handlePass(const Request& req, int client_fd) {
    if (req.params.size() < 1) {
        std::vector<std::string> params;
        params.push_back("PASS");
        params.push_back("Not enough parameters");
        return format_message(_name, ERR_NEEDMOREPARAMS, params);
    }
    if (_clients[client_fd]->isAuthenticated()) {
        std::vector<std::string> params;
        params.push_back("PASS");
        params.push_back("Already registered");
        return format_message(_name, ERR_ALREADYREGISTRED, params);
    }
    if (req.params[0] != _password) {
        std::vector<std::string> params;
        params.push_back("PASS");
        params.push_back("Password incorrect");
        return format_message(_name, ERR_PASSWDMISMATCH, params);
    }
    _clients[client_fd]->setAuthenticated(true);
    print_debug("Client authenticated: " + itos(client_fd), colors::green, colors::bold);
    std::vector<std::string> params;
    params.push_back("PASS");
    params.push_back("Welcome to the IRC server!");
    return format_message(_name, RPL_WELCOME, params);
}

std::string Server::handleNick(const Request& req, int client_fd) {
    // Verificar si se ha dado un nickname
    if (req.params.size() < 1) {
        std::vector<std::string> params;
        params.push_back("NICK");
        params.push_back("No nickname given");
        return format_message(_name, "431", params);
    }
    // Verificar si el nickname ya está en uso
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second->getNickname() == req.params[0]) {
            std::vector<std::string> params;
            params.push_back("NICK");
            params.push_back("Nickname is already in use");
            return format_message(_name, "433", params);
        }
    }
    // Verificar si el nickname es válido
    if (req.params[0].length() > 9) {
        std::vector<std::string> params;
        params.push_back("NICK");
        params.push_back("Erroneous nickname");
        return format_message(_name, "432", params);
    }
    // Establecer el nickname o cambiarlo
    _clients[client_fd]->setNickname(req.params[0]);
    std::vector<std::string> params;
    params.push_back("NICK");
    params.push_back("Nickname set");
    return format_message(_name, "001", params);
}

std::string Server::handleUser(const Request& req, int client_fd) {
    // Verificar si el usuario ya está autenticado
    if (_clients[client_fd]->isAuthenticated()) {
        std::vector<std::string> params;
        params.push_back("USER");
        params.push_back("Already registered");
        return format_message(_name, "462", params);
    }
    // Verificar si el usuario ha dado todos los parámetros
    if (req.params.size() < 4) {
        std::vector<std::string> params;
        params.push_back("USER");
        params.push_back("Not enough parameters");
        return format_message(_name, "461", params);
    }
    // Establecer el nombre de usuario
    _clients[client_fd]->setUsername(req.params[0]);
    std::vector<std::string> params;
    params.push_back("USER");
    params.push_back("Username set");
    return format_message(_name, "001", params);
}
