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
// std::vector<std::string> list_to_vector(std::initializer_list<std::string> list){
// 		return std::vector<std::string>(list);
// }