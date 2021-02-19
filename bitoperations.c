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

// Logical function used in Secure Hash Algorithms, where x, y and z and 64-bit integers.
// Referencing: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
void Ch(int64_t x, int64_t y, int64_t z)
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

    // Size of 64-bit int
    int j = sizeof(int64_t) * 8;

    printf("In binary:\t");
    // Loop over the number of bits in i (4 * 8)
    for (j--; j >= 0; j--)
    {
        // Picks out the j^th bit of i, evaluates as true if it's a 1 else evaluates as 0.
        int k = ((1 << j) & ch) ? 1 : 0;
        // Print k
        printf("%d", k);
    }

    printf("\n");
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

    printf("In binary:\t");
    // Loop over the number of bits in i (4 * 8)
    for (j--; j >= 0; j--)
    {
        // Picks out the j^th bit of i, evaluates as true if it's a 1 else evaluates as 0.
        int k = ((1 << j) & maj) ? 1 : 0;
        // Print k
        printf("%d", k);
    }

    printf("\n");
}

// The rotate right (circular right shift) operation, where x is a w-bit word
// and n is an integer (number of rotations) with 0 <= n < w
// Referencing: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
void ROTR(int n, int64_t x)
{
    // Formula = ROTR^n(x) = (x >> n) | (x << w - n).

    printf("\nROTR function: a logical function used in secure hash algorithms:\n");
    printf("Given input (x, n) and evaluating with ROTR^n(x) = (x >> n) | (x << w - n)\n");
    printf("Where w is the number of bits in x, and n is the number of rotations\n\n");

    // Get ROTR(x)
    for(int i = 0; i <= n; i++)
    {   
        int64_t rotr_x = (x >> i) | (x << (sizeof(x) * 8) - i);

        // Print binary of ROTR(x)
        int j = sizeof(int64_t) * 8;
        for (j--; j >= 0; j--)
        {
            // Picks out the j^th bit of i, evaluates as true if it's a 1 else evaluates as 0.
            int k = ((1 << j) & rotr_x) ? 1 : 0;
            // Print k
            printf("%d", k);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    // // Set i to a literal value, Hex value of 241.
    // unsigned int i = 0x0f0f0f0f;

    // // Print original value in binary.
    // printf("Original:\t");
    // bin_print(i);

    // printf("\t%x\t%u\n\n", i, i);

    // // 32
    // int j = sizeof(unsigned int) * 8;

    // for (j--; j >= 0; j--)
    // {
    //     // 1 shifted left j times.
    //     bin_print(1 << j);
    //     printf("\n");
    //     // i.
    //     bin_print(i);
    //     printf("\n");
    //     printf("-------------------------------- &\n");
    //     // (1 shifted left j times) bitwise logical and i.
    //     bin_print((1 << j) & i);
    //     printf("\n\n");
    // }

    int64_t x = 0x1234F6789C011121LL;
    int64_t y = 0x9586912385591238LL;
    int64_t z = 0x1293756091714057LL;

    printf("\nx = \t0x%" PRIX64 "\n", x);
    printf("y = \t0x%" PRIX64 "\n", y);
    printf("z = \t0x%" PRIX64 "\n", z);

    Ch(x, y, z);
    Maj(x, y, z);

    ROTR(16, x);

    printf("\n");

    return 0;
}
