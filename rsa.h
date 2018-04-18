#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_PRIME_PRODUCT 600
#define SPACE_ASCII_NUMBER 32

struct RSAKeys
{
    char *public;
    char *private;
};
struct EncryptNumbers
{
    int e;
    int d;
};
struct RSAKeyTokens
{
    int key;
    int n;
};
struct RSAKeyTokens get_tokens(char *key);
char *rsa_encrypt(char *message, char *public_key);
char *rsa_decrypt(char *message, char *private_key);
int is_prime(int num);
struct RSAKeys generate_key_pair();
int generate_prime_number();
struct EncryptNumbers generate_encrypt_numbers(int totient);
int power(int base, unsigned int exp);