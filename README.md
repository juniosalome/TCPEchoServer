# TCPEchoServer

## Project

An TCP echo server-client.

## Dev

- Ubuntu 22.04.2 LTS
- vscode
- C++

## Objective

- The client will send a Login Request initially.
- The server will check if the login as following:
- Username and password is valid send a Login Response with status code OK.
- Login Response with status code FAILED, and disconnect client.
- The client will send Echo Request with variable sized ciphered message.
- The server shall get message in the Echo Request and decipher it and then send to client as Echo Response.

## About the project

- Create a TCP Echo Server-Client c++ with Login Request {req_id:0, username, password}, Login Response {req_id:0, status}, Echo Request {req_id:0, message}, Echo Response {req_id:0, message}

- Cipher and Decryption of Echo Request Message

- Server uses Thread

- ServerMulti uses IO socket multiplexing

- Message Serialization/Deserialization

- Server continuously receive messages from the client. If the received message is equal to "exit," the server can close the connection with that client.

- user: "testuser" password: "testpass" (modify in includes/constants.h)
