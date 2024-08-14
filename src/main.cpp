#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <cstring>

#define PORT 8080
#define MAX_CLIENTS 10

int main() {
		int server_fd, new_socket;
		struct sockaddr_in address;
		int addrlen = sizeof(address);

		// Crear el socket del servidor
		server_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (server_fd == 0) {
				std::cerr << "Error al crear el socket" << std::endl;
				return -1;
		}

		// Configurar la dirección y puerto
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(PORT);

		// Enlazar el socket a la dirección y puerto
		if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
				std::cerr << "Error en bind" << std::endl;
				close(server_fd);
				return -1;
		}

		// Escuchar conexiones entrantes
		if (listen(server_fd, 3) < 0) {
				std::cerr << "Error en listen" << std::endl;
				close(server_fd);
				return -1;
		}

		// Array de estructuras pollfd para almacenar los sockets
		struct pollfd fds[MAX_CLIENTS];
		fds[0].fd = server_fd;
		fds[0].events = POLLIN; // Monitorea si hay nuevas conexiones

		// Inicializar el resto de las entradas del array
		for (int i = 1; i < MAX_CLIENTS; i++) {
				fds[i].fd = -1; // -1 indica que no se está usando
		}

		std::cout << "Servidor en escucha en el puerto " << PORT << std::endl;

		while (true) {
				int activity = poll(fds, MAX_CLIENTS, -1); // Espera indefinidamente hasta que haya actividad

				if (activity < 0) {
						std::cerr << "Error en poll" << std::endl;
						break;
				}

				// Comprobar si hay nuevas conexiones
				if (fds[0].revents & POLLIN) {
						new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
						if (new_socket < 0) {
								std::cerr << "Error en accept" << std::endl;
								continue;
						}

						std::cout << "Nueva conexión, socket fd es " << new_socket << std::endl;

						// Añadir nuevo socket al array de poll
						for (int i = 1; i < MAX_CLIENTS; i++) {
								if (fds[i].fd == -1) {
										fds[i].fd = new_socket;
										fds[i].events = POLLIN; // Monitorea si hay datos para leer
										break;
								}
						}
				}

				// Revisar los sockets de clientes para ver si tienen datos para leer
				for (int i = 1; i < MAX_CLIENTS; i++) {
						if (fds[i].fd != -1 && fds[i].revents & POLLIN) {
								char buffer[1024] = {0};
								int valread = read(fds[i].fd, buffer, 1024);
								if (valread == 0) {
										// Cliente ha cerrado la conexión
										std::cout << "Cliente desconectado, socket fd " << fds[i].fd << std::endl;
										close(fds[i].fd);
										fds[i].fd = -1; // Marcar como no usado
								} else {
										std::cout << "Mensaje recibido: " << buffer << std::endl;
										// Responder al cliente
										send(fds[i].fd, "Mensaje recibido\n", 17, 0);
								}
						}
				}
		}

		close(server_fd);
		return 0;
}
