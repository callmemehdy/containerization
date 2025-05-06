/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Auth.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-amma <ael-amma@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:15:22 by mel-akar          #+#    #+#             */
/*   Updated: 2025/03/23 15:13:15 by ael-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IrcServ.hpp>

// tools for commands

void	Server::passAth(const std::vector<std::string>& vec, int fd)
{
	std::string response;
	
	if (is_auth(fd)) {
		response = ERR_ALREADYREGISTRED;
		response += " * :You may not reregister\r\n";
		send_response(response.c_str(), fd);
	} else if (vec.size() < 2) {
		response = ERR_NEEDMOREPARAMS;
		response += " * PASS :not enough parameters\r\n";
		send_response(response.c_str(), fd);
	} else {
		if (vec[1] != password) {
			response = ERR_PASSWDMISMATCH;
			response += " * :incorrect password\r\n";
			send_response(response.c_str(), fd);
		} else {
			for (size_t i(0); i < clients.size(); i++) {
				if (clients[i].get_fd() == fd)
					clients[i].make_auth();
			}
		}
	}
}

void	Server::userAth(const std::vector<std::string>& vec, int fd)
{
	std::string response;

	// i should print the nickname if i have it in the reregister case
	if (is_auth(fd))
	{
		if (is_reg(fd)) {
			response = ERR_ALREADYREGISTRED;
			response += std::string(" " + get_nick(fd));
			response += " :You may not reregister\r\n";
			send_response(response.c_str(), fd);
		} else if (vec.size() < 5 /*|| vec[3] == "" */) {
			response = ERR_NEEDMOREPARAMS;
			response += " USER";
			response += " :Not enough parameters\r\n";
			send_response(response.c_str(), fd);
		} else if (get_nick(fd) == "*")  {
			response = ERR_NONICKNAMEGIVEN;
			response += " * :no nickname given (USE NICK FIRST)\r\n";
			send_response(response.c_str(), fd);
		} else {
			if (set_user(vec[1], fd)) {
				// gotta set the other info & send the welcoming set
				Client& cl  = *get_client(fd);	cl.set_real(vec[4]);
				response = RPL_WELCOME; nickleftover(cl);
				response += std::string(" " + get_nick(fd)) + " :Welcome to the FT_IRC42 Network";
				response += ", " + std::string(get_nick(fd) + "\r\n");
				send_response(response.c_str(), fd);
			}
		}
	} else {
			response = ERR_NOTREGISTERED;
			response += " :You have not registered enter the server's password\r\n";
			send_response(response.c_str(), fd);
	}
}	

bool	Server::nickname_chars(const std::string& nick) {
	std::string ValidChars("_-[]\\`^{}");

	for (size_t i(0); i < nick.size(); i++) {
		if (!std::isalnum(nick[i]) && ValidChars.find(nick[i]) == std::string::npos)
			return false;
	}
	return true;
}

void	Server::nickAth(const std::vector<std::string>& vec, int fd)
{
	std::string response;

	if (is_auth(fd))
	{
		if (vec.size() < 2 || vec[1] == "") {
			response = ERR_NONICKNAMEGIVEN;
			response += " * :no nickname param\r\n";
			send_response(response.c_str(), fd);
		} else if (this->nicksearch(vec[1]) && is_reg(vec[1])) {
			response = ERR_NICKNAMEINUSE;
			response += std::string(" " + get_nick(fd));
			response += " :nickname in use\r\n";
			send_response(response.c_str(), fd);
		} else if (!nickname_chars(vec[1]) || vec[1].length() > 9) {
			response = ERR_ERRONEUSNICKNAME;
			response += " * ";
			response += vec[1] + " :invalid nickname\r\n";
			send_response(response.c_str(), fd);
		} else
			set_nick(vec[1], fd);
	} else {
			response = ERR_NOTREGISTERED;
			response += " :You have not registered enter the server's password\r\n";
			send_response(response.c_str(), fd);
	}
}
