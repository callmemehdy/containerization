/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-amma <ael-amma@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:37:48 by ael-amma          #+#    #+#             */
/*   Updated: 2025/04/19 11:28:28 by ael-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServ.hpp"

//	TOPIC <channel> [<topic>]
void			Server::topicCh( const std::vector<std::string>& vec, int _fd )
{
	std::string					nick;
	std::string					chName;
	Client*						client;
	Channel*					channel;

	client = get_client(_fd);
	if (!client)
		return ;
	nick = client->get_nick();

	//	PARAMS ERROR
	if (vec.size() < 2)
		return (send_response(std::string(ERR_NEEDMOREPARAMS2 + nick + " MODE :Not enough parameters\r\n").c_str(), _fd));

	//	CHECK THE VALIDITY OF THE CHANNEL NAME
	chName = vec[1];
	if (chName[0] != '#' && chName[0] != '&')
		return(send_response(std::string(ERR_NOSUCHCHANNEL + nick + " " + chName + " :No such channel\r\n" ).c_str(), _fd));

	//	CHECK THE CHANNEL EXIST
	channel = getChannel(chName);
	if (!channel)
		return(send_response(std::string(ERR_NOSUCHCHANNEL + nick + " " + chName + " :No such channel\r\n" ).c_str(), _fd));

	//	CHECK THE CLIENT IS AN OPERATOR
	if (vec.size() > 2 && channel->getModes().find('t') != std::string::npos && !channel->isOP(nick))
		return(send_response(std::string(ERR_CHANOPRIVSNEEDED + nick + " " + chName + " :You're not channel operator\r\n").c_str(), _fd));

	//	IF 2 ARGS SEND TOPIC ELSE SET TOPIC
	if (vec.size() > 2)
	{
		channel->setTopic(vec[2]);
		send_response(std::string(":" + nick + "!" + client->get_user() + "@" + client->get_ip() + " TOPIC " + chName + " :" + vec[2] + "\r\n").c_str(), _fd);
	}
	else
	{
		if (channel->getTopic().empty())
			send_response(std::string(RPL_NOTOPIC + chName + " :No topic is set\r\n").c_str(), _fd);
		else
			send_response(std::string(RPL_TOPIC + chName + " :" + channel->getTopic() + "\r\n").c_str(), _fd);
	}
}
