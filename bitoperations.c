#include <stdio.h>

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

int main(int argc, char *argv[])
{

    // Set i to a literal value, Hex value of 241.
    unsigned int i = 0x0f0f0f0f;

    // Print original value in binary.
    printf("Original:\t");
    bin_print(i);

    printf("\t%x\t%u\n\n", i, i);

    // 32
    int j = sizeof(unsigned int) * 8;

    for (j--; j >= 0; j--)
    {
        // 1 shifted left j times.
        bin_print(1 << j); printf("\n");
        // i.
        bin_print(i); printf("\n");
        printf("-------------------------------- &\n");
        // (1 shifted left j times) bitwise logical and i.
        bin_print((1 << j) & i); printf("\n\n");
    }

    return 0;
}
