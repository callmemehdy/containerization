/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_handling.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-amma <ael-amma@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:24:31 by mel-akar          #+#    #+#             */
/*   Updated: 2025/04/19 20:21:49 by ael-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IrcServ.hpp>


bool	Server::handle_auth(std::vector<std::string>& req, int fd) 
{

	if (req[0] == "PASS" || req[0] == "pass")
		return passAth(req, fd), true;
	else if (req[0] == "NICK" || req[0] == "nick")
		return nickAth(req, fd), true;
	else if (req[0] == "USER" || req[0] == "user")
		return userAth(req, fd), true;
	return false;
}

void	Server::handle_request(std::string buff, int fd) {
	std::vector<std::string> req;
	
	req	=	ft_split(buff);
	std::string cmd(to_lower(req[0]));
	if (handle_auth(req, fd)) return;
	switch ((int)is_reg(fd))
	{
		case true:
			if (cmd == "join")
				joinCh(req, fd);
			else if (cmd == "kick")
				kickCh(req, fd);
			else if (cmd == "invite")
				inviteCh(req, fd);
			else if (cmd == "topic")
				topicCh(req, fd);
			else if (cmd == "mode")
				modeCh(req, fd);
			else if (cmd == "privmsg")
				privMsg(req, fd);
			else
				check_state(fd, req[0]);
			break ;
		default :
			NotRegMsg(fd);
	}
}

std::vector<std::string>	Server::multiple_cmds(std::string commands)
{
	std::istringstream			strm(commands);
	std::vector<std::string>	vec;
	std::string					line;

	while (std::getline(strm, line)) {
// no need to check for the last line newline cause im sure it will contain a newline
		line += '\n';
		vec.push_back(line);
	}
	return vec;
}

bool	Server::still_alive(int fd) {
	for (size_t i(0); i < clients.size(); i++) {
		if (clients[i].get_fd() == fd)
			return true;
	}
	return false;
}


