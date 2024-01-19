#include "server.h"

uint32_t next_key(uint32_t key) {
    return (key * 1103515245 + 12345) % 0x7FFFFFFF;
}


uint8_t calculateChecksum(const char* str) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < strlen(str); ++i) {
        checksum += static_cast<uint8_t>(str[i]);
    }
    return ~checksum; // Sum complement
}


void encryptMessage(char* message, uint32_t message_sequence, uint8_t username_sum, uint8_t password_sum) {
    uint32_t initial_key = (message_sequence << 16) | (username_sum << 8) | password_sum;
    uint32_t cipher_key = initial_key;

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        cipher_key = next_key(cipher_key);
        message[i] ^= cipher_key % 256;
    }
}


void decryptMessage(char* message, uint32_t message_sequence, uint8_t username_sum, uint8_t password_sum) {
    uint32_t initial_key = (message_sequence << 16) | (username_sum << 8) | password_sum;
    uint32_t cipher_key = initial_key;

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        cipher_key = next_key(cipher_key);
        message[i] ^= cipher_key % 256;
    }
}


bool handleLoginRequest(int client_socket, uint32_t& message_sequence, uint8_t& username_sum, uint8_t& password_sum) {
    LoginRequest loginRequest;
    char serializedLoginRequest[sizeof(LoginRequest)];
    ssize_t bytes_received = recv(client_socket, serializedLoginRequest, sizeof(LoginRequest), 0);

    if (bytes_received <= 0) {
        // Connection closed or error occurred
        return false;
    }
    loginRequest.deserialize(serializedLoginRequest);

    // Calculate checksums
    username_sum = calculateChecksum(loginRequest.username);
    password_sum = calculateChecksum(loginRequest.password);
    std::cout << " Username:" << loginRequest.username << " Password: " << loginRequest.password<< " client socket:" << client_socket << std::endl;

    bool loginStatus = (strcmp(loginRequest.username, USER) == 0) && (strcmp(loginRequest.password, PASS) == 0);

    LoginResponse loginResponse;
    loginResponse.req_id = loginRequest.req_id;
    loginResponse.status = loginStatus;
    send(client_socket, &loginResponse, sizeof(loginResponse), 0);

    // Update message_sequence for encryption of Echo Request
    message_sequence = next_key(message_sequence);

    return loginStatus;
}

bool handleEchoRequest(int client_socket, uint32_t message_sequence, uint8_t username_sum, uint8_t password_sum) {
    EchoRequest echoRequest;
    EchoResponse echoResponse;
    char serializedEchoRequest[sizeof(EchoRequest)];

    ssize_t bytes_received = recv(client_socket, serializedEchoRequest, sizeof(EchoRequest), 0);

    if (bytes_received <= 0) {
        // Connection closed or error occurred
        return false;
    }
    echoRequest.deserialize(serializedEchoRequest);

    // Decrypt the Echo Request message
    decryptMessage(echoRequest.message, message_sequence, username_sum, password_sum);

    echoResponse.req_id = echoRequest.req_id;

    // Encrypt the Echo Response message
    encryptMessage(echoResponse.message, message_sequence, username_sum, password_sum);

    send(client_socket, &echoResponse, sizeof(echoResponse), 0);

    // Check if the received message is "exit"
    if (strcmp(echoRequest.message, EXIT) == 0) {
        return false; // Client wants to exit, close connection
    }

    return true;
}