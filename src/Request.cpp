// Request.cpp
#include "Request.hpp"

// Constructor por defecto (inicializa los punteros a NULL y la bandera a false)
Request::Request(): valid(false) {}

// Constructor con inicialización de prefijo, comando y parámetros
Request::Request(const std::string& pref, const std::string& cmd, const std::vector<std::string>& prms)
    : prefix(pref), command(cmd), params(prms), valid(true) {}

// Método para imprimir la solicitud
void Request::print() const {
    print_debug("Request:", colors::magenta, colors::on_bright_white);
    print_debug("Prefix: " + prefix, colors::magenta, colors::bold);
    print_debug("Command: " + command, colors::magenta, colors::bold);
    print_debug("Params:", colors::magenta, colors::bold);
    for (size_t i = 0; i < params.size(); ++i) {
        print_debug("  " + params[i], colors::magenta, colors::reset);
    }
    std::cout << std::endl;
}
