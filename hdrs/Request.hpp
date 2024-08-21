#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

#ifndef REQUEST_HPP
#define REQUEST_HPP
class Request {
public:
    std::string prefix;        // El origen del mensaje, si existe
    std::string command;       // El comando o código numérico de 3 dígitos
    std::vector<std::string> params; // Los parámetros del comando

    // Constructor por defecto
    Request();

    // Constructor con inicialización
    Request(const std::string& pref, const std::string& cmd, const std::vector<std::string>& prms);
    
    // Método para imprimir la solicitud (útil para depuración) ELIMINAR LUEGO PORQUE NO ESTÁ PERMITIDO DEFINIR FUNCIONES EN CLASES
    void print() const;
};
Request parse_request(const std::string& buffer);


#endif