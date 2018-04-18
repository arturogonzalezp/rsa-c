#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysocket.h"
#include "rsa.h"
#define PORT 8080
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char hello[MAX_MESSAGE_LENGTH];
    char buffer[1024] = {0};

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

    
    printf("message> ");
    printf("\n");
    /*fgets (hello, MAX_MESSAGE_LENGTH, stdin);
    send(sock, hello, strlen(hello), 0);
    printf("Hello message sent: %s\n", hello);*/
    struct RSAKeys key_pair = generate_key_pair();
    printf("Public key: %i\nPrivate key: %i\n",key_pair.public,key_pair.private);

    char * tempMessage = encrypt("Hola server");
    send(sock,tempMessage,strlen(tempMessage),0);
    printf("Message sent: %s\n", tempMessage);
    
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);
    return 0;
}