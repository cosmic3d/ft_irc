/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:52:45 by damendez          #+#    #+#             */
/*   Updated: 2024/08/27 15:41:38 by jenavarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "Client.hpp"

class Channel {
private:
    std::string _name;
    std::string _topic;
    std::vector<Client*> _clients;
    Client *_operator;

public:
    Channel(const std::string &name, Client *op);
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