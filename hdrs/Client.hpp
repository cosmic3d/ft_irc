/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:31:10 by damendez          #+#    #+#             */
/*   Updated: 2024/09/06 17:10:50 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "Server.hpp"

class Channel;

class Client 
{
    private:
        int                 _clientfd;
        std::string         _nickname;
        std::string         _username;
    std::string _hostname;
        std::string         _fullname;
        std::string         _ID;
        //const std::string   _host;
        bool                _authenticated; //Password correct
        bool                _registered; //Nick, user, pass correct
    std::string _receiveBuffer;
        bool                _isOperator;
        std::map<std::string, Channel *>    _joinedChannels;

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