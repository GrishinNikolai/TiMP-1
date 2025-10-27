#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

const int PORT = 7777;
const int BUFFER_SIZE = 1024;
const int MAX_QUEUE = 10;

int main() {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Ошибка привязки сокета" << std::endl;
        close(server_sock);
        return 1;
    }

    if (listen(server_sock, MAX_QUEUE) == -1) {
        std::cerr << "Ошибка перевода в режим прослушивания" << std::endl;
        close(server_sock);
        return 1;
    }

    std::cout << "Echo сервер запущен на порту " << PORT << std::endl;

    while (true) {
        struct sockaddr_in client_addr = {};
        socklen_t client_len = sizeof(client_addr);
        
        int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
        
        if (client_sock == -1) {
            std::cerr << "Ошибка принятия соединения" << std::endl;
            continue;
        }

        std::cout << "Новое соединение принято " << std::endl;

        char buffer[BUFFER_SIZE];
        while (true) {
           int bytes_received = recv(client_sock, buffer, BUFFER_SIZE - 1, 0);
            
            if (bytes_received <= 0) {
                break;
            }

            buffer[bytes_received] = '\0';
            
            std::cout << "Получено от клиента: " << buffer;
            
            int bytes_sent = send(client_sock, buffer, bytes_received, 0);
            if (bytes_sent == -1) {
                std::cerr << "Ошибка отправки данных" << std::endl;
                break;
            }
        }

        close(client_sock);
        std::cout << "Соединение закрыто" << std::endl;
    }

    close(server_sock);
    return 0;
}
