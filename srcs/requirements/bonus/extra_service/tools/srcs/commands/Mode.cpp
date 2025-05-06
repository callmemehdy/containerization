/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-amma <ael-amma@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:38:07 by ael-amma          #+#    #+#             */
/*   Updated: 2025/04/19 20:23:11 by ael-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServ.hpp"

//	MODE <channel> {[+|-]|i|t|k|o|l} [<key>] [<operator>] [<limit>]
void			Server::modeCh( const std::vector<std::string>& vec, int _fd )
{
	size_t						j = 1;
	Client*						client;
	Channel*					channel;
	std::string					chName;
	std::string					nick;

	client = get_client(_fd);
	if (!client)
		return ;
	nick = client->get_nick();

	//	PARAMS ERROR
	if (vec.size() < 3)
		return (send_response(std::string(ERR_NEEDMOREPARAMS2 + nick + " MODE :Not enough parameters\r\n").c_str(), _fd));

	//	CHECK THE VALIDITY OF THE CHANNEL NAME
	chName = vec[j++];
	if (chName[0] != '#' && chName[0] != '&')
		return(send_response(std::string(ERR_NOSUCHCHANNEL + nick + " " + chName + " :No such channel\r\n" ).c_str(), _fd));

	//	CHECK THE CHANNEL EXIST
	channel = getChannel(chName);
	if (!channel)
		return(send_response(std::string(ERR_NOSUCHCHANNEL + nick + " " + chName + " :No such channel\r\n" ).c_str(), _fd));

	//	CHECK THE CLIENT IS AN OPERATOR
	if (!channel->isOP(nick))
		return(send_response(std::string(ERR_CHANOPRIVSNEEDED + nick + " " + chName + " :You're not channel operator\r\n").c_str(), _fd));

	//	PARSING MODES
	bool								sign;
	std::string							token = vec[j++];
	std::vector<std::pair<bool, char> >	modes;

	for (size_t i = 0; token[i]; ++i) {
		if (token[i] == '+' || token[i] == '-') {
			sign = (token[i] == '+');
			continue ;
		} else
			modes.push_back(std::make_pair(sign, token[i]));
	}

	//	EVALUATE MODES
	for (size_t i = 0; i < modes.size(); ++i) {
		bool					sign = modes[i].first;
		char					mode = modes[i].second;
		std::string				reply;

		switch (mode) {
			case 'i':
				if (sign && !channel->isMode(mode)) {
					channel->addMode(mode);
					reply = ":" + nick + "!" + client->get_user() + "@" + client->get_ip() + " MODE " + chName + " +i\r\n";
				} else if (!sign && channel->isMode(mode)) {
					channel->rmMode(mode);
					reply = ":" + nick + "!" + client->get_user() + "@" + client->get_ip() + " MODE " + chName + " -i\r\n";
				}
				break ;
			case 't':
				if (sign && !channel->isMode(mode)) {
					channel->addMode(mode);
					reply = ":" + nick + "!" + client->get_user() + "@" + client->get_ip() + " MODE " + chName + " +t\r\n";
				} else if (!sign && channel->isMode(mode)) {
					channel->rmMode(mode);
					reply = ":" + nick + "!" + client->get_user() + "@" + client->get_ip() + " MODE " + chName + " -t\r\n";
				}
				break ;
			case 'k':
				if (j < vec.size()) {
					std::string	key = vec[j++];

					if (sign) {
						if (!channel->isMode(mode)) {
							channel->addMode(mode);
							channel->setKey(key);
						} else if (key != channel->getKey()) {
							reply = ERR_KEYSET + std::string( chName + ": " + chName + " Channel key already set\r\n");
							break ;
						}
						reply = ":" + nick + "!" + client->get_user() + "@" + client->get_ip() + " MODE " + chName + " +k " + key + "\r\n";
					} else if (!sign && channel->isMode(mode)) {
						channel->rmMode(mode);
						reply = ":" + nick + "!" + client->get_user() + "@" + client->get_ip() + " MODE " + chName + " -k " + key + "\r\n";
					}
				} else
					return (send_response(std::string(ERR_NEEDMOREPARAMS2 + nick + " MODE :Not enough parameters\r\n").c_str(), _fd));	
				break;
			case 'o':
				if (j < vec.size()) {
					std::string	user = vec[j++];
					if (sign && !channel->isMode(mode)) {
						if (!channel->isUserInChannel(user)) {
							reply = ERR_USERNOTINCHANNEL;
							reply += user;
							reply += " ";
							reply += chName;
							reply += " :They aren't on that channel\r\n";
							break ;
						} else if (!channel->isOP(user))
							channel->promoteUser(user);
						reply = ":" + nick + "!" + client->get_user() + "@" + client->get_ip() + " MODE " + chName + " +o " + user + "\r\n";
					} else if (!sign && channel->isMode(mode)) {
						channel->rmMode(mode);
						reply = ":" + nick + "!" + client->get_user() + "@" + client->get_ip() + " MODE " + chName + " -o " + user + "\r\n";
					}
				} else
					return (send_response(std::string(ERR_NEEDMOREPARAMS2 + nick + " MODE :Not enough parameters\r\n").c_str(), _fd));	
				break;
			case 'l':
				if (!sign && channel->isMode(mode)) {
					channel->rmMode(mode);
					channel->setLimit(INT_MAX);
					reply = ":" + nick + "!" + client->get_user() + "@" + client->get_ip() + " MODE " + chName + " -l\r\n"; 
				} else if (j < vec.size()) {
					long	limit = atol(vec[j].c_str());
					if (sign) {
						if (limit > INT_MAX || limit < INT_MIN)
							return (send_response(std::string(ERR_UNKNOWNMODE + std::string(std::string(1, mode) + " :Invalid parameter for +l mode\r\n")).c_str(), _fd));
						if (limit != channel->getLimit()) {
							channel->addMode(mode);
							channel->setLimit(limit);
							reply = ":" + nick + "!" + client->get_user() + "@" + client->get_ip() + " MODE " + chName + " +l " + vec[j++] + "\r\n"; 
						}
					}
				} else
					return (send_response(std::string(ERR_NEEDMOREPARAMS2 + nick + " MODE :Not enough parameters\r\n").c_str(), _fd));	
				break;
			default:
				reply = ERR_UNKNOWNMODE;
				reply += mode;
				reply += " :Is unknown mode char to me\r\n";
				break;
		}
		send_response(reply.c_str(), _fd);
	}
}
