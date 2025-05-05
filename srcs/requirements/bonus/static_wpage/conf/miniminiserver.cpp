#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>

int main() {

	// 3ala bab sima
    int server = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == server)
		return 1;
    sockaddr_in addr = {AF_INET, htons(8080), INADDR_ANY};
	if (bind(server, (sockaddr*)&addr, sizeof(addr)) == -1)
		return 2;
    if (listen(server, 1) == -1)
		return 3;

	// this shit not ircing hhhhh
    const char* html =
        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
        "<!DOCTYPE html><html><head><title>Hi</title></head>"
        "<body><h1>Inception Not inceptionning anymore!</h1></body></html>";

    while (true) {
        int client = accept(server, 0, 0);
        send(client, html, strlen(html), 0);
        close(client);
    }
}
