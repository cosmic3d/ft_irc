/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 21:23:05 by jenavarr          #+#    #+#             */
/*   Updated: 2024/08/23 21:23:05 by jenavarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Server.hpp"
#include "colors.hpp"

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