#include <asayadBot.hpp>

int	AsayadBot::hopcount;

int AsayadBot::_sig(false);

AsayadBot::AsayadBot() : Botfd(-1) {
	
}

AsayadBot::~AsayadBot() {
	close(Botfd);
}

void	AsayadBot::signals(int sig) {
	if (sig == SIGINT || sig == SIGQUIT || sig == SIGPIPE)  {
		_sig = true;
	}
}

void	AsayadBot::CreatSock() {
	struct pollfd	fd;	bzero(&fd, sizeof(struct pollfd));
	signal(SIGQUIT, signals);
	signal(SIGINT, signals);
	signal(SIGPIPE, signals);
	Botfd = socket(AF_INET, SOCK_STREAM, 0);
	if (Botfd == -1)
		throw std::runtime_error(std::string("socket syscall: ")+strerror(errno));
	fd.events = POLLIN;	fd.fd = Botfd; fds.push_back(fd);
}

void	AsayadBot::send_response(const char *buff, int fd) {
		int sents;
		// gotta handle the partial sends by putting this send inside a loop
		int ret = send(fd, buff, strlen(buff), 0);
		if (ret == -1) {
			// if (errno == EPIPE || errno == ECONNRESET) {
			// 	std::cout << COLOR_BLUE<< fd \
			// 			<< "error while sending data to served server" \
			// 			<< COLOR_RESET<< std::endl;
			// 	_sig = true;
			// 	return ;
			// }
			// if (errno == EAGAIN || errno == EWOULDBLOCK) return ;
			throw std::runtime_error("send syscall failed " + std::string(strerror(errno)));
		}
}

void	AsayadBot::login() {
	int			rd;
	std::string	err;
	char		logbuff[MAXREQLEN + 1];
	std::string	request("pass " + password + LOGIN);
	bzero(logbuff, MAXREQLEN + 1);

	send_response(request.c_str(), Botfd);
	if ((rd = recv(Botfd, logbuff, MAXREQLEN, 0)) == -1 && !_sig) {
		throw std::runtime_error("an error occurs while reading form socket");
	} else {
		logbuff[rd] = 0;
		if (logbuff == std::string(LOGWELCOME)) {
			std::cout <<COLOR_GREEN<< "Logged successfully" <<COLOR_RESET<< std::endl;
			ASAYAD
		} else {
			std::vector<std::string> spl = ft_split(logbuff);
			err = (spl[0] == "433") ? NICKUSE : (spl[0] == "464") ? INCPSS : MISINF;
			throw std::runtime_error(LOGERR + std::string(" "+err));
		}
	}
}


void	AsayadBot::reply_back(std::string buff) {
	//
	std::vector<std::string> cmd = ft_split(buff);

	if (cmd.size() < 4) while (cmd.size() < 4) cmd.push_back("");
	cmd[2] = to_lower(cmd[0]) == "privmsg" ? cmd[2] : "hihi";

	// cmds
	switch (enumerate(cmd[2])) {
		case USMELL:
			usmell(cmd[2]); break;
		case QRA:
			qra(cmd[2]); break;
		case NOISY:
			noisy(cmd[2]); break;
		case HELP:
			help(cmd[-1 + cmd.size()]); break;
		case UKN:
		// handle uknown commands hhhhh by breaking form switch
			break;
	}
	return ;
}


void	AsayadBot::event_loop() {
	char	buff[MAXREQLEN + 1];

	bzero(buff, MAXREQLEN + 1);
	std::cout <<COLOR_MAGENTA<<WAITCMD<<COLOR_RESET<<std::endl;
	if (fcntl(Botfd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error(strerror(errno));
	while (_sig == false) {
		// should buffer the msg
		if (poll(&fds[0], fds.size(), -1) == -1 && !_sig)
			throw std::runtime_error(strerror(errno));
		if (fds[0].revents & POLLIN) {
			int ret = recv(Botfd, buff, MAXREQLEN, 0);
			if (ret == -1 && !_sig) {
				throw std::runtime_error("recv failed");
			} else if (ret == 0) {
				std::cout <<COLOR_BLUE<< "the served server disconnected!"<<COLOR_RESET<<std::endl;
				return ;
			} else {
				buff[ret] = 0;
				reply_back(buff);
			}
		} else if (fds[0].revents & POLLNVAL) {
				std::cout <<COLOR_BLUE<< "the served server disconnected!"<<COLOR_RESET<<std::endl;
				return ;
		}
	}
}

void	AsayadBot::run(std::string passW, std::string ip, int port) {
	sockaddr_in		sock;
	password	=	passW;

	bzero(&sock, sizeof sock);
	inet_aton(ip.c_str(), &sock.sin_addr);
	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);
	CreatSock();
	int ret = connect(Botfd, (sockaddr *)&sock, sizeof sock);
	if (-1 == ret)
		throw std::runtime_error("connect syscall failed: "+std::string(strerror(errno)) + CNS);
	// if (fcntl(Botfd, F_SETFL, O_NONBLOCK) == -1)
	// 	throw std::runtime_error(std::string("fcntl syscall: ")+strerror(errno));
	login();
	event_loop();
}
