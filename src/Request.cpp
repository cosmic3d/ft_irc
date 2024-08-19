#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class Request {
public:
    std::string prefix;        // El origen del mensaje, si existe
    std::string command;       // El comando o código numérico de 3 dígitos
    std::vector<std::string> params; // Los parámetros del comando

    // Constructor por defecto
    Request();

    // Constructor con inicialización
    Request(const std::string& pref, const std::string& cmd, const std::vector<std::string>& prms);
    
    // Método para imprimir la solicitud (útil para depuración)
    // void print() const; {
    //     std::cout << "Prefix: " << prefix << std::endl;
    //     std::cout << "Command: " << command << std::endl;
    //     std::cout << "Params: ";
    //     for (size_t i = 0; i < params.size(); ++i) {
    //         std::cout << params[i] << (i < params.size() - 1 ? ", " : "");
    //     }
    //     std::cout << std::endl;
    // }
};
