/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-amma <ael-amma@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:49:37 by mel-akar          #+#    #+#             */
/*   Updated: 2025/04/19 15:44:18 by ael-amma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IrcServ.hpp>

Channel::Channel() : name("Uknown"), limit(0) {}

Channel::Channel(std::string _name) : name(_name), limit(0) {}

Channel::~Channel() {}

const std::string&	Channel::getName() {
		return name;
}

const std::string&	Channel::getTopic()
{
	return topic;
}
const std::string&	Channel::getModes( void )
{
	return modes;
}

void				Channel::addUser(int fd, const Client& client)
{
	users[fd] = client;
}

void				Channel::addOp(int fd, const Client& client)
{
	op[fd] = client;
}

bool				Channel::isUserInChannel(const std::string& nick)
{
	std::map<int, Client>::iterator it;

	for(it = op.begin(); it != op.end(); it++)
	{
		if (it->second.get_nick() == nick)
			return true;
	}
	for(it = users.begin(); it != users.end(); it++)
	{
		if (it->second.get_nick() == nick)
			return true;
	}
	return false;
}

void				Channel::setKey(std::string str)
{
	key = str;
}

const std::string&	Channel::getKey(void)
{
	return (key);
}

const std::string	Channel::listUsers(void)
{
	std::string	res;

	for (std::map<int, Client>::iterator it = op.begin(); it != op.end(); it++)
	{
		if (!res.empty())
			res += " ";
		res += "@";
		res += it->second.get_nick();
	}
	for (std::map<int, Client>::iterator it = users.begin(); it != users.end(); it++)
	{
		if (!res.empty())
			res += " ";
		res += it->second.get_nick();
	}
	return (res);
}

std::map<int, Client>&	Channel::getUsers( void )
{
	return users;
}

bool					Channel::isInvited(const std::string& nick)
{
	return (invited.find(nick) != invited.end());
}

int						Channel::getLimit( void )
{
	return limit;
}

void					Channel::broadcast(const std::string& msg)
{
	std::map<int, Client>::iterator it;

	for(it = op.begin(); it != op.end(); it++)
		send_response(msg.c_str(), it->second.get_fd());
	for(it = users.begin(); it != users.end(); it++)
		send_response(msg.c_str(), it->second.get_fd());
}

bool					Channel::isNew( void )
{
	static int	flag;

	if (!flag)
	{
		flag++;
		return true;
	}
	return false;
}

bool					Channel::isOP( const std::string& nick )
{
	std::map<int, Client>::iterator it;

	for (it = op.begin(); it != op.end(); it++)
	{
		if (it->second.get_nick() == nick)
			return true;
	}
	return false;
}

void					Channel::setInvite(const std::string& nick)
{
	invited.insert(nick);
}

void					Channel::kickUser( const std::string& nick )
{
	std::map<int, Client>::iterator it;

	for (it = op.begin(); it != op.end(); it++)
	{
		if (it->second.get_nick() == nick)
		{
			op.erase(it);
			break ;
		}
	}
	for (it = users.begin(); it != users.end(); it++)
	{
		if (it->second.get_nick() == nick)
		{
			users.erase(it);
			break ;
		}
	}
	invited.erase(nick);
}

void					Channel::setTopic( const std::string& topic )
{
	this->topic = topic;
}

void					Channel::addMode( char mode )
{
	modes += mode;
}

void					Channel::rmMode( char mode )
{
	modes.erase(modes.find(mode), 1);
}
bool					Channel::isMode( char mode )
{
	if (modes.find(mode) != std::string::npos)
		return (true);
	return (false);
}

void					Channel::setLimit( int num )
{
	limit = num;
}

void					Channel::promoteUser( const std::string& nick )
{
	std::map<int, Client>::iterator it;

	for (it = users.begin(); it != users.end(); it++)
	{
		if (it->second.get_nick() == nick)
		{
			Client	client = it->second;
			users.erase(it);
			op[client.get_fd()] = client;
			break ;
		}
	}
}
