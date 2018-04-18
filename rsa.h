#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct RSAKeys {
    int public;
    int private;
};
char * encrypt();
struct RSAKeys generate_key_pair();
int generate_prime_number();
int gcd(int x, int y);