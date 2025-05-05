#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>
#include <cstdlib>
#include <netdb.h>
#include <iostream>
#include <sys/socket.h>
#include <exception>
#include <errno.h>
#include <cstring>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <algorithm>
#include <limits.h>
#include <sys/un.h>
#include <fstream>
#include <wait.h>

// # define SOCK_PATH "./redisServer.sock"

void	remove_client(int fd, std::vector<pollfd>& fds) {
	for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); it++) {
		if (it->fd == fd)  {
			close(it->fd);
			fds.erase(it);
			return ;
		}
	}
}

void	execute(std::string cmd) {
	if (!cmd.empty() && cmd.back() == '\n') cmd.pop_back();

	if (cmd == "run redis") {
		pid_t pid = fork();
		if (pid == -1)
			throw std::runtime_error("fork failed!");
		else if (pid == 0) {
			execv("./redising", 0);
			exit(1);
		}
		else
			waitpid(pid, 0, 0);
	} else
		return ;
}

int main () {
	int s_fd(-1), c_fd(-1), optva(1);
	struct sockaddr_in	addr;
	char 				buff[512];
	ssize_t				rd_bytes(0), nullTerm(0);
	std::vector<pollfd>	fds;
	pollfd				tmp;
	
	close(0);close(1); close(2);

	int stds = open("/dev/null", O_CREAT | O_RDWR , 0777); dup(stds);dup(stds);
	bzero(buff, sizeof addr); bzero(&addr, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.s_addr = 0;


	std::ofstream		logFile("./redis_script.log", std::ios::out | std::ios::app); 
	logFile << "begin session: <--this file is used by redis script to log output-->\n\n\n\n";logFile.flush();
	// strncpy(addr.sun_path, SOCK_PATH, strlen(SOCK_PATH));
	try {
		if ((s_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			throw std::runtime_error("Socket syscall failed: in server part");
		if (setsockopt(s_fd, SOL_SOCKET, SO_REUSEADDR, &optva, sizeof optva) == -1)
			throw std::runtime_error("setsockopt failed");
		if (fcntl(s_fd, F_SETFL, O_NONBLOCK) == -1)
			throw std::runtime_error("fcntl syscall failed");
		tmp.events = POLLIN; tmp.fd = s_fd; tmp.revents = 0; fds.push_back(tmp);
		// if (remove(SOCK_PATH) == -1 && errno != ENOENT)
		// 	throw std::runtime_error("remove failed!");
		if (bind(s_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
		 	throw std::runtime_error("bind failed!");
		if (listen(s_fd, SOMAXCONN) == -1)
			throw std::runtime_error("listen failed!");
		for (;;)  {

			if (poll(&fds[0], fds.size(), -1) == -1)
				throw std::runtime_error("poll err!");
			for (size_t i(0); i < fds.size(); i++) {
				if (fds[i].revents & POLLIN) {
					if (fds[i].fd == s_fd) {
						c_fd = accept(s_fd, 0, 0);
						if (c_fd == -1)
							throw std::runtime_error("accept error!");
						if (fcntl(c_fd, F_SETFL, O_NONBLOCK) == -1)
							throw std::runtime_error("fcntl syscall failed");
						logFile << "client connected with fd = " << fds[i].fd << std::endl; logFile.flush();
						tmp.fd = c_fd;
						fds.push_back(tmp);
					} else {
						rd_bytes = recv(fds[i].fd, buff, 512, 0);
						if (rd_bytes == -1)
							throw std::runtime_error("recv failed!");
						if (rd_bytes == 0) {
							remove_client(fds[i].fd, fds); continue ;
						}
						buff[rd_bytes] = 0;
						logFile << "client output fds[i].fd = " << fds[i].fd << "\n" << buff << std::endl; logFile.flush();
						execute(std::string(buff));
					}
				}
			}
		}

	} catch (const std::exception& x) {
		logFile << "error occured: " << std::endl; return 1;
	}
	
}