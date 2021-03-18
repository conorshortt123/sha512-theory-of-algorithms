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
    uint16_t sixf[8];
};

enum Status
{
    READ,
    PAD,
    END
};

// Get the next block.
int next_block(FILE *f, union Block *B, enum Status *S, uint64_t *nobits)
{
    // Number of bytes to read
    size_t numbytes;

    if (*S == END)
    {
        return 0;
    }
    else if (*S == READ)
    {
        // Try to read 64 bytes.
        numbytes = fread(B->bytes, 1, 64, f);
        // Calculate the total bits read so far.
        *nobits = *nobits + (8 * numbytes);
        // Enough room for padding.
        if (numbytes == 64)
        {
            return 1;
        }
        else if (numbytes <= 55)
        {
            // Append 1 bit (and 7 bits to make a full byte.)
            B->bytes[numbytes] = 0x80; // in bits 1000000
            // Append enough 64 bits, leaving 0 at the end.
            while (numbytes++ < 64)
            {
                B->bytes[numbytes] = 0x00; // in bits 00000000
            }
            // Append length of original input (Check endianess).
            B->sixf[7] = *nobits;
            // Say this is the last block
            *S = END;
        }
        else
        {
            // Reached the end of the input message, not enough room in this block
            // for all the padding. Append a 1 bit and seven 0 bits to make a full byte.
            B->bytes[numbytes] = 0x80;
            // Append 0 bits.
            while (numbytes++ < 64)
            {
                B->bytes[numbytes] = 0x00; // in bits 00000000
            }
            // Change the status to PAD.
            *S = PAD;
        }
    }
    else if (*S == PAD)
    {
        numbytes = 0;
        // Append 0 bits.
        while (numbytes++ < 64)
        {
            B->bytes[numbytes] = 0x00; // in bits 00000000
        }
        // Append nobits as an integer.
        B->sixf[7] = *nobits;
        // Change the status to PAD.
        *S = END;
    }

    return 0;
}

int main(int argc, char *argv[])
{

    union Block B;
    // Total number of bits read

    uint64_t nobits = 0;

    // Iterator
    int i;

    // Current Status
    enum Status S = READ;

    // File pointer for reading - Project: error checks
    FILE *f;
    // Open from command line for reading - Project: verify args
    f = fopen(argv[1], "rb");

    while (next_block(f, &B, &S, &nobits))
    {
        for (i = 0; i < 16; i++)
        {
            printf("%08" PF " ", B.words[i]);
        }
        printf("\n");
    }
    // Close file.
    fclose(f);
    printf("Total bits read is %d\n", nobits);

    return 0;
}