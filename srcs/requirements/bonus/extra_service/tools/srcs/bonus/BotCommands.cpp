#include <asayadBot.hpp>


void	AsayadBot::usmell(std::string rec) {
	std::istringstream	strm(rec);
	// std::string			sender(rec);

	strm >> rec;
	if (!(strm >> rec)) return;
	std::string rsp(":Bot PRIVMSG " + rec);
	
	rsp += USMELLRSP;
	send_response(rsp.c_str(), Botfd);
}

void	AsayadBot::qra(std::string rec) {
	std::istringstream strm(rec);

	strm >> rec;
	if (!(strm >> rec)) return;
	std::string rsp(":Bot PRIVMSG " + rec);
	
	rsp += " :ayoo " + rec + ", ";
	rsp += QRAWZIQRA;
	send_response(rsp.c_str(), Botfd);
}

void	AsayadBot::noisy(std::string rec) {
	std::istringstream strm(rec);

	strm >> rec;
	if (!(strm >> rec)) return;
	std::string rsp(":Bot PRIVMSG " + rec);
	
	rsp += " :Asalamualaikum " + rec + ", ";
	rsp += NOISE;
	send_response(rsp.c_str(), Botfd);
}

void	AsayadBot::help(std::string rec) {

	rec = rec.substr(0, rec.find("!") != std::string::npos ? rec.find("!") : rec.size());
	std::string rsp(":Bot PRIVMSG " + rec);
	
	rsp += " :Hello " + rec + ", ";
	rsp += HLP;
	send_response(rsp.c_str(), Botfd);
}
