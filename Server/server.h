#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdint>
#include <sys/select.h>
#include <vector>
#include <thread>
#include <mutex>
#include "../include/constants.h"

struct LoginRequest {
    int req_id;
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    // Serialization method
    void serialize(char* buffer) {
        memcpy(buffer, this, sizeof(LoginRequest));
    }

    // Deserialization method
    void deserialize(const char* buffer) {
        memcpy(this, buffer, sizeof(LoginRequest));
    }
};

struct LoginResponse {
    int req_id;
    bool status;

        // Serialization method
    void serialize(char* buffer) {
        memcpy(buffer, this, sizeof(LoginResponse));
    }

    // Deserialization method
    void deserialize(const char* buffer) {
        memcpy(this, buffer, sizeof(LoginResponse));
    }
};

struct EchoRequest {
    int req_id;
    char message[BUFFER_SIZE];

        // Serialization method
    void serialize(char* buffer) {
        memcpy(buffer, this, sizeof(EchoRequest));
    }

    // Deserialization method
    void deserialize(const char* buffer) {
        memcpy(this, buffer, sizeof(EchoRequest));
    }
};

struct EchoResponse {
    int req_id;
    char message[BUFFER_SIZE];

        // Serialization method
    void serialize(char* buffer) {
        memcpy(buffer, this, sizeof(EchoResponse));
    }

    // Deserialization method
    void deserialize(const char* buffer) {
        memcpy(this, buffer, sizeof(EchoResponse));
    }
};

// LCG variant for key generation
uint32_t next_key(uint32_t key);

// Function to calculate checksum (sum complement) of a string
uint8_t calculateChecksum(const char* str) ;

// Function to encrypt the message
void encryptMessage(char* message, uint32_t message_sequence, uint8_t username_sum, uint8_t password_sum) ;

// Function to decrypt the message
void decryptMessage(char* message, uint32_t message_sequence, uint8_t username_sum, uint8_t password_sum) ;

// Function to handle the login request from the client
bool handleLoginRequest(int client_socket, uint32_t& message_sequence, uint8_t& username_sum, uint8_t& password_sum) ;

// Function to handle the echo request from the client
bool handleEchoRequest(int client_socket, uint32_t message_sequence, uint8_t username_sum, uint8_t password_sum) ;

// Function to handle a client in a separate thread
void handleClient(int client_socket) ;


#endif //SERVER_H