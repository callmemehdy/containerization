/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-amma <ael-amma@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:02:52 by ael-amma          #+#    #+#             */
/*   Updated: 2025/04/19 20:21:40 by ael-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServ.hpp"

static std::vector<std::string>	splitc( const std::string &str, char delim )
{
	std::string					curr;
	std::vector<std::string>	tokens;
	std::istringstream			tokenStream(str);

	while (std::getline(tokenStream, curr, delim))
		tokens.push_back(curr);
	return (tokens);
}

//	JOIN <channel>{,<channel>} [<key>{,<key>}]
void	Server::joinCh( const std::vector<std::string>& vec, int _fd )
{
	Client*						client;
	std::string					nick;
	std::vector<std::string>	keys;
	std::vector<std::string>	channels;

	client = get_client(_fd);
	if (!client)
		return ;
	nick = client->get_nick();

	//	PARAMS ERROR
	if (vec.size() < 2)
		return (send_response(std::string(ERR_NEEDMOREPARAMS2 + nick + " JOIN :Not enough parameters\r\n").c_str(), _fd));

	//	SPLITING
	channels = splitc(vec[1], ',');
	if (vec.size() > 2)
		keys = splitc(vec[2], ',');

	for (size_t i = 0; i < channels.size(); i++)
	{
		std::string	chName(channels[i]);

		//	CHECK VALID CHANNEL PREFIX
		if (chName[0] != '&' && chName[0] != '#')
		{
			send_response(std::string(ERR_NOSUCHCHANNEL + nick + " " + chName + " :No such channel\r\n" ).c_str(), _fd);
			continue ;
		}

		//	CHECK ILLEGAL CHANNEL NAME
		if (chName.find(7) != std::string::npos || chName.size() > 200)
		{
			send_response(std::string(ERR_BADCHANNAME + nick + " " + chName + " :Illegal channel name\r\n").c_str(), _fd);
			continue ;
		}

		//	CHECK CHANNEL LIMIT TO JOIN FOR USER
		if (client->getCLimit() >= MAX_JOINCHAN)
			return (send_response(std::string(ERR_TOOMANYCHANNELS + nick + " " + chName + " :You have joined too many channels\r\n").c_str(), _fd));

		Channel	*channel = isChannel(chName);
		if (!channel->isUserInChannel(nick))
		{
			std::map<int, Client> users(channel->getUsers());

			//	CHECK USER IN INVITE-ONLY LIST
			if (channel->getModes().find('i') != std::string::npos && !channel->isInvited(nick))
			{
				send_response(std::string(ERR_INVITEONLYCHAN + nick + " " + chName + " :Invite only channel (+i)\r\n").c_str(), _fd);
				continue ;
			}

			//	CHECK USER LIMIT TO CHANNEL
			if (channel->getModes().find('l') != std::string::npos && users.size() < channel->getLimit())
			{
				send_response(std::string(ERR_CHANNELISFULL + nick + " " + chName + " :Channel is full (+l)\r\n").c_str(), _fd);
				continue ;
			}

			//	CHECK KEY
			if (channel->getModes().find('k') != std::string::npos)
			{
				if (i < keys.size() && channel->getKey() != keys[i])
				{
					send_response(std::string(ERR_BADCHANNELKEY + nick + " :Cannot join channel (+k)\r\n").c_str(), _fd);
					continue ;
				}
			}

			//	GIVE CHANNEL CREATOR OPERATOR ROLE
			client->joinChannel(nick);
			if (channel->isNew())
				channel->addOp(_fd, *client);
			else
				channel->addUser(_fd, *client);

			//	SEND A JOIN BROADCAST MESSAGE
			channel->broadcast(std::string(":" + nick + "!" + client->get_user() + "@" + client->get_ip() +  " JOIN " + chName + "\r\n").c_str());

			//	SEND CHANNEL'S MODES MESSAGE *RFC1459
			if (!channel->getModes().empty())
			{
				if (channel->getKey().empty())
					send_response(std::string(RPL_CHANNELMODEIS + nick + " " + chName + " +" + channel->getModes() + "\r\n").c_str(), _fd);
				else
					send_response(std::string(RPL_CHANNELMODEIS + nick + " " + chName + " +" + channel->getModes() + " " + channel->getKey() + "\r\n").c_str(), _fd);
			}

			//	SEND TOPIC MESSAGE *RFC1459
			if (!channel->getTopic().empty())
				send_response(std::string(RPL_TOPIC + chName + " :" + channel->getTopic() + "\r\n").c_str(), _fd);
			// else
			// 	send_response(std::string(RPL_NOTOPIC + chName + " :" + channel->getTopic() + "\r\n").c_str(), _fd);

			//	SEND LIST OF USERS IN THE CHANNEL *RFC1459
			send_response(std::string(RPL_NAMREPLY + nick + " @ " + chName + " :" + channel->listUsers() + "\r\n").c_str(), _fd);
			send_response(std::string(RPL_ENDOFNAMES + nick + " " + chName + " :End of /NAMES list\r\n").c_str(), _fd);
		}
	}
}
