#include "client.h"   


// Main function for the TCP Echo Client
int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &(server_address.sin_addr));
    bool resp = true;

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error connecting to the server" << std::endl;
        return 1;
    }

    uint32_t message_sequence = 87;
    uint8_t username_sum = 0;
    uint8_t password_sum = 0;

    // Attempt login until successful
    while (!login(client_socket, message_sequence, username_sum, password_sum)) {
        // Retry login
    }

    // Perform encrypted echo
    while (resp){
        resp = echo(client_socket, message_sequence, username_sum, password_sum);
    }


    close(client_socket);

    return 0;
}




