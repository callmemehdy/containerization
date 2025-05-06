/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-akar <mel-akar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 21:32:22 by mel-akar          #+#    #+#             */
/*   Updated: 2025/03/08 01:05:03 by mel-akar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IrcServ.hpp>

// Verify absolutely every possible error and issue (receiving partial data, low bandwidth,
// and so forth).

static
bool	check_password(const std::string& password)
{
	std::istringstream	insertor(password);
	size_t				counter(0);
	std::string			word;


	if (!!password.empty())
		return true;
	while (insertor >> word)
		counter++;
	return (counter > 1  || counter < 1) ? true : false;
}


int main(int argc, char *argv[])
{
	Server		server;
	// gotta close all clients's fds and server too....
	if (argc != 3) {
		std::cerr << "Wrong number of args: Usage `"<<argv[0]<<" [Port] [Password]`\n";
		return 1;
	}
	try {
		int port = std::atoi(argv[1]);
		if (port < 1024 || port > 65535)
			throw std::runtime_error(PORTERROR);
		else if (check_password(std::string(argv[2])))
			throw std::runtime_error(PASSERROR);
		server.set_port(port);
		server.set_pass(argv[2]);
		server.run();

	} catch (const std::exception& exp) {
		std::cout << "error occured: " << exp.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}