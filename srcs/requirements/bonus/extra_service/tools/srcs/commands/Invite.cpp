/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-amma <ael-amma@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 04:37:13 by ael-amma          #+#    #+#             */
/*   Updated: 2025/03/24 00:13:49 by ael-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServ.hpp"

//	INVITE <nickname> <channel>	
void			Server::inviteCh( const std::vector<std::string>& vec, int _fd )
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
	chName = vec[2];
	if (chName[0] != '#' && chName[0] != '&')
		return(send_response(std::string(ERR_NOSUCHCHANNEL + nick + " " + chName + " :No such channel\r\n" ).c_str(), _fd));

	//	CHECK THE CHANNEL EXIST
	channel = getChannel(chName);
	if (!channel)
		return(send_response(std::string(ERR_NOSUCHCHANNEL + nick + " " + chName + " :No such channel\r\n" ).c_str(), _fd));

	//	CHECK IF THE CHANNEL IS INVITE-ONLY
	if (channel->getModes().find('i') == std::string::npos)
		return ;

	//	CHECK THE CLIENT IS AN OPERATOR
	if (!channel->isOP(nick))
		return(send_response(std::string(ERR_CHANOPRIVSNEEDED + nick + " " + chName + " :You're not channel operator\r\n").c_str(), _fd));

	channel->setInvite(vec[1]);
	send_response(std::string(RPL_INVITING + nick + " " + vec[1] + " " + chName + "\r\n").c_str(), _fd);
	send_response(std::string(":" + nick + "!" + client->get_user() + "@" + client->get_ip() + " PRIVMSG " + vec[1] + " :You have been invited to " + chName + " by " + nick + "\r\n").c_str(), get_fd(vec[1]));
}
