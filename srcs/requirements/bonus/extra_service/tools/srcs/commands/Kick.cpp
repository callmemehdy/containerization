/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-amma <ael-amma@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:36:35 by ael-amma          #+#    #+#             */
/*   Updated: 2025/03/24 00:03:30 by ael-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServ.hpp"

//	KICK <channel> <user> [<comment>]
void			Server::kickCh( const std::vector<std::string>& vec, int _fd )
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
	if (vec.size() < 3)
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
	if (!channel->isOP(nick))
		return(send_response(std::string(ERR_CHANOPRIVSNEEDED + nick + " " + chName + " :You're not a channel operator\r\n").c_str(), _fd));

	std::string	user = vec[2];
	if (nick != user && channel->isUserInChannel(user))
	{
		if (vec.size() > 3)
			channel->broadcast(std::string(":" + nick + "!" + client->get_user() + "@" + client->get_ip() + " KICK " + vec[1] + " " + user + " :" + vec[3] + "\r\n"));
		else
			channel->broadcast(std::string(":" + nick + "!" + client->get_user() + "@" + client->get_ip() + " KICK " + vec[1] + " " + user + "\r\n"));
		channel->kickUser(user);
	}
}
