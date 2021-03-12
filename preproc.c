#include <stdio.h>
#include <inttypes.h>

// Defining constants, WORD is a 32-bit integer value, PF is a constant
// for the formatted printing of 32-bit integers.
#define WORD uint32_t
#define PF PRIX32
#define W 32
#define BYTE uint8_t

union Block
{
    BYTE bytes[64];
    WORD words[16];
};

int main(int argc, char *argv[])
{
    union Block B;

    // Total number of bits read
    uint64_t nobits = 0;
    // Iterator
    int i;

    // File pointer for reading - Project: error checks
    FILE *f;
    // Open from command line for reading - Project: verify args
    f = fopen(argv[1], "rb");

    // Number of bytes to read
    size_t numbytes;

    // Try to read 64 bytes
    numbytes = fread(B.bytes, 1, 64, f);
    // Print to console how many bytes were read.
    printf("Read %d bytes.\n", numbytes);
    // Update number of bits read
    nobits = nobits + (8 * numbytes);
    // Print the 16 32-bit words.
    for (i = 0; i < 16; i++)
    {
        printf("%08" PF " ", B.words[i]);
        if ((i + 1) % 8 == 0)
            printf("\n");
    }

    while (!feof(f))
    {
        numbytes = fread(B.bytes, 1, 64, f);
        printf("Read %d bytes\n", numbytes);
        nobits = nobits + (8 * numbytes);

        // Print the 16 32-bit words.
        for (i = 0; i < 16; i++)
        {
            printf("%08" PF " ", B.words[i]);
            if ((i + 1) % 8 == 0)
                printf("\n");
        }
    }
    // Close file.
    fclose(f);
    printf("\n");
    printf("Total bits read is %d\n", nobits);

    return 0;
}