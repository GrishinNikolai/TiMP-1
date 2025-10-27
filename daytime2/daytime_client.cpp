#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(13);
    server_addr.sin_addr.s_addr = inet_addr("172.16.40.1");

    int send_result = sendto(sockfd, "", 0, 0, 
                           (struct sockaddr*)&server_addr, 
                           sizeof(server_addr));
    
    if (send_result == -1) {
        std::cerr << "Ошибка отправки данных" << std::endl;
        close(sockfd);
        return 1;
    }

    char buffer[1024];
    socklen_t server_len = sizeof(server_addr);
    
    int recv_result = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                             (struct sockaddr*)&server_addr, &server_len);
    
    if (recv_result == -1) {
        std::cerr << "Ошибка приема данных" << std::endl;
        close(sockfd);
        return 1;
    }

    buffer[recv_result] = '\0';
    std::cout << "Время от сервера: " << buffer;

    close(sockfd);
    return 0;
}
