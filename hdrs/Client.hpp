/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:31:10 by damendez          #+#    #+#             */
/*   Updated: 2024/09/12 18:51:37 by jenavarr         ###   ########.fr       */
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
        Server*             _server;
        std::string         _nickname;
        std::string         _username;
        std::string         _hostname;
        bool                _authenticated; //Password correct
        bool                _registered; //Nick, user, pass correct
        std::string _receiveBuffer;
        std::map<std::string, Channel *>    _joinedChannels; //Cuando el usuario se une a un canal, se añade a este mapa. Cuando se desconecte, se eliminará de cada canal.

    public:
        Client();
        Client(int socket, Server *server );
        Client(const Client & x);
        ~Client();
        Client &operator=(const Client & rhs);

        int         isJoined(std::string ChannelName) const;
        std::string joinedChannels() const;
        
    public: // Getters
        Server*                             getServer()			const;
    	std::string							getUsername()		const;
		std::string							getNickname()		const;
		std::string                         getHostname()       const;
		int									getClientfd()		const;
		bool								getAuth()			const;
		int									getRegistered()		const;
		//int									getMode(char mode)	const;
		std::string							mask()		const;
		std::string							getUserInfo()		const;
		std::string							getAllChannels()	const;
		std::map<std::string, Channel *>	getJoinedChannels()	const;
        
    public: // Setters
		void			setUsername(std::string username);
		void			setNickname(std::string nickname);
		void			setClientfd(int clientfd);
		void			setAuth(int Auth);
		void			setRegistered(int Registered);
		//void			setMode(int value, char mode);
		void			joinChannel( std::string ChannelName, Channel *channel );
		void			leaveChannel( std::string ChannelName );
		std::string		leaveAllChannels();
        bool    checkRegistered();
        std::string retrieveHostnameIp() const;
        std::string &getReceiveBuffer();
        void appendToReceiveBuffer(const std::string &data);
};

#endif