#include "mysocket.h"
#include "rsa.h"
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    system("clear");
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server running in port %i\n", PORT);
    // Generate public and private keys
    struct RSAKeys key_pair = generate_key_pair();
    printf("\nPublic key: %s\nPrivate key: %s\n", key_pair.public, key_pair.private);
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Receive the client public key
    valread = read(new_socket, buffer, 1024);
    char *client_public_key = malloc(strlen(buffer) + 1);
    strcpy(client_public_key, buffer);
    memset(buffer, 0, sizeof buffer);
    printf("Client public key: %s\n", client_public_key);

    // Send public key
    send(new_socket, key_pair.public, strlen(key_pair.public), 0);
    printf("Public key sent!\n");

    // Receive message
    valread = read(new_socket, buffer, 1024);
    char *decrypted_message = rsa_decrypt(buffer, key_pair.private);
    printf("\nClient message: %s\n", buffer);
    printf("Client message (decrypted): %s\n", decrypted_message);

    // Send response message
    char *message = "hello from server";
    char *message_encrypted = rsa_encrypt(message, client_public_key);
    send(new_socket, message_encrypted, strlen(message_encrypted), 0);
    printf("\nServer message: %s\n", message);
    printf("Server message (encrypted): %s\n", message_encrypted);

    free(client_public_key);
    return 0;
}