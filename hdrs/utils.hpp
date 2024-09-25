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
int stoi(const std::string &str);
std::vector<std::string> ft_split(std::string &str, std::string delim);
bool isAlphaNumeric(const std::string &str);
bool isNumeric(const std::string &str);

#endif