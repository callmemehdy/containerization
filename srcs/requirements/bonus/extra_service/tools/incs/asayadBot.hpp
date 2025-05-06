#ifndef ASAYAD_HPP
# define ASAYAD_HPP

#include <IrcServ.hpp>


# define ERRSP ":WRONG NUMBER OF PARAMS\r\n"
# define USMELLRSP " :Hey, just a friendly reminder to take care of yourself! A shower can really help you feel refreshed and energized.\r\n"
# define QRAWZIQRA "remember, failing your exam is just life's way of telling you to take a break... forever.\r\n"
# define NOISE "The chat is getting a bit loud. Let's keep it calm so we can all focus. Appreciate your understanding!\r\n"
# define HLP "to use our bot use 'PRIVMSG Bot [{command}<space>{nickname of someone}]', Available commands are USMELL, NOISY, QRA\r\n"

# define ASAYAD  	std::cout <<COLOR_MAGENTA<< " ▗▄▖  ▗▄▄▖ ▗▄▖▗▖  ▗▖▗▄▖ ▗▄▄▄ ▗▄▄▖  ▗▄▖▗▄▄▄▖\n"<<COLOR_RESET;\
    				std::cout <<COLOR_MAGENTA<< "▐▌ ▐▌▐▌   ▐▌ ▐▌▝▚▞▘▐▌ ▐▌▐▌  █▐▌ ▐▌▐▌ ▐▌ █  \n"<<COLOR_RESET;\
   					std::cout <<COLOR_MAGENTA<< "▐▛▀▜▌ ▝▀▚▖▐▛▀▜▌ ▐▌ ▐▛▀▜▌▐▌  █▐▛▀▚▖▐▌ ▐▌ █  \n"<<COLOR_RESET;\
   					std::cout <<COLOR_MAGENTA<< "▐▌ ▐▌▗▄▄▞▘▐▌ ▐▌ ▐▌ ▐▌ ▐▌▐▙▄▄▀▐▙▄▞▘▝▚▄▞▘ █  \n\n"<<COLOR_RESET;


# define CNS	"\nTry to add a valid server ip/port, or check the server state."
# define ARGERR "Invalid number of args!\nUsage ./AsayadBot [Port] [IP_ADD] [SeverPassword]"
# define LOGIN "\r\nnick Bot\r\nuser * * * *\r\n"
# define LOGWELCOME ":42IRC 001 Bot :Welcome to the FT_IRC42 Network, Bot\r\n"
# define LOGERR "can't log to the provided server!"
# define NICKUSE "\nsome one using the bot nickname."
# define INCPSS "\nincorrect server's password."
# define MISINF "\nmisinformation in server's clues."
# define WAITCMD "Available commands: [USMELL], [QRA], [NOISY], [HELP].\nfor more information use help command in your client."


# define LINEEXCEED str == "usmell" ? USMELL : str == "qra" ? QRA : str == "noisy" ? NOISY : str == "help" ? HELP : UKN

class AsayadBot {
	private:
		static int	hopcount;
		int			Botfd;
		static int	_sig;
		std::string	password;
		std::vector<struct pollfd> fds;
		// todo
	public:
void			send_response(const char *buff, int fd);
				AsayadBot();
				~AsayadBot();
void			CreatSock();
void			login();
void			reply_back(std::string buff);
void			event_loop();
void			run(std::string passW, std::string ip, int port);
				typedef enum cmd {
					USMELL,
					QRA,
					NOISY,
					HELP,
					UKN
				}			cmd;
cmd				enumerate(std::string str);
void			usmell(std::string rec);
void			qra(std::string rec);
void			noisy(std::string rec);
void			help(std::string rec);


// static member functions

static void		signals(int sig);

};


#endif