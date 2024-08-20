// Request.cpp
#include "Request.hpp"
#include <iostream>

// Constructor por defecto
Request::Request() {}

// Constructor con inicialización de prefijo, comando y parámetros
Request::Request(const std::string& pref, const std::string& cmd, const std::vector<std::string>& prms)
    : prefix(pref), command(cmd), params(prms) {}

// Método para imprimir la solicitud
void Request::print() const {
    std::cout << "Prefix: " << prefix << std::endl;
    std::cout << "Command: " << command << std::endl;
    std::cout << "Params: ";
    for (size_t i = 0; i < params.size(); ++i) {
        std::cout << params[i] << (i < params.size() - 1 ? ", " : "");
    }
    std::cout << std::endl;
}
