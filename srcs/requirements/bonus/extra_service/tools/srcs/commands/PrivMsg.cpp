/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-amma <ael-amma@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:38:34 by mel-akar          #+#    #+#             */
/*   Updated: 2025/03/24 00:04:16 by ael-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include  <IrcServ.hpp>

// :info priv

void	Server::target_spec(const std::vector<std::string>& vec, int fd)
{
	Client &client = *get_client(fd);
	std::string senderInfo = ":" + client.get_nick() + "!" + client.getuser() + "@" + client.getIp();
	std::string response(senderInfo + " PRIVMSG ");

	if (vec[1][0] != '&' && vec[1][0] != '#') {
		int recFd = get_fd(vec[1]);
		response += get_nick(recFd);
		response += " :" + vec[2] + "\r\n";
		if (recFd != -1)
			send_response(response.c_str(), recFd);
	} else {
		Channel *channel = getChannel(vec[1]);
		if (channel)
		{
			response += " :" + vec[2] + "\r\n";
			channel->broadcast(response);
		}
	}
}

bool	Server::check_limit(std::string receivers) {

	size_t	pos = 0;
	while (pos != std::string::npos) {
		pos = receivers.find(",", pos);
		if (pos == std::string::npos)
			continue ;
		receivers =  receivers.replace(pos, 1, " ");
	}
	std::istringstream	insertor(receivers);
	size_t				counter(0);
	std::string			word;

	while (insertor >> word)
		counter++;

	return (counter != 1) ? true : false;
}

void	Server::privMsg(const std::vector<std::string>& vec, int _fd)
{
	std::string response;

	// there's some ambiguity in the error of norecp & nomsgtosend
	// gotta use the receiver limit error

	if (vec.size() < 2) {
		response += ERR_NEEDMOREPARAMS;
		response += std::string(" " + get_nick(_fd));
		response += " :need params given\r\n";
		return send_response(response.c_str(), _fd);
	} else if (vec.size() == 2 || vec[2] == "") {
		response += ERR_NOTEXTTOSEND;
		response += std::string(" " + get_nick(_fd));
		response += " :no message to send\r\n";
		return send_response(response.c_str(), _fd);
	} else if (check_limit(vec[1])) {
		response += ERR_TOOMANYTARGETS;
		response += std::string(" " + get_nick(_fd));
		response += PRIVLIMITERR;
		return send_response(response.c_str(), _fd);
	} else if (vec[1][0] == '#' || vec[1][0] == '&') {
		if (!getChannel(vec[1])) {
			response += ERR_NORECIPIENT;
			response += std::string(" " + get_nick(_fd));
			response += " :No such channel <" + std::string(vec[1]+">\r\n");
			return send_response(response.c_str(), _fd);
		}
	}
	else if (false == nicksearch(vec[1]) || !is_reg(get_fd(vec[1]))) {
		// should check for the recp prefix &
		response += ERR_NORECIPIENT;
		response += std::string(" " + get_nick(_fd));
		response += " :no such recipient <" + std::string(vec[1]+">\r\n");
		return send_response(response.c_str(), _fd);
	}
	target_spec(vec, _fd);
}