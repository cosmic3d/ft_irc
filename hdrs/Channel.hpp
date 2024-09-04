/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damendez <damendez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 08:52:45 by damendez          #+#    #+#             */
/*   Updated: 2024/09/04 14:36:28 by damendez         ###   ########.fr       */
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

class Channel 
{
    private:
        char                    _prefix;
        int                     _onlineUsers;
        Client*                 _creator;
        std::string             _name;
        std::string             _topic;
        std::string				_key;
        std::map<int, Client *> _members;
        std::map<int, Client *> _operators;
        //std::map<int, Client *> _voice; //?
        std::vector<std::string>    _banned;

    public:
        Channel();
        Channel(std::string channelName, Client *creator);
        Channel(std::string channelName, std::string channelKey, Client *creator);
        Channel(const Channel& x);
        Channel &operator=(const Channel& rhs);
        ~Channel();

   	public: /*             Getters                         */
        const std::string& getName() const;
        const std::string& getTopic() const;
        const std::string& getKey() const;

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
};

#endif