/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-amma <ael-amma@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 11:32:36 by mel-akar          #+#    #+#             */
/*   Updated: 2025/03/23 15:20:12 by ael-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IrcServ.hpp>

bool							Server::_sig;
std::vector<struct pollfd>		Server::fds;
std::vector<Client>				Server::clients;
std::map<std::string, Channel>	Server::channels;

Server::Server() : s_fd(-1), s_port(SPORT), password("anas123") {}

Server::~Server() {
	for (size_t in(0); in < fds.size(); in++) {
		close(fds[in].fd);
	}
	// clean
	std::cout << COLOR_RED <<"IRC SERVER CLOSED!" << COLOR_RESET << std::endl;
}

void	Server::set_port(int port) {s_port = port;}

void	Server::set_pass(std::string pass) {password = pass;}

void	Server::set_fd(int fd) {
	if (fd >= 0) {
		this->s_fd = fd;
	} else {
		throw std::runtime_error("set_fd: fd is out of the valid range");
	}
}

void		Server::create_sock() {
	int optva = 1;
	struct pollfd pfd;

	if ((s_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("Socket syscall failed: in server part");
	if (fcntl(s_fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl syscall failed");
	if (setsockopt(s_fd, SOL_SOCKET, SO_REUSEADDR, &optva, sizeof optva) == -1)
		throw std::runtime_error("setsockopt failed");
	// if (setsockopt(s_fd, SOL_SOCKET, SO_REUSEPORT, &optva, sizeof optva) == -1)
	// 	throw std::runtime_error("setsockopt SO_REUSEPORT failed");
	pfd.fd = s_fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	fds.push_back(pfd);
}

void		Server::bind_sock() {
	struct sockaddr_in	add;
	
	memset(&add, 0, sizeof add);
	add.sin_family = AF_INET;
	add.sin_addr.s_addr = INADDR_ANY;
	add.sin_port = htons(s_port);
	if (bind(s_fd, (sockaddr *)&add, sizeof add) == -1)
		throw std::runtime_error(std::string("bind syscall: ") + strerror(errno));
}

void		Server::listening() {
	if (listen(s_fd, SOMAXCONN) == -1) {
		throw std::runtime_error("listen syscall failed");			
	}
}

void	send_response(const char *buff, int fd) {
		// gotta handle the partial sends by putting this send inside a loop
		int ret = send(fd, buff, strlen(buff), 0);
		if (ret == -1) {
			// if (errno == EPIPE || errno == ECONNRESET) {
				std::cout << COLOR_BLUE<< fd \
						<< "FD client disconnected" \
						<< COLOR_RESET<< std::endl;
				return remove_client(fd);
			// }
			// if (errno == EAGAIN || errno == EWOULDBLOCK) return ;
			// throw std::runtime_error("send syscall failed");
		}
}

bool	Server::recv_request(int fd) {
	int			ret;
	std::string	str;
	char		buff[MAXREQLEN + 1];
	
	bzero(buff, MAXREQLEN + 1);
	ret = recv(fd, buff, MAXREQLEN, 0);
	if (0 == ret) {
		std::cout << COLOR_BLUE<< fd \
				  << "FD client disconnected" \
				  << COLOR_RESET<< std::endl;
		remove_client(fd);
		return false;
	}
	else if (-1 == ret) {
		// if (errno == ECONNRESET/* || errno == EPIPE*/) {
			std::cout << COLOR_BLUE<< fd \
					<< "FD client disconnected" \
					<< COLOR_RESET<< std::endl;
			return remove_client(fd), true;
		// }
		// if (errno == EAGAIN || errno == EWOULDBLOCK) return true;
		throw std::runtime_error("recv failed");
	}
	(buff[ret] = 0) , str = buff;
	if (str.back() != '\n') {
		Client&			eofChecker	= *get_client(fd);
		std::string&	inbuff		= eofChecker.get_buff();
		if ((inbuff.size() + str.size()) > MAXREQLEN) {
			inbuff = "";
			std::cerr << "the irc standard request maxsize=512 dont exceed that size\n";
			return true;
		}
		inbuff += str;
	} else {
		Client&			eofChecker	= *get_client(fd);
		std::string&	inbuff		= eofChecker.get_buff();
		if ((inbuff.size() + str.size()) >= MAXREQLEN) {
			inbuff = "";
			std::cerr<< "the irc standard request maxsize=512 dont exceed that size\n";
			return true;
		}
		inbuff += str;
		std::vector<std::string> lines = multiple_cmds(inbuff);
		for (size_t len(0); len < lines.size() && still_alive(fd); len++)
			handle_request(lines[len], fd);
		inbuff = "";
	}
	return true;
}

void	Server::accept_connect() {
	struct pollfd		pfd;

	// client's info vars
	struct sockaddr_in	cl_add;
	socklen_t			sock_len = sizeof(cl_add);
	Client				cl;

	char 				ip_str[INET_ADDRSTRLEN];
	
	bzero(&pfd, sizeof pfd);
	int newsock = accept(s_fd, (sockaddr *)&cl_add, &sock_len);
	if (newsock == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) return;
		throw std::runtime_error("accept syscall failed");
	}
	if (fcntl(newsock, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("fcntl syscall failed: in acceptFunc");
	if (inet_ntop(AF_INET, &cl_add.sin_addr, ip_str, INET_ADDRSTRLEN) == NULL)
		throw std::runtime_error("inet_ntop failed");
	cl.set_fd(newsock);
	cl.set_ip(ip_str);
	std::cout	<< COLOR_YELLOW \
				<< "new client accepted, with IpAdd: "<< \
		ip_str	<< COLOR_RESET<<std::endl;
	pfd.fd = newsock;
	pfd.events = POLLIN;
	pfd.revents = 0;
	fds.push_back(pfd);
	clients.push_back(cl);
}

void	Server::run() {
	IRC42_PATTERN;

	std::cout << COLOR_GREEN<<\
	"\t\t THE SERVER'S PORT: "<<\
	s_port <<COLOR_RESET<< std::endl;
	create_sock();
	bind_sock();
	listening();
	signal_handling();
	// have to print some information on the server prompt like wait for connection
	// 
	while (false == _sig) {
		if (poll(&fds[0], (nfds_t)fds.size(), -1) == -1 && !_sig)
			throw std::runtime_error("poll: " + std::string(strerror(errno)));
		// if (false == _sig == false) break ; 
		for (size_t i = 0; i < fds.size(); i++) {
			if (fds[i].revents & POLLIN) {
				// check revents
				if (fds[i].fd == s_fd) {
					accept_connect();
				} else
					recv_request(fds[i].fd);
			} else if (fds[i].revents & POLLNVAL) {
				if (fds[i].fd != s_fd) {
					std::cout << COLOR_BLUE << fds[i].fd \
							<< "FD client disconnected" \
							<< COLOR_RESET<< std::endl;
					remove_client(fds[i].revents);
				}
			}
		}
	}
}

Channel*	Server::isChannel( const std::string &channelName )
{
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end())
	{
		channels[channelName] = Channel(channelName);
		return (&channels[channelName]);
	}
	return (&it->second);
}

Channel*	Server::getChannel( const std::string &channelName )
{
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end())
		return NULL;
	return (&it->second);
}
