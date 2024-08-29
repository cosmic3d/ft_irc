/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:31:10 by damendez          #+#    #+#             */
/*   Updated: 2024/08/27 15:42:37 by jenavarr         ###   ########.fr       */
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
    bool        _authenticated; //Password correct
    bool        _registered; //Nick, user, pass correct

public:
    Client(int socket);
    ~Client();
    
    int getSocket() const;
    void setNickname(const std::string &nickname);
    void setUsername(const std::string &username);
    std::string getNickname() const;
    std::string getUsername() const;
    bool    isAuthenticated() const;
    void    setAuthenticated(bool status);
    bool    isRegistered() const;
    void    setRegistered(bool status);
    bool    checkRegistered();
};

#endif