#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysocket.h"
#include "rsa.h"
#define PORT 8080
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    struct sockaddr_in serv_addr;
    int sock = 0;
    int valread;
    char buffer[1024] = {0};
    system("clear");
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    printf("Connected to server\n");
    // Generate public and private keys
    struct RSAKeys key_pair = generate_key_pair();
    printf("\nPublic key: %s\nPrivate key: %s\n", key_pair.public, key_pair.private);

    // Send public key
    send(sock, key_pair.public, strlen(key_pair.public), 0);
    printf("Public key sent!\n");

    // Receive server public key
    valread = read(sock, buffer, 1024);
    char *server_public_key = malloc(strlen(buffer) + 1);
    strcpy(server_public_key, buffer);
    memset(buffer, 0, sizeof buffer);
    printf("Server public key: %s\n", server_public_key);

    // Input message
    printf("message> ");
    char message[MAX_MESSAGE_LENGTH];
    fgets(message, MAX_MESSAGE_LENGTH, stdin);
    strtok(message, "\n");

    // Send message
    char *message_encrypted = encrypt(message, server_public_key);
    send(sock, message_encrypted, strlen(message_encrypted), 0);
    printf("Client message: %s\n", message);
    printf("Client message (encrypted): %s\n", message_encrypted);

    // Receive response from server
    valread = read(sock, buffer, 1024);
    printf("\nServer message: %s\n", buffer);
    printf("Server message (decrypted): %s\n", decrypt(buffer, key_pair.private));

    free(server_public_key);
    return 0;
}