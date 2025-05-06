#include <asayadBot.hpp>

// gotta handle signalz again
// std::string					source;

std::string	to_lower(std::string& cmd) {
	std::string str(cmd);
	for (size_t i(0); i < str.size(); i++) {
		if (isupper(str[i]))
			str[i] += 32;
	}
	return str;
}

int main(int ac, char **av) {
	AsayadBot Bot;

	if (ac != 4) {
		std::cerr << ARGERR <<std::endl;
		return EXIT_FAILURE;
	} else {
		try {
			int port = std::atoi(av[1]);
			if (port < 1024 || port > 65535)
				throw std::runtime_error(PORTERROR);
			Bot.run(av[3], av[2], port);
		} catch (std::exception& exp) {
			std::cerr <<COLOR_RED<< "An error occured: " << exp.what() <<COLOR_RESET<< std::endl;
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
