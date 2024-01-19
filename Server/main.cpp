#include "server.h"


// Main function for the TCP Echo Server
int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    listen(server_socket, 5);

    std::cout << " Server Thread" << std::endl;
    std::cout << "Server listening on port " << PORT << "..." << std::endl;
    std::cout << "Use word '" << EXIT <<"' on client side for close connection " << std::endl;

    std::vector<std::thread> client_threads;
    std::mutex client_threads_mutex;

    while (true) {
        int client_socket = accept(server_socket, nullptr, nullptr);

        std::lock_guard<std::mutex> lock(client_threads_mutex);
        client_threads.emplace_back(handleClient, client_socket);
    }

    // Join all client threads (shouldn't reach here in this simple example)
    for (std::thread& thread : client_threads) {
        thread.join();
    }

    // Close the server socket
    close(server_socket);

    return 0;
}
