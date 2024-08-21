/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:52:45 by damendez          #+#    #+#             */
/*   Updated: 2024/08/21 15:02:16 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"

class Channel {
private:
    std::string             _name;
    std::string             _topic;
    std::vector<Client*>    _clients;
    Client *                _moderator;

public:
    Channel(const std::string &name, Client *mod);
    ~Channel();

    void addClient(Client *client);
    void removeClient(Client *client);
    void broadcastMessage(const std::string &message, Client *sender);
    void setTopic(const std::string &topic);
    bool isOperator(Client *client);
    const std::string& getName() const;
    const std::string& getTopic() const;
};

#endif