#include "rsa.h"
int gcd(int x, int y)
{
    int i, gcd;
    for (i = 1; i <= x && i <= y; ++i)
    {
        // Checks if i is factor of both integers
        if (x % i == 0 && y % i == 0)
            gcd = i;
    }
    return gcd;
}
int generate_prime_number()
{
    int n, prime, i = 3, count, c, upperBound = 50;
    srand(time(0));
    n = rand() % upperBound;
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
struct RSAKeys generate_key_pair()
{
    int prime1 = generate_prime_number();
    int prime2 = 0;
    while (prime2 == 0 || prime2 == prime1)
    {
        prime2 = generate_prime_number();
    }
    printf("Prime1: %i\nPrime2: %i\n",prime1,prime2);
    int primeProduct = prime1 * prime2;
    int totient = (prime1 - 1) * (prime2 - 1);

    struct RSAKeys key_pair;
    key_pair.public = 100;
    key_pair.private = 200;
    return key_pair;
}

char *encrypt(char *message)
{
    return message;
}