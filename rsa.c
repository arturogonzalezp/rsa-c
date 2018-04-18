#include "rsa.h"
int generate_prime_number()
{
    /*int prime_array[] = {2,3,5,7,11};
    prime = prime_array[n];*/
    int n, prime, i = 3, count, c, upperBound = 10;
    srand(time(NULL));
    n = rand() % upperBound + 1;
    if (n == 1)
    {
        prime = 2;
    }
    else
    {
        for (count = 2; count <= n;)
        {
            for (c = 2; c <= i - 1; c++)
            {
                if (i % c == 0)
                    break;
            }
            if (c == i)
            {
                prime = i;
                count++;
            }
            i++;
        }
    }
    return prime;
}
int is_prime(int num)
{
    if (num <= 1)
        return 0;
    if (num % 2 == 0 && num > 2)
        return 0;
    for (int i = 3; i < num / 2; i += 2)
    {
        if (num % i == 0)
            return 0;
    }
    return 1;
}
struct RSAKeyTokens get_tokens(char *key)
{
    int i = 0;
    struct RSAKeyTokens tokens;
    char *pch = strtok(key, "-");
    while (pch != NULL)
    {
        if (i == 0)
        {
            tokens.key = atoi(pch);
        }
        else if (i == 1)
        {
            tokens.n = atoi(pch);
        }
        i++;
        pch = strtok(NULL, "-");
    }
    return tokens;
}
struct EncryptNumbers generate_encrypt_numbers(int totient)
{
    int i = 0, e, d;
    struct EncryptNumbers numbers;
    numbers.e = 0;
    numbers.d = 0;

    while (numbers.e == 0 && numbers.d == 0)
    {
        i++;
        int temp = (totient * i) + 1;
        if (is_prime(temp) == 0)
        {
            /*for (e = temp - 1; e > 1; e--)
            {
                for (d = 2; d < e; d++)
                {
                    if (e * d == temp)
                    {
                        numbers.e = e;
                        numbers.d = d;
                        return numbers;
                    }
                }
            }*/
            for (e = 2; e < temp; e++)
            {
                for (d = e + 1; d < temp; d++)
                {
                    if (e * d == temp)
                    {
                        numbers.e = e;
                        numbers.d = d;
                        return numbers;
                    }
                }
            }
        }
    }
    return numbers;
}
struct RSAKeys generate_key_pair()
{
    int prime1 = generate_prime_number();
    int prime2 = 0;
    while (prime2 == 0 || prime2 == prime1)
    {
        prime2 = generate_prime_number();
    }
    int n = prime1 * prime2;
    int totient = (prime1 - 1) * (prime2 - 1);
    struct EncryptNumbers encrypt_numbers = generate_encrypt_numbers(totient);

    printf("\n(DEV) n: %i\n(DEV) prime1: %i\n(DEV) prime2: %i\n", n, prime1, prime2);
    struct RSAKeys key_pair;
    char *tempPublicKey = malloc(sizeof(char) * 20);
    char *tempPrivateKey = malloc(sizeof(char) * 20);
    sprintf(tempPublicKey, "%d-%d", encrypt_numbers.e, n);
    sprintf(tempPrivateKey, "%d-%d", encrypt_numbers.d, n);
    key_pair.public = tempPublicKey;
    key_pair.private = tempPrivateKey;
    return key_pair;
}

char *encrypt(char *message, char *public_key)
{
    // Get numbers
    struct RSAKeyTokens tokens = get_tokens(public_key);
    //printf("\nTokens\n(DEV) e: %i\n(DEV) n: %i\n", tokens.key, tokens.n);
    // Create encrypted message
    int i = 0;
    char *encrypted_message = malloc(strlen(message) * sizeof(char));
    for (i = 0; i < strlen(message); i++)
    {
        int m = message[i] - 'a' + 1;
        if (message[i] == ' ')
        {
            encrypted_message[i] = message[i];
        }
        else
        {
            unsigned long long int c = power(m, tokens.key);
            char newC = 'a' + (c % tokens.n) - 1;
            //printf("%c: %i, %llu | %c\n", message[i], m, c, newC);
            encrypted_message[i] = newC;
        }
    }
    //printf("\n");
    return encrypted_message;
}
char *decrypt(char *message, char *private_key)
{
    // Get numbers
    struct RSAKeyTokens tokens = get_tokens(private_key);
    //printf("\nTokens\n(DEV) d: %i\n(DEV) n: %i\n", tokens.key, tokens.n);
    // Create decrypted message
    int i;
    char *decrypted_message = malloc(strlen(message) * sizeof(char));
    for (i = 0; i < strlen(message); i++)
    {
        int c = message[i] - 'a' + 1;
        if (message[i] == ' ')
        {
            decrypted_message[i] = message[i];
        }
        else
        {
            unsigned long long int m = power(c, tokens.key);
            char newM = 'a' + (m % tokens.n) - 1;
            //printf("%c: %i | %llu | %c\n", message[i], c, m, newM);
            //printf("%c - %c\n",message[i],newM);
            decrypted_message[i] = newM;
        }
    }
    printf("\n");
    return decrypted_message;
}
int power(int base, unsigned int exp)
{
    int i, result = 1;
    for (i = 0; i < exp; i++)
        result *= base;
    return result;
}