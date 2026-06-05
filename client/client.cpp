#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    std::string message;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("172.19.0.3");

    std::cout << "Enter message: ";
    std::getline(std::cin, message);

    sendto(sockfd, message.c_str(), message.length(), 0,
           (const struct sockaddr *)&server_addr, sizeof(server_addr));

    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
    buffer[n] = '\0';
    std::cout << "Echo: " << buffer << std::endl;

    close(sockfd);
    return 0;
}
