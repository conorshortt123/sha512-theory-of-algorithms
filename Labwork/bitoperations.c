#include <stdio.h>
#include <inttypes.h>

void bin_print(unsigned int i)
{
    // Number of bits in an int
    int j = sizeof(unsigned int) * 8;

    // Temp var
    int k;

    // Loop over the number of bits in i (4 * 8)
    for (j--; j >= 0; j--)
    {
        // Picks out the j^th bit of i, evaluates as true if it's a 1 else evaluates as 0.
        k = ((1 << j) & i) ? 1 : 0;
        // Print k
        printf("%d", k);
    }
}

void bin_print_64(int64_t i)
{
    // Number of bits in a 64-bit int
    int j = sizeof(int64_t) * 8;

    // Temp var
    int k;

    // Loop over the number of bits in i (4 * 8)
    for (j--; j >= 0; j--)
    {
        // Picks out the j^th bit of i, evaluates as true if it's a 1 else evaluates as 0.
        k = ((1 << j) & i) ? 1 : 0;
        // Print k
        printf("%d", k);
    }
}

// Logical function used in Secure Hash Algorithms, where x, y and z and 64-bit integers.
// Referencing: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
int64_t Ch(int64_t x, int64_t y, int64_t z)
{
    // Formula = Ch(x, y, z) = (x & y) ^ (~x & z)
    // & = Bitwise AND operator
    // ^ = Bitwise XOR operator (exclusive OR)
    // ~ = Bitwise NOT operator (inverts all bits)

    printf("\nCh function: a logical function used in secure hash algorithms:\n");
    printf("Given input (x, y, z) and evaluating with Ch = (x & y) ^ (~x & z)\n");
    printf("Where x, y and z are 64-bit numbers and output is also a 64-bit number\n\n");

    int64_t ch = (x & y) ^ (~x & z);

    printf("Ch:\nIn decimal:\t%" PRId64 "\n", ch);
    printf("In hexidecimal:\t0x%" PRIX64 "\n", ch);

    return ch;
}

// Logical function used in Secure Hash Algorithms, where x, y and z and 64-bit integers.
// Referencing: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
void Maj(int64_t x, int64_t y, int64_t z)
{
    // Formula = Maj(x, y, z) = (x & y) ^ (x & z) ^ (y & z)

    printf("\nMaj function: a logical function used in secure hash algorithms:\n");
    printf("Given input (x, y, z) and evaluating with Maj = (x & y) ^ (x & z) ^ (y & z)\n");
    printf("Where x, y and z are 64-bit numbers and output is also a 64-bit number\n\n");

    int64_t maj = (x & y) ^ (x & z) ^ (y & z);

    printf("Maj:\nIn decimal:\t%" PRId64 "\n", maj);
    printf("In hexidecimal:\t0x%" PRIX64 "\n", maj);

    // Size of 64-bit int
    int j = sizeof(int64_t) * 8;

    // Print Maj in binary.
    printf("In binary:\t");
    bin_print_64(maj);

    printf("\n");
}

// The rotate right (circular right shift) operation, where x is a w-bit word
// and n is an integer (number of rotations) with 0 <= n < w
// Referencing: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
void ROTR(int n, int64_t x)
{
    // Formula = ROTR^n(x) = (x >> n) | (x << w - n).

    printf("\nROTR function: a logical function used in secure hash algorithms that rotates bits to the right:\n");
    printf("Given input (x, n) and evaluating with ROTR^n(x) = (x >> n) | (x << w - n)\n");
    printf("Where w is the number of bits in x, and n is the number of rotations\n\n");

    // Get ROTR(x)
    for (int i = 0; i <= n; i++)
    {
        int64_t rotr_x = (x >> i) | (x << (sizeof(x) * 8) - i);

        // Print binary of ROTR(x)
        bin_print_64(rotr_x);

        printf("\n");
    }
}

// The rotate left (circular left shift) operation, where x is a w-bit word
// and n is an integer (number of rotations) with 0 <= n < w
// Referencing: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
void ROTL(int n, int64_t x)
{
    // Formula = ROTL^n(x) = (x << n) | (x >> w - n)

    printf("\nROTL function: a logical function used in secure hash algorithms that rotates bits to the left:\n");
    printf("Given input (x, n) and evaluating with ROTL^n(x) = (x << n) | (x >> w - n)\n");
    printf("Where w is the number of bits in x, and n is the number of rotations\n\n");

    // Get ROTL(x)
    for (int i = 0; i <= n; i++)
    {
        int64_t rotl_x = (x << i) | (x >> (sizeof(x) * 8) - i);

        // Print binary of ROTL(x)
        bin_print_64(rotl_x);

        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    printf("%s %s %s", argv[1], argv[2], argv[3]);

    int64_t x;
    int64_t y;
    int64_t z;

    if (argc == 4)
    {
        x = (int64_t)argv[1];
        y = (int64_t)argv[2];
        z = (int64_t)argv[3];
    }
    else if (argc > 4)
    {
        printf("Too many arguments supplied.\n");
        return 0;
    }
    else if (argc > 1 && argc < 4)
    {
        printf("Arguments require 3 64-bit integers: x y z\n");
        return 0;
    }
    else
    {
        x = 0x1234F6789C011121LL;
        y = 0x9586912385591238LL;
        z = 0x1293756091714057LL;
    }

    printf("\nx = \t0x%" PRIX64 "\n", x);
    printf("y = \t0x%" PRIX64 "\n", y);
    printf("z = \t0x%" PRIX64 "\n", z);

    int64_t ch = Ch(x, y, z);
    printf("In binary:\t");
    bin_print_64(ch);
    printf("\n");

    Maj(x, y, z);
    ROTR(16, x);
    ROTL(16, x);

    printf("\n");

    return 0;
}
