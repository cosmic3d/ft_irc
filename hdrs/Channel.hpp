/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:52:45 by damendez          #+#    #+#             */
/*   Updated: 2024/08/29 15:37:22 by damendez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"

class Channel 
{
    private:
        std::string             _name;
        std::string             _topic;
        std::string				_key;
        int                     _onlineUsers;
        std::vector<Client*>    _clients;
        Client *                _creator;

   	public: /*             Getters                         */
        const std::string& getName() const;
        const std::string& getTopic() const;
        const std::string& getKey() const;

	public: /*             Setters                         */
        void	setName(std::string name);
        void	setTopic(std::string topic);
        void	setKey(std::string key);

    public:
        Channel(const std::string &name, Client *creator);
        ~Channel();

        void addClient(Client *client);
        void removeClient(Client *client);
        void broadcastMessage(const std::string &message, Client *sender);
        void setTopic(const std::string &topic);
        bool isOperator(Client *client);
};

#endif