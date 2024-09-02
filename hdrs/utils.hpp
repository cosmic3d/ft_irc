/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 21:26:33 by jenavarr          #+#    #+#             */
/*   Updated: 2024/08/23 21:26:33 by jenavarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "Server.hpp"
#include "colors.hpp"

// void    send_message(int client_fd, const std::string &message); ESTA FUNCIÓN QUIZÁ SE HAGA AQUÍ EN EL FUTURO

void print_debug(const std::string &message, std::ostream& (*color)(std::ostream&), std::ostream& (*style)(std::ostream&));
std::string itos(int n);
std::vector<std::string> ft_split(std::string &str, std::string delim);
//a function which returns a vector of strings and receives a list like {"a", "b", "c"}
// std::vector<std::string> list_to_vector(std::initializer_list<std::string> list);
//how to use
//format_message("prefix", "command", list_to_vector({"a", "b", "c"})); NO FUNCIONA PORQUE EN C++98 NO SE PUEDE INICIALIZAR UN VECTOR CON UNA LISTA

#endif