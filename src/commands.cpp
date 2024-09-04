#include "Server.hpp"

std::string Server::handlePass(const Request& req, int client_fd) {
    if (req.params.size() < 1) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("Not enough parameters");
        return format_message(_name, ERR_NEEDMOREPARAMS, params);
    }
    if (_clients[client_fd]->isRegistered()) {
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
    _clients[client_fd]->setAuthenticated(true);
    print_debug("Client authenticated: " + itos(client_fd), colors::green, colors::bold);
    return "";
}

std::string Server::handleNick(const Request& req, int client_fd) {
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
    // Verificar si el nickname es válido
    if (req.params[0].length() > 9) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("Erroneous nickname");
        return format_message(_name, ERR_ERRONEUSNICKNAME, params);
    }
    //Guardar el format message del anterior nickname para notificar al cliente el cambio de nickname
    std::string acknoledgement = format_message(_clients[client_fd]->formatPrefix(), "NICK", req.params);
    // Establecer el nickname o cambiarlo
    _clients[client_fd]->setNickname(req.params[0]);
    if (_clients[client_fd]->isRegistered()) {
        return acknoledgement;
    }
    return "";
}

std::string Server::handleUser(const Request& req, int client_fd) {
    // Verificar si el usuario ya está autenticado
    if (_clients[client_fd]->isRegistered()) {
        std::vector<std::string> params;
        params.push_back(_clients[client_fd]->getNickname());
        params.push_back("Already registered");
        return format_message(_name, ERR_ALREADYREGISTRED, params);
    }
    // Verificar si el usuario ha dado todos los parámetros
    if (req.params.size() < 4) {
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
    params.push_back("Welcome to " + _name + " " + _clients[client_fd]->getNickname() + "!" + _clients[client_fd]->getUsername() + "@" + _clients[client_fd]->getHostname());
    return format_message(_name, RPL_WELCOME, params);
}

std::string Server::handleQuit(const Request& req, int client_fd) {
    // Verificar si el usuario ha dado un mensaje de salida
    std::string quit_message = req.params.size() > 0 ? req.params[0] : "Client quit";
    //Enviar mensaje de salida a todos los clientes
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->first != client_fd) {
            std::vector<std::string> params;
            params.push_back(_clients[client_fd]->getNickname());
            params.push_back(quit_message);
            std::string message = format_message(_clients[client_fd]->getNickname(), "QUIT", params);
            send(it->first, message.c_str(), message.length(), 0);
        }
    }
    // Desconectar al cliente
    handleDisconnection(client_fd);
    return "";
}
