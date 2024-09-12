/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:52:45 by damendez          #+#    #+#             */
/*   Updated: 2024/09/12 18:39:49 by jenavarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#define	USERALREADYJOINED 0
#define USERISJOINED 1
#define NOTINCHANNEL 2
#define BANNEDFROMCHAN 3
#define TOOMANYCHANNELS 4
#define BADCHANNELKEY 5
#define CHANNELISFULL 6
#define NOSUCHCHANNEL 7
#define USERISBANNED 8
#define BADCHANMASK 9
#define USERNOTINCHANNEL -1
#define USERNOTFOUND -1

#include "Server.hpp"

class Client;

class Channel 
{
    private:
        char                    _prefix;
        Client*                 _creator;
        int                     _onlineUsers;
        std::string             _name;
        std::string				_key;
        std::string             _topic;
        std::map<int, Client *> _members;
        std::map<int, Client *> _operators;
        /*  
         *  Users with a plus sign (+) next to their nickname are voiced users. That
            means that they are allowed to speak when the channel mode +m is set. +m
            sets moderated mode which means that regular users may not speak.
        */ 
        std::vector<std::string>    _banned; //Hay que mirar que el formato de los baneos sea correcto (En el protocolo IRC se banea por máscara, como en el invite-only)

    public:
        Channel();
        Channel(std::string channelName, Client *creator);
        Channel(std::string channelName, std::string channelKey, Client *creator);
        Channel(const Channel& x);
        Channel &operator=(const Channel& rhs);
        ~Channel();

   	public: /*             Getters                         */
        const char                      &getPrefix() const;
        const int                       &getOnlineUsers() const; 
        const std::string               &getName() const;
        const std::string               &getKey() const;
        const std::string               &getTopic() const;
        std::map<int, Client *>         getAllUsers() const;
        const std::map<int, Client *>   &getMembers() const;
        const std::map<int, Client *>   &getOperators() const;
        const std::map<int, Client *>   &getVoice() const;
        Client*                         getCreator() const;
        
    public: // modifiers
		int		addMember( Client *member );
		int		addOperator( Client *member );
		void	addVoice( Client *member );
		int		banUser( Client *member );
		void	removeOperator( int i );
		void	removeVoice( int i );
		void	removeBanned( std::string NickName );
		void	removeMember( int i );
    
	public: /*             Setters                         */
        void	setName(std::string name);
        void	setTopic(std::string topic);
        void	setKey(std::string key);
        void    setPrefix(char prefix);
        void    setOnlineUsers(int online);

    public:
        std::pair<Client *, int>    findUserRole( int i );
        std::string                 listAllUsers() const;
};

#endif