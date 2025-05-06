#include <asayadBot.hpp>


std::vector<std::string>	ft_split(std::string str) {
	std::istringstream			strm(str);
	std::vector<std::string>	vec;
	std::string					cmd;
	size_t						skipper(0);
	std::string					source;


	strm >> cmd;
	if (cmd[0] == ':') {
		source = cmd.substr(1);
		skipper = cmd.length(); strm >> cmd;
		skipper = skipper > 0 ? --skipper : skipper; 
	}
	vec.push_back(cmd);
	while (!(strm >> cmd).eof()) {
			if (cmd[0] == ':') {
				size_t offset = str.find(':', skipper);
				if (offset != std::string::npos)
					str = str.substr(offset + 1);
				if (str.size() > 0 && str[str.size() - 1] == '\n') {
					str.erase(str.size() - 1);
					if (str.size() > 0 && str[str.size() - 1] == '\r')
						str.erase(str.size() - 1);
				}
				vec.push_back(str);
				break;
			}
			vec.push_back(cmd);
	}
	!source.empty() ? vec.push_back(source) : (void)vec;
	return vec;	
}

AsayadBot::cmd		AsayadBot::enumerate(std::string str) 
{
	str = to_lower(str);
	str = str.substr(0, str.find(' '));
	return LINEEXCEED;
}
