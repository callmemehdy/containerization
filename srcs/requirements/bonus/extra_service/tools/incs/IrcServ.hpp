/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-amma <ael-amma@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 11:29:22 by mel-akar          #+#    #+#             */
/*   Updated: 2025/04/19 15:23:14 by ael-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP


#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>
#include <cstdlib>
#include <netdb.h>
#include <iostream>
#include <sys/socket.h>
#include <exception>
#include <errno.h>
#include <cstring>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <algorithm>
#include <limits.h>

#define COLOR_RESET      "\033[0m"
#define COLOR_BLACK      "\033[38;5;16m"
#define COLOR_RED        "\033[38;5;124m"
#define COLOR_GREEN      "\033[38;5;34m"
#define COLOR_YELLOW     "\033[38;5;226m"
#define COLOR_BLUE       "\033[38;5;32m"
#define COLOR_MAGENTA    "\033[38;5;201m"
#define COLOR_CYAN       "\033[38;5;51m"
#define COLOR_WHITE      "\033[38;5;15m"
#define COLOR_ORANGE     "\033[38;5;214m"
#define COLOR_GRAY       "\033[38;5;8m"
#define COLOR_LIGHT_GRAY "\033[38;5;250m"


#define IRC42_PATTERN \
    std::cout << COLOR_GREEN<<"░▒▓█▓▒░░▒▓███████▓▒░  ░▒▓██████▓▒░ ░▒▓█▓▒░░▒▓█▓▒░░▒▓███████▓▒░  \n"<<COLOR_RESET; \
    std::cout << COLOR_GREEN<<"░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░       ░▒▓█▓▒░ \n"<<COLOR_RESET; \
    std::cout << COLOR_GREEN<<"░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░       ░▒▓█▓▒░░▒▓█▓▒░       ░▒▓█▓▒░ \n"<<COLOR_RESET; \
    std::cout << COLOR_GREEN<<"░▒▓█▓▒░░▒▓███████▓▒░ ░▒▓█▓▒░       ░▒▓████████▓▒░ ░▒▓██████▓▒░  \n"<<COLOR_RESET; \
    std::cout << COLOR_GREEN<<"░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░              ░▒▓█▓▒░░▒▓█▓▒░        \n"<<COLOR_RESET; \
    std::cout << COLOR_GREEN<<"░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░       ░▒▓█▓▒░░▒▓█▓▒░        \n"<<COLOR_RESET; \
    std::cout << COLOR_GREEN<<"░▒▓█▓▒░░▒▓█▓▒░░▒▓█▓▒░ ░▒▓██████▓▒░        ░▒▓█▓▒░░▒▓████████▓▒░ \n"<<COLOR_RESET;


// some Macros for the server use 
# define SPORT		0x1A0A
# define BACKLOG	10
# define MAXREQLEN	512


// some parsing errors macros
# define PORTERROR "the port number is out of range!"
# define PASSERROR "the password cannot be used!\nA valid password mustn't be empty and should contains only 1 word."

// RPLS
# define RPL_WELCOME ":42IRC 001"



// edge commands
# define ERR_NOTREGISTERED	":42IRC 451"
# define ERR_UNKNOWNCOMMAND	":42IRC 421"

// PASS COMMAND'S RPLS
# define ERR_NEEDMOREPARAMS ":42IRC 461"
# define ERR_PASSWDMISMATCH ":42IRC 464"

// NICK COMMANDS'S RPLS
# define ERR_NONICKNAMEGIVEN	":42IRC 431"
# define ERR_NICKNAMEINUSE		":42IRC 433"
# define ERR_ERRONEUSNICKNAME	":42IRC 432"
# define ERR_ALREADYREGISTRED	":42IRC 462"

// privmsg errors hhh
# define ERR_TOOMANYTARGETS ":42IRC 407" 
# define ERR_NOTEXTTOSEND	":42IRC 412"
# define ERR_NORECIPIENT	":42IRC 411"
# define PRIVLIMITERR		" :there's more than one target to receive the message{ServerReceiversLimit=1}\r\n"

//	Commands MACROS
# define MAX_JOINCHAN			10
# define ERR_UNKNOWNERROR		":42IRC 400 "

//	COMMON
# define ERR_NEEDMOREPARAMS2	":42IRC	461 "
# define ERR_NOSUCHCHANNEL		":42IRC 403 "
# define ERR_CHANOPRIVSNEEDED	":42IRC 482 "


//	JOIN
# define ERR_BADCHANNAME		":42IRC 479 "
# define ERR_TOOMANYCHANNELS	":42IRC 405 "
# define ERR_BADCHANNELKEY		":42IRC 475 "
# define ERR_CHANNELISFULL		":42IRC 471 "
# define RPL_NAMREPLY			":42IRC 353 "
# define RPL_ENDOFNAMES			":42IRC 366 "
# define RPL_CHANNELMODEIS		":42IRC 324 "

//	INVITE
# define ERR_INVITEONLYCHAN		":42IRC 473 "
# define RPL_INVITING			":42IRC 341 "

//	TOPIC
# define RPL_NOTOPIC			":42IRC 331 "
# define RPL_TOPIC				":42IRC 332 "

//	MODE
# define ERR_UNKNOWNMODE		":42IRC 472 "
# define ERR_KEYSET				":42IRC 467 "
# define ERR_USERNOTINCHANNEL	":42IRC 441 "
# define RPL_CHANNELMODEIS		":42IRC 324 "

	/////////////////////////////////////////////////////
   //some typdef to make it easy to define an object ///
  /////////////////////////////////////////////////////
typedef struct pollfd POLL;
typedef struct sockaddr_in SOCK_ADD;
	/////////////////////////////////////////////////////
   //        some classes for the server part        ///
  /////////////////////////////////////////////////////

class Client {
	private:
			int						c_fd;
			std::string				ip_addr;
			std::string				nickname;
			std::string				username;
			std::string				real_name;
			bool					isreg;
			bool					authenticated;
			std::string				eof_buff;
			int						Climit;
			std::set<std::string>	channels;
			// gotta put all the client attributes that a client needs here ...
	public:
			Client();
			Client(int fd, const std::string& ip);
			void							set_fd(int fd);
			void							set_nick(const std::string& nick);
			void							set_user(const std::string& user);
			void							set_real(const std::string& real);
			void							make_reg();
			void							make_auth();
			void							set_ip(const std::string& ip);
			int								get_fd();
			const std::string&				getIp();
			const std::string&				getuser();
			std::string&					get_buff();
			const std::string&				get_nick();
			const std::string&				get_user();
			const std::string&				get_real();
			const std::string&				get_ip();
			bool							get_auth();
			bool							get_reg();

			int								getCLimit();

			void							joinChannel(const std::string &channelName);
			~Client();
};


class Channel {
	private:
			int						limit;
			std::string				name;
			std::string				key;
			std::string				topic;
			std::string				modes;
			std::set<std::string>	invited;
			std::map<int, Client>	users;
			std::map<int, Client>	op;

	public:
			Channel();
			Channel(std::string _name);
			~Channel();

			void							addUser(int fd, const Client& client);
			void							addOp(int fd, const Client& client);
			void							setInvite(const std::string& nick);
			bool							isInvited(const std::string& nick);
			bool							isUserInChannel(const std::string& nick);
			bool							isNew( void );
			void							setKey(std::string pass);
			int								getLimit( void );
			const std::string&				getKey( void );
			const std::string&				getName( void );
			const std::string&				getTopic( void );
			const std::string&				getModes( void );
			const std::string				listUsers( void );
			std::map<int, Client>&			getUsers( void );
			void							broadcast(const std::string& msg);
			bool							isOP( const std::string& nick );
			void							kickUser( const std::string& nick );
			void							setTopic( const std::string& topic );
			void							addMode( char );
			void							rmMode( char );
			bool							isMode( char );
			void							setLimit( int );
			void							promoteUser( const std::string& );
};

class	IChannel
{
	public:
			virtual void					joinCh( const std::vector<std::string>& vec, int _fd ) = 0;
			virtual void					kickCh( const std::vector<std::string>& vec, int _fd ) = 0;
			virtual void					inviteCh( const std::vector<std::string>& vec, int _fd ) = 0;
			virtual void					topicCh( const std::vector<std::string>& vec, int _fd ) = 0;
			virtual void					modeCh( const std::vector<std::string>& vec, int _fd ) = 0;
};

class IAuth {
	public:
			virtual void					passAth(const std::vector<std::string>& vec, int _fd) = 0;
			virtual void					userAth(const std::vector<std::string>& vec, int _fd) = 0;	
			virtual void					nickAth(const std::vector<std::string>& vec, int _fd) = 0;
			virtual void					privMsg(const std::vector<std::string>& vec, int _fd) = 0;
	};
	
	class Server : public IChannel, public IAuth
	{
		private:
				static bool								_sig;
				int										s_fd;
				int										s_port;
				std::string 							password;
				// todo
				public:
				static std::map<std::string, Channel>	channels;
				static std::vector<struct pollfd>		fds;
				static std::vector<Client>				clients;
				Server();
				~Server();
				
				// operator bool() const;
	
				static void					signal_handling();
				static void					Signal_Handler(int _signal);
				void						set_fd(int fd);
				void						set_port(int port);
				void						set_pass(std::string pass);
				void						create_sock();
				void						bind_sock();
				void						listening();
				void						run();
				bool						recv_request(int fd);
				// void						send_response(const char *buff, int fd);
				void						accept_connect();
				void						handle_request(std::string buff, int fd);

				void						joinCh( const std::vector<std::string>& vec, int _fd );
				void						kickCh( const std::vector<std::string>& vec, int _fd );
				void						inviteCh( const std::vector<std::string>& vec, int _fd );
				void						topicCh( const std::vector<std::string>& vec, int _fd );
				void						modeCh( const std::vector<std::string>& vec, int _fd );

				void						passAth(const std::vector<std::string>& vec, int _fd);
				void						userAth(const std::vector<std::string>& vec, int _fd);	
				void						nickAth(const std::vector<std::string>& vec, int _fd);
				void						privMsg(const std::vector<std::string>& vec, int _fd);

				// TOOLS FOR COMMANDS
				int							get_fd(const std::string& nick);
				bool						nicksearch(const std::string& nick);
				bool						nickname_chars(const std::string& nick);
				bool						is_auth(int fd);
				bool						is_reg(int fd);
				bool						is_reg(const std::string& nick);
				bool						set_nick(const std::string& nick, int fd);
				bool						set_user(const std::string& user, int fd);
				const std::string			get_nick(int fd); 
				Client*						get_client(int fd);
				void						check_state(int fd, std::string& cmd);
				void						NotRegMsg(int fd);
				bool						handle_auth(std::vector<std::string>& req, int fd);
				// void						remove_client(int fd);
				void						target_spec(const std::vector<std::string>& vec, int fd);
				bool						check_limit(std::string receivers);
				std::vector<std::string>	multiple_cmds(std::string commands);
				bool						still_alive(int fd);
				void						nickleftover(Client& client);
				// todo

				Channel*					isChannel( const std::string &channelName );
				Channel*					getChannel( const std::string &channelName );
		};
		

		// Server_tools
		void								send_response(const char *buff, int fd);
		void								remove_client(int fd);
		std::vector<std::string>			ft_split(std::string str);
		std::string							to_lower(std::string& cmd);	
#endif