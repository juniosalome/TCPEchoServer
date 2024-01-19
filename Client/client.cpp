#include "client.h"

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

    std::cout << "Initial Value: 0x" << std::hex << initial_key << std::dec << std::endl;
    std::cout << "Cipher Key (first 64 bytes):" << std::endl;

    for (int i = 0; i < 64; ++i) {
        cipher_key = next_key(cipher_key);
        std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(cipher_key % 256) << " ";
        if ((i + 1) % 16 == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}


void decryptMessage(char* message, uint32_t message_sequence, uint8_t username_sum, uint8_t password_sum) {
    uint32_t initial_key = (message_sequence << 16) | (username_sum << 8) | password_sum;
    uint32_t cipher_key = initial_key;

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        cipher_key = next_key(cipher_key);
        message[i] ^= cipher_key % 256;
    }
}

bool login(int client_socket, uint32_t& message_sequence, uint8_t& username_sum, uint8_t& password_sum) {
   
    LoginRequest loginRequest;
    loginRequest.req_id = 0;
    char serializedLoginRequest[sizeof(LoginRequest)];

    std::cout << " Username: "<< USER <<"   Password: "<< PASS  << std::endl;
    std::cout << "Enter username: ";
    std::cin.getline(loginRequest.username, 50);
    std::cout << "Enter password: ";
    std::cin.getline(loginRequest.password, 50);
    

    // Calculate checksums 
    username_sum = calculateChecksum(loginRequest.username);
    password_sum = calculateChecksum(loginRequest.password);

    loginRequest.serialize(serializedLoginRequest);
    send(client_socket, serializedLoginRequest, sizeof(loginRequest), 0);

    // Receive login response
    LoginResponse loginResponse;
    recv(client_socket, &loginResponse, sizeof(loginResponse), 0);

    // Check login status
    if (loginResponse.status) {
        std::cout << "Login successful!"<< loginResponse.status << std::endl;
        // Update message_sequence for encryption of Echo Request
        message_sequence = next_key(message_sequence);
        return true; 
    } else {
        std::cout << "Login failed. Please try again." << loginResponse.status << std::endl;
        return false; 
    }
}

bool echo(int client_socket, uint32_t message_sequence, uint8_t username_sum, uint8_t password_sum) {

    EchoRequest echoRequest;
    char serializedEchoRequest[sizeof(EchoRequest)];
    echoRequest.req_id = 0;
    std::cout << "Use word '" << EXIT <<"' for close connection " << std::endl;
    std::cout << "Enter message to echo: ";
    std::cin.getline(echoRequest.message, BUFFER_SIZE);

    // Encrypt the Echo Request message
    encryptMessage(echoRequest.message, message_sequence, username_sum, password_sum);

    echoRequest.serialize(serializedEchoRequest);
    send(client_socket, serializedEchoRequest, sizeof(EchoRequest), 0);

    // Receive encrypted echo response
    EchoResponse encryptedEchoResponse;
    recv(client_socket, &encryptedEchoResponse, sizeof(encryptedEchoResponse), 0);

    // Decrypt the Echo Response message
    decryptMessage(encryptedEchoResponse.message, message_sequence, username_sum, password_sum);

    if( strcmp(echoRequest.message, EXIT) == 0){
        return false;
    }
    return true;
}


