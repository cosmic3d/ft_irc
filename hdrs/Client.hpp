/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:31:10 by damendez          #+#    #+#             */
/*   Updated: 2024/09/06 13:36:33 by damendez         ###   ########.fr       */
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
        std::string         _fullname;
        std::string         _ID;
        //const std::string   _host;
        bool                _authenticated; //Password correct
        bool                _registered; //Nick, user, pass correct
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
    	std::string							getUserName()		const;
		std::string							getNickName()		const;
		std::string							getFullName()		const;
		std::string							getID()				const;
		//std::string							getHost()			const;
		int									getClientfd()		const;
		bool								getAuth()			const;
		int									getRegistered()		const;
		int									getisOperator()		const;
		//int									getMode(char mode)	const;
		std::string							getUserPerfix()		const;
		std::string							getUserInfo()		const;
		std::string							getAllChannels()	const;
		std::map<std::string, Channel *>	getJoinedChannels()	const;
        
    public: // Setters
		void			setUserName(std::string UserName);
		void			setNickName(std::string NickName);
		void			setFullName(std::string FullName);
		//void			setHost(std::string Host);
		void			setID(std::string ID);
		void			setClientfd(int clientfd);
		void			setAuth(int Auth);
		void			setRegistered(int Registered);
		void			setIsOperator(int isOperator);
		//void			setMode(int value, char mode);
		void			joinChannel( std::string ChannelName, Channel *channel );
		void			leaveChannel( std::string ChannelName );
		std::string		leaveAllChannels();
};

#endif