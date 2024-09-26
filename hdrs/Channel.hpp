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
#define NOTINVITED 10
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
		int                     _userLimit;
		std::string             _name;
		std::string				_key;
		std::string             _topic;
		std::map<int, Client *> _members;
		std::map<int, Client *> _operators;
		std::vector<std::string>   _banMasks; //Lista de máscaras de baneo
		std::vector<std::string>   _inviteMasks; //Lista de máscaras de invitación
		bool                        _inviteOnly;
		bool                        _topicRestricted;

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
		bool                      getTopicRestricted() const;
		bool                      getInviteOnly() const;
		std::string				        getModes() const;
		bool											getBanMask(std::string mask) const;
		bool											getInviteMask(std::string mask) const;
		int												getUserLimit() const;
		
	public: // modifiers
		int		addMember( Client *member, bool checkConditions = true );
		int		addOperator( Client *member );
		void	addVoice( Client *member );
		void	removeOperator( int i );
		void	removeVoice( int i );
		void	removeMember( int i );
	
	public: /*             Setters                         */
		void	setName(std::string name);
		void	setTopic(std::string topic);
		void	setKey(std::string key);
		void    setPrefix(char prefix);
		void    setOnlineUsers(int online);
		void		setInviteOnly(bool inviteOnly);
		void		setTopicRestricted(bool topicRestricted);
		void		setBanMask(std::string mask);
		void		setInviteMask(std::string mask);
		void		removeBanMask(std::string mask);
		void		removeInviteMask(std::string mask);
		void		setUserLimit(int limit);

	public:
		bool	isOperator( int i ) const;
		bool	isMember( int i ) const;
		std::string                 listAllUsers() const;
		bool matchMask(const std::string& mask, const std::string& address) const;
		bool matchMaskList(const std::vector<std::string>& maskList, const std::string& address) const;
};

#endif