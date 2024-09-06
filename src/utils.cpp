/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 21:25:47 by jenavarr          #+#    #+#             */
/*   Updated: 2024/08/23 21:25:47 by jenavarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void print_debug(const std::string &message, std::ostream& (*color)(std::ostream&), std::ostream& (*style)(std::ostream&)) {
		style(std::cout);
    color(std::cout) << message << colors::reset << std::endl;
}

std::string itos(int n) {
		std::ostringstream stream;
		stream << n;
		return stream.str();
}

std::vector<std::string> ft_split(std::string &str, std::string delim) {
		std::vector<std::string> result;
		size_t pos = 0;
    std::string token;
    while ((pos = str.find(delim)) != std::string::npos) {
        token = str.substr(0, pos);
        result.push_back(token);
        str.erase(0, pos + delim.length());
    }
    result.push_back(str);
    return result;
}

bool isAlphaNumeric(const std::string &str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (!std::isalnum(str[i])) {
            return false;
        }
    }
    return true;
}