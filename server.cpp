#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080  // Port to listen on

void handleClient(int clientSocket) {
    // Basic HTTP response
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello, World!";

    send(clientSocket, response, strlen(response), 0);
    close(clientSocket);
}

int main() {


    int serverFd, clientSocket;
    struct sockaddr_in address;
    int addrLen = sizeof(address);

    // Create socket
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    // Bind address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    // Start listening
    if (listen(serverFd, 10) < 0) {
        std::cerr << "Listen failed\n";
        return 1;
    }

    std::cout << "Server running on port " << PORT << "...\n";

    while (true) {
        clientSocket = accept(serverFd, (struct sockaddr *)&address, (socklen_t *)&addrLen);

        if (clientSocket < 0) {
            std::cerr << "Accept failed\n";
            continue;
        }

        handleClient(clientSocket);  // Handle request
    }

    close(serverFd);
    return 0;
}
