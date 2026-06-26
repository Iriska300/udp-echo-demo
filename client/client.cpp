#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    struct hostent *server;
    std::string message;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    std::string server_host = "udp-echo-service";
    std::cout << "Resolving " << server_host << "..." << std::endl;
    
    server = gethostbyname(server_host.c_str());
    if (server == NULL) {
        std::cerr << "ERROR: No such host: " << server_host << std::endl;
        close(sockfd);
        return 1;
    }

    bcopy((char *)server->h_addr, 
          (char *)&server_addr.sin_addr.s_addr,
          server->h_length);

    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(server_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
    std::cout << "Server IP: " << ip_str << std::endl;

    int ping_count = 0;
    while (true) {
        ping_count++;
        message = "ping #" + std::to_string(ping_count);
        std::cout << "Sending: " << message << std::endl;

        if (sendto(sockfd, message.c_str(), message.length(), 0,
                   (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Send failed" << std::endl;
            break;
        }

        fd_set readfds;
        struct timeval tv;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        int ret = select(sockfd + 1, &readfds, NULL, NULL, &tv);
        if (ret > 0) {
            int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
            if (n > 0) {
                buffer[n] = '\0';
                std::cout << "Received: " << buffer << std::endl;
            }
        } else {
            std::cout << "Timeout - no response" << std::endl;
        }

        sleep(2);
    }

    close(sockfd);
    return 0;
}
