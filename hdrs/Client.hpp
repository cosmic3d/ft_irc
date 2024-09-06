/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:31:10 by damendez          #+#    #+#             */
/*   Updated: 2024/09/06 19:22:48 by damendez         ###   ########.fr       */
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
        std::string         _hostname;
        //const std::string   _host;
        bool                _authenticated; //Password correct
        bool                _registered; //Nick, user, pass correct
        std::string _receiveBuffer;
        bool                _isOperator;
        std::map<std::string, Channel *>    _joinedChannels;

    public:
        Client();
        Client(int socket);
        Client(const Client & x);
        ~Client();
        Client &operator=(const Client & rhs);

        int         isJoined(std::string ChannelName) const;
        std::string joinedChannels() const;
        
    public: // Getters
    	std::string							getUsername()		const;
		std::string							getNickname()		const;
		std::string                         getHostname()       const;
		int									getClientfd()		const;
		bool								getAuth()			const;
		int									getRegistered()		const;
		int									getOperator()		const;
		//int									getMode(char mode)	const;
		std::string							formatPrefix()		const;
		std::string							getUserInfo()		const;
		std::string							getAllChannels()	const;
		std::map<std::string, Channel *>	getJoinedChannels()	const;
        
    public: // Setters
		void			setUsername(std::string username);
		void			setNickname(std::string nickname);
		void			setClientfd(int clientfd);
		void			setAuth(int Auth);
		void			setRegistered(int Registered);
		void			setIsOperator(int isOperator);
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