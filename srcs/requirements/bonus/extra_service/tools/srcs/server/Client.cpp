/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-amma <ael-amma@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 18:37:06 by mel-akar          #+#    #+#             */
/*   Updated: 2025/03/23 15:13:54 by ael-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IrcServ.hpp>

Client::Client() : c_fd(-1), ip_addr("unfound client"), isreg(false), authenticated(false), Climit(0)
{
	nickname = "*";
	username = "";
	real_name = "";
}

Client::Client(int fd, const std::string& ip) {
	if (fd > -1 && !ip.empty()) {
		c_fd = fd;
		ip_addr = ip;
		Climit = 0;
	} else {
		throw std::runtime_error("Client constructor: fd or ip is not valid");
	}
}

Client::~Client() {
	// close(c_fd);
}

void	Client::set_fd(int fd) {c_fd = fd;}

void	Client::set_real(const std::string& real) {real_name = real;}

void	Client::set_nick(const std::string& nick) {
	nickname = nick;
	if (username != "")
		isreg = true;
}

void	Client::set_user(const std::string& user) {
	username = user;
	if (nickname != "")
		isreg = true;
}

void	Client::set_ip(const std::string& ip) {
	ip_addr = ip;
}

void	Client::make_reg() {
	isreg = true;
}


void	Client::make_auth() {
	authenticated = true;
}

int		Client::get_fd() {
	return  c_fd;
}

const std::string&	Client::get_real() {return real_name;}

bool	Client::get_auth() {
	return authenticated;
}

bool	Client::get_reg() {
	return isreg;
}

const std::string&	Client::get_nick() {
	return nickname;
}

const std::string&	Client::get_user()
{
	return username;
}

const std::string&	Client::get_ip()
{
	return ip_addr;
}

std::string&	Client::get_buff() {
	return eof_buff;
}

const std::string&	Client::getIp() {
	return ip_addr;
}

const std::string&	Client::getuser() {
	return username;
}


int		Client::getCLimit()
{
	return  Climit;
}

void	Client::joinChannel(const std::string &channelName)
{
	channels.insert(channelName);
	Climit++;
}
