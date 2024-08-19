#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include "../hdrs/Request.hpp"
#include "parsing.cpp"

const std::vector<std::string> supported_capabilities = 
    std::vector<std::string>(1, "account-notify");

void handle_client(int client_fd, const std::string& password) {
    
    // Build the list of supported capabilities. EN EL FUTURO GUARDAR ESTA LISTA EN UNA VARIABLE EN EL SERVER
    std::string capabilities;
    for (std::vector<std::string>::const_iterator it = supported_capabilities.begin(); it != supported_capabilities.end(); ++it) {
        if (!capabilities.empty()) {
            capabilities += " ";
        }
        capabilities += *it;
    }
    
    std::string cap_ls_response = ":server.capabilities CAP * LS :" + capabilities + "\r\n";
    send(client_fd, cap_ls_response.c_str(), cap_ls_response.length(), 0);
    // En algún punto de la función de manejo de cliente:
    char buffer[512];
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received > 0) {
        buffer[bytes_received] = '\0'; // Asegurarse de que la cadena esté terminada
        Request req = parse_request(buffer);
        req.print(); // Para depuración, imprimir la solicitud recibida
        execute_command(req);
    } else {
        // Manejo de error o desconexión
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    int port;
    std::stringstream(argv[1]) >> port;
    std::string password = argv[2];

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
        sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);

        if (client_fd < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        std::cout << "Client connected" << std::endl;

        handle_client(client_fd, password);
    }

    close(server_fd);
    return 0;
}
