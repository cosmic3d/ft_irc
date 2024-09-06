/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responses.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jenavarr <jenavarr@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 21:23:16 by jenavarr          #+#    #+#             */
/*   Updated: 2024/08/26 21:23:16 by jenavarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSES_HPP
#define RESPONSES_HPP

// #pragma once // This is a non-standard directive but it is supported by most compilers. It is used to include the file only once in a compilation. It is similar to the include guards but it is more efficient and easy to use.

// Section 1: Welcome Messages (001 - 005)
#define RPL_WELCOME       "001" // "Welcome to the Internet Relay Network <nick>!<user>@<host>"
#define RPL_YOURHOST      "002" // "Your host is <servername>, running version <ver>"
#define RPL_CREATED       "003" // "This server was created <date>"
#define RPL_MYINFO        "004" // "<servername> <version> <available user modes> <available channel modes>"
#define RPL_BOUNCE        "005" // "Try server <server name>, port <port number>"

// Section 2: Userhost and ISON Replies (302 - 303)
#define RPL_USERHOST      "302" // ":*1<reply> *( " " <reply> )"
#define RPL_ISON          "303" // ":*1<nick> *( " " <nick> )"

// Section 3: Away and WHOIS Replies (301, 305 - 319)
#define RPL_AWAY          "301" // "<nick> :<away message>"
#define RPL_UNAWAY        "305" // ":You are no longer marked as being away"
#define RPL_NOWAWAY       "306" // ":You have been marked as being away"
#define RPL_WHOISUSER     "311" // "<nick> <user> <host> * :<real name>"
#define RPL_WHOISSERVER   "312" // "<nick> <server> :<server info>"
#define RPL_WHOISOPERATOR "313" // "<nick> :is an IRC operator"
#define RPL_WHOISIDLE     "317" // "<nick> <integer> :seconds idle"
#define RPL_ENDOFWHOIS    "318" // "<nick> :End of WHOIS list"
#define RPL_WHOISCHANNELS "319" // "<nick> :*( ( "@" / "+" ) <channel> " " )"

// Section 4: WHOWAS Replies (314, 369)
#define RPL_WHOWASUSER    "314" // "<nick> <user> <host> * :<real name>"
#define RPL_ENDOFWHOWAS   "369" // "<nick> :End of WHOWAS"

// Section 5: Channel List Replies (321 - 325)
#define RPL_LISTSTART     "321" // "Obsolete. Not used."
#define RPL_LIST          "322" // "<channel> <# visible> :<topic>"
#define RPL_LISTEND       "323" // ":End of LIST"
#define RPL_UNIQOPIS      "325" // "<channel> <nickname>"

// Section 6: Channel Mode and Topic Replies (324, 331 - 332)
#define RPL_CHANNELMODEIS "324" // "<channel> <mode> <mode params>"
#define RPL_NOTOPIC       "331" // "<channel> :No topic is set"
#define RPL_TOPIC         "332" // "<channel> :<topic>"

// Section 7: Invitation and Summoning Replies (341 - 342)
#define RPL_INVITING      "341" // "<channel> <nick>"
#define RPL_SUMMONING     "342" // "<user> :Summoning user to IRC"

// Section 8: Invite and Exception List Replies (346 - 349)
#define RPL_INVITELIST        "346" // "<channel> <invitemask>"
#define RPL_ENDOFINVITELIST   "347" // "<channel> :End of channel invite list"
#define RPL_EXCEPTLIST        "348" // "<channel> <exceptionmask>"
#define RPL_ENDOFEXCEPTLIST   "349" // "<channel> :End of channel exception list"

// Section 9: Server Version and WHO Replies (351 - 353, 315)
#define RPL_VERSION       "351" // "<version>.<debuglevel> <server> :<comments>"
#define RPL_WHOREPLY      "352" // "<channel> <user> <host> <server> <nick> ( "H" / "G" > ["*"] [ ( "@" / "+" ) ] :<hopcount> <real name>"
#define RPL_ENDOFWHO      "315" // "<name> :End of WHO list"
#define RPL_NAMREPLY      "353" // "( "=" / "*" / "@" ) <channel> :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )"

// Section 10: End of Names Reply (366)
#define RPL_ENDOFNAMES    "366" // "<channel> :End of NAMES list"

// Section 11: Server Links Replies (364 - 365)
#define RPL_LINKS         "364" // "<mask> <server> :<hopcount> <server info>"
#define RPL_ENDOFLINKS    "365" // "<mask> :End of LINKS list"

// Section 12: Channel Ban List Replies (367 - 368)
#define RPL_BANLIST       "367" // "<channel> <banmask>"
#define RPL_ENDOFBANLIST  "368" // "<channel> :End of channel ban list"

// Section 13: Info Replies (371 - 376)
#define RPL_INFO          "371" // ":<string>"
#define RPL_ENDOFINFO     "374" // ":End of INFO list"
#define RPL_MOTDSTART     "375" // ":- <server> Message of the day - "
#define RPL_MOTD          "372" // ":- <text>"
#define RPL_ENDOFMOTD     "376" // ":End of MOTD command"

// Section 14: Operator Replies (381 - 383)
#define RPL_YOUREOPER     "381" // ":You are now an IRC operator"
#define RPL_REHASHING     "382" // "<config file> :Rehashing"
#define RPL_YOURESERVICE  "383" // "You are service <servicename>"

// Section 15: Server Time Replies (391 - 395)
#define RPL_TIME          "391" // "<server> :<string showing server's local time>"
#define RPL_USERSSTART    "392" // ":UserID   Terminal  Host"
#define RPL_USERS         "393" // ":<username> <ttyline> <hostname>"
#define RPL_ENDOFUSERS    "394" // ":End of users"
#define RPL_NOUSERS       "395" // ":Nobody logged in"

// Section 16: Trace Replies (200 - 210, 261 - 262)
#define RPL_TRACELINK         "200" // "Link <version & debug level> <destination> <next server> V<protocol version> <link uptime in seconds> <backstream sendq> <upstream sendq>"
#define RPL_TRACECONNECTING   "201" // "Try. <class> <server>"
#define RPL_TRACEHANDSHAKE    "202" // "H.S. <class> <server>"
#define RPL_TRACEUNKNOWN      "203" // "???? <class> [<client IP address in dot form>]"
#define RPL_TRACEOPERATOR     "204" // "Oper <class> <nick>"
#define RPL_TRACEUSER         "205" // "User <class> <nick>"
#define RPL_TRACESERVER       "206" // "Serv <class> <int>S <int>C <server> <nick!user|*!*>@<host|server> V<protocol version>"
#define RPL_TRACESERVICE      "207" // "Service <class> <name> <type> <active type>"
#define RPL_TRACENEWTYPE      "208" // "<newtype> 0 <client name>"
#define RPL_TRACECLASS        "209" // "Class <class> <count>"
#define RPL_TRACERECONNECT    "210" // Unused.
#define RPL_TRACELOG          "261" // "File <logfile> <debug level>"
#define RPL_TRACEEND          "262" // "<server name> <version & debug level> :End of TRACE"

// Section 17: Server Stats Replies (211 - 219, 221, 234 - 235, 242 - 243)
#define RPL_STATSLINKINFO     "211" // "<linkname> <sendq> <sent messages> <sent Kbytes> <received messages> <received Kbytes> <time open>"
#define RPL_STATSCOMMANDS     "212" // "<command> <count> <byte count> <remote count>"
#define RPL_ENDOFSTATS        "219" // "<stats letter> :End of STATS report"
#define RPL_STATSUPTIME       "242" // ":Server Up %d days %d:%02d:%02d"
#define RPL_STATSOLINE        "243" // "O <hostmask> * <name>"
#define RPL_UMODEIS           "221" // "<user mode string>"
// #define RPL_SERVLIST          "234" // "<name> <server> <mask> <type> <hopcount> <info>"
// #define RPL_SERVLISTEND       "235" // "<mask> <type> :End of service listing"

// Section 18: LUsers Replies (251 - 255)
#define RPL_LUSERCLIENT       "251" // ":There are <integer> users and <integer> services on <integer> servers"
#define RPL_LUSEROP           "252" // "<integer> :operator(s) online"
#define RPL_LUSERUNKNOWN      "253" // "<integer> :unknown connection(s)"
#define RPL_LUSERCHANNELS     "254" // "<integer> :channels formed"
#define RPL_LUSERME           "255" // ":I have <integer> clients and <integer> servers"

// Section 19: Local and Global Users Replies (265 - 266)
#define RPL_LOCALUSERS        "265" // ":Current local users: <integer>  Max: <integer>"
#define RPL_GLOBALUSERS       "266" // ":Current global users: <integer>  Max: <integer>"

// Section 20: Admin and Host Replies (256 - 263)
#define RPL_ADMINME           "256" // "<server> :Administrative info"
#define RPL_ADMINLOC1         "257" // ":<admin info>"
#define RPL_ADMINLOC2         "258" // ":<admin info>"
#define RPL_ADMINEMAIL        "259" // ":<admin info>"
#define RPL_TRYAGAIN          "263" // ":Please wait a while and try again."

// Section 21: Error Replies (401 - 476)
#define ERR_NOSUCHNICK        "401" // "<nickname> :No such nick/channel"
#define ERR_NOSUCHSERVER      "402" // "<server name> :No such server"
#define ERR_NOSUCHCHANNEL     "403" // "<channel name> :No such channel"
#define ERR_CANNOTSENDTOCHAN  "404" // "<channel name> :Cannot send to channel"
#define ERR_TOOMANYCHANNELS   "405" // "<channel name> :You have joined too many channels"
#define ERR_WASNOSUCHNICK     "406" // "<nickname> :There was no such nickname"
#define ERR_TOOMANYTARGETS    "407" // "<target> :Duplicate recipients. No message delivered"
#define ERR_NOORIGIN          "409" // ":No origin specified"
#define ERR_NORECIPIENT       "411" // ":No recipient given (<command>)"
#define ERR_NOTEXTTOSEND      "412" // ":No text to send"
#define ERR_NOTOPLEVEL        "413" // "<mask> :No toplevel domain specified"
#define ERR_WILDTOPLEVEL      "414" // "<mask> :Wildcard in toplevel domain"
#define ERR_UNKNOWNCOMMAND    "421" // "<command> :Unknown command"
#define ERR_NOMOTD            "422" // ":MOTD File is missing"
#define ERR_NOADMININFO       "423" // "<server> :No administrative info available"
#define ERR_FILEERROR         "424" // ":File error doing <file op> on <file>"
#define ERR_NONICKNAMEGIVEN   "431" // ":No nickname given"
#define ERR_ERRONEUSNICKNAME  "432" // "<nick> :Erroneous nickname"
#define ERR_NICKNAMEINUSE     "433" // "<nick> :Nickname is already in use"
#define ERR_NICKCOLLISION     "436" // "<nick> :Nickname collision KILL"
#define ERR_USERNOTINCHANNEL  "441" // "<nick> <channel> :They aren't on that channel"
#define ERR_NOTONCHANNEL      "442" // "<channel> :You're not on that channel"
#define ERR_USERONCHANNEL     "443" // "<user> <channel> :is already on channel"
#define ERR_NOLOGIN           "444" // "<user> :User not logged in"
#define ERR_SUMMONDISABLED    "445" // ":SUMMON has been disabled"
#define ERR_USERSDISABLED     "446" // ":USERS has been disabled"
#define ERR_NOTREGISTERED     "451" // ":You have not registered"
#define ERR_NEEDMOREPARAMS    "461" // "<command> :Not enough parameters"
#define ERR_ALREADYREGISTRED  "462" // ":You may not reregister"
#define ERR_NOPERMFORHOST     "463" // ":Your host isn't among the privileged"
#define ERR_PASSWDMISMATCH    "464" // ":Password incorrect"
#define ERR_YOUREBANNEDCREEP  "465" // ":You are banned from this server"
#define ERR_KEYSET            "467" // "<channel> :Channel key already set"
#define ERR_CHANNELISFULL     "471" // "<channel> :Cannot join channel (+l)"
#define ERR_UNKNOWNMODE       "472" // "<char> :is unknown mode char to me"
#define ERR_INVITEONLYCHAN    "473" // "<channel> :Cannot join channel (+i)"
#define ERR_BANNEDFROMCHAN    "474" // "<channel> :Cannot join channel (+b)"
#define ERR_BADCHANNELKEY     "475" // "<channel> :Cannot join channel (+k)"
#define ERR_BADCHANNELMASK    "476" // "<channel> :Cannot join channel (+k)"
#define ERR_NOPRIVILEGES      "481" // ":Permission Denied- You're not an IRC operator"
#define ERR_CHANOPRIVSNEEDED  "482" // "<channel> :You're not channel operator"
#define ERR_CANTKILLSERVER    "483" // ":You can't kill a server!"
#define ERR_NOOPERHOST        "491" // ":No O-lines for your host"

// Section 22: Kill and Ping Replies (250, 361, 364)
#define RPL_KILLDONE          "361" // "<nick> :has been killed"
#define RPL_PONG              "364" // "<server> :Ping reply from server"

// Section 23: Other Replies (251 - 260, 263 - 376, 392 - 401, 414 - 444, 455 - 504)
#define RPL_SERVICEINFO       "231" // "Service <service name> <service info>"
#define RPL_SERVICE           "232" // "<name> <info>"
#define RPL_ENDOFSERVICES     "233" // "<name> :End of service listing"
// #define RPL_SERVICE_LIST      "235" // "List of services"
// #define RPL_SERVICE_LISTEND   "236" // "End of service list"
#define RPL_STATSDLINE        "240" // "<Dline>"
#define RPL_SERVLIST          "235" // "Server list"
#define RPL_SERVLISTEND       "236" // "End of server list"
#define RPL_TRACEINFO         "241" // "<info>"
#define RPL_USERHOST          "302" // "User host"
#define RPL_ENDOFWHO          "315" // "<name> :End of WHO list"
#define RPL_NAMREPLY          "353" // "( "=" / "*" / "@" ) <channel> :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )"
#define RPL_ENDOFNAMES        "366" // "<channel> :End of NAMES list"
#define RPL_INFOSTART         "373" // "Start of Info"

#endif