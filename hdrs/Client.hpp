/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:31:10 by damendez          #+#    #+#             */
/*   Updated: 2024/08/21 08:50:58 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "Server.hpp"

class Client {
private:
    int         _socket;
    std::string _nickname;
    std::string _username;
    bool        _authenticated;

public:
    Client(int socket);
    ~Client();
    
    int getSocket() const;
    void setNickname(const std::string &nickname);
    std::string getNickname() const;
    bool    isAuthenticated() const;
    void    setAuthenticated();
};

#endif