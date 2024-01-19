#include "server.h"

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    uint8_t username_sum = 0;
    uint8_t password_sum = 0;
    uint32_t message_sequence = 87;

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    listen(server_socket, 5);

    std::cout << " Server IO socket multiplexing" << std::endl;
    std::cout << "Server listening on port " << PORT << "..." << std::endl;
    std::cout << "Use word 'exit' on client side for close connection " << std::endl;

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(server_socket, &read_fds);

    std::vector<int> client_sockets;

    while (true) {
        fd_set temp_fds = read_fds;
        int max_fd = server_socket;

        for (int client_socket : client_sockets) {
            FD_SET(client_socket, &temp_fds);
            max_fd = std::max(max_fd, client_socket);
        }

        int activity = select(max_fd + 1, &temp_fds, nullptr, nullptr, nullptr);

        if (activity < 0) {
            std::cerr << "Error in select" << std::endl;
            break;
        }

        if (FD_ISSET(server_socket, &temp_fds)) {
            // New connection
            int client_socket = accept(server_socket, nullptr, nullptr);
            client_sockets.push_back(client_socket);
            FD_SET(client_socket, &read_fds);

            std::cout << "New connection established. Client socket: " << client_socket << std::endl;
        }

        for (auto it = client_sockets.begin(); it != client_sockets.end();) {
    int client_socket = *it;

    if (FD_ISSET(client_socket, &temp_fds)) {
        // Handle login and echo requests
        if (!handleLoginRequest(client_socket, message_sequence, username_sum , password_sum )) {
            // Login failed or connection closed
            std::cout << "Login failed or connection closed. Client socket: " << client_socket << std::endl;
            close(client_socket);
            FD_CLR(client_socket, &read_fds);
            it = client_sockets.erase(it);  // Remove current client_socket
            continue;
        }

        while (handleEchoRequest(client_socket, message_sequence, username_sum , password_sum )) {
            // Continue handling echo requests until "exit" is received
        }

        std::cout << "close connection. Client socket: " << client_socket << std::endl;
        close(client_socket);
        FD_CLR(client_socket, &read_fds);
        it = client_sockets.erase(it);  // Remove current client_socket
    } else {
        ++it;  // Move to the next client_socket
    }
}
    }

    // Close all client sockets
    for (int client_socket : client_sockets) {
        close(client_socket);
    }

    close(server_socket);

    return 0;
}