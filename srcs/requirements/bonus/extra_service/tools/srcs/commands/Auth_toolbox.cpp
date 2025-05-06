/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Auth_toolbox.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-akar <mel-akar@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-19 10:17:58 by mel-akar          #+#    #+#             */
/*   Updated: 2025-03-19 10:17:58 by mel-akar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IrcServ.hpp>

void	Server::nickleftover(Client& client) {
	// for
	for (size_t i(0); i < clients.size(); i++) {
		bool samenick = clients[i].get_nick() == client.get_nick();
		if (samenick && clients[i].get_fd() != client.get_fd()) {
			clients[i].set_nick("*");
		}
	}
}

bool	Server::nicksearch(const std::string& nick) {

	for (size_t i(0); i < clients.size();i++) {
		if (nick == clients[i].get_nick()) {
			return true;
		}
	}
	return false;
}

bool		Server::is_auth(int fd) {
	for (size_t i(0); i < clients.size(); i++) {
		if (clients[i].get_fd() == fd)
			return clients[i].get_auth() ? true : false;
	}
	return false;
}

bool		Server::is_reg(int fd) {
	for (size_t i(0); i < clients.size(); i++) {
		if (clients[i].get_fd() == fd)
			return clients[i].get_reg() ? true : false;
	}
	return false;
}

bool	Server::set_nick(const std::string& nick, int fd) {

	for (size_t i(0); i < clients.size(); i++) {
		if (clients[i].get_fd() == fd) {
			clients[i].set_nick(nick);
		}
	}
	return true;
}

bool	Server::set_user(const std::string& user, int fd) {

	for (size_t i(0); i < clients.size(); i++) {
		if (clients[i].get_fd() == fd) {
			clients[i].set_user(user);
		}
	}
	return true;
}

bool	Server::is_reg(const std::string& nick) {
	for (size_t i(0); i < clients.size(); i++) {
		if (clients[i].get_nick() == nick)
			return clients[i].get_reg() ? true : false;
	}
	return false;
}