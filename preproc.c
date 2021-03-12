#include <stdio.h>
#include <inttypes.h>

#define BYTE uint8_t

void bin_print(BYTE i)
{
    // Number of bits in an int
    int j = sizeof(BYTE) * 8;

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
    BYTE b;

    // Project: error checks
    FILE *f;
    // Project: verify args
    f = fopen(argv[1], "rb");

    size_t numbytes;

    numbytes = fread(&b, 1, 1, f);
    while (numbytes)
    {
        bin_print(b);
        numbytes = fread(&b, 1, 1, f);
    }
    fclose(f);
    printf("\n");
    

    return 0;
}