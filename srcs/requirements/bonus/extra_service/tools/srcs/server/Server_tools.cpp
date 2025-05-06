/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_tools.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-amma <ael-amma@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:51:37 by mel-akar          #+#    #+#             */
/*   Updated: 2025/03/23 15:14:25 by ael-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IrcServ.hpp>

const std::string	Server::get_nick(int fd) {
	for (size_t i(0); i < clients.size(); i++) {
		if (clients[i].get_fd() == fd) {
			return clients[i].get_nick();
		}
	}
	return std::string("");
}

void	Server::NotRegMsg(int fd) {
	std::string response;
	
	if (is_auth(fd)) {
		response = ERR_NOTREGISTERED;
		response += " :You have not registered (register using NICK command than USER)\r\n";
		send_response(response.c_str(), fd);
	} else {
		response = ERR_NOTREGISTERED;
		response += " :You have not registered enter the server's password\r\n";
		send_response(response.c_str(), fd);
	}
}

void	Server::check_state(int fd, std::string& cmd) {
		if (!is_reg(fd)) {
			NotRegMsg(fd);
		} else {
			std::string response = ERR_UNKNOWNCOMMAND;
			response += " ";
			response += cmd;
			response += " :Unknown command\r\n";
			send_response(response.c_str(), fd);
		}
}

std::vector<std::string>	ft_split(std::string str) {
	std::istringstream			strm(str);
	std::vector<std::string>	vec;
	std::string					cmd;
	size_t						skipper(0);


	strm >> cmd;
	if (cmd[0] == ':') {
		skipper = cmd.length(); strm >> cmd;
		skipper = skipper > 0 ? --skipper : skipper;
	}
	vec.push_back(cmd);
	while (strm >> cmd) {
			if (cmd[0] == ':') {
				size_t offset = str.find(':', skipper);
				if (offset != std::string::npos)
					str = str.substr(offset + 1);
				if (str.size() > 0 && str[str.size() - 1] == '\n') {
					str.erase(str.size() - 1);
				}
				if (str.size() > 0 && str[str.size() - 1] == '\r')
					str.erase(str.size() - 1);
				vec.push_back(str);
				break;
			}
			vec.push_back(cmd);
	}
	return vec;	
}


Client*			Server::get_client(int fd) {
	for (size_t i(0); i < clients.size(); i++) {
		if (fd == clients[i].get_fd())
			return &clients[i];
	}
	return NULL;
}

void	remove_client(int fd) {
	for (size_t i(0); i < Server::clients.size(); i++) {
		if (fd == Server::clients[i].get_fd()) {
			Server::clients.erase(Server::clients.begin() + i);
		}
	} for  (size_t i(0); i < Server::fds.size(); i++) {
		if (Server::fds[i].fd == fd) {
			Server::fds.erase(Server::fds.begin() + i);
		}
	}
	close(fd);
}

void	Server::Signal_Handler(int _signal) {
	if (_signal == SIGINT || SIGQUIT == _signal)
		_sig = true;
}

void	Server::signal_handling()
{
	signal(SIGPIPE, Signal_Handler);
	signal(SIGINT, Signal_Handler);
	signal(SIGQUIT, Signal_Handler);
}

int	Server::get_fd(const std::string& nick) {

	for (size_t i(0); i < clients.size(); i++) {
		if (clients[i].get_nick() == nick)
			return clients[i].get_fd();
	}
	return -1;
}

std::string	to_lower(std::string& cmd) {
	std::string str(cmd);
	for (size_t i(0); i < str.size(); i++) {
		if (isupper(str[i]))
			str[i] += 32;
	}
	return str;
}
