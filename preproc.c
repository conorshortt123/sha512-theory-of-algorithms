#include <stdio.h>
#include <inttypes.h>

// Defining constants, WORD is a 32-bit integer value, PF is a constant
// for the formatted printing of 32-bit integers.
#define WORD uint32_t
#define PF PRIX32
#define W 32
#define BYTE uint8_t

// SHA256 works on blocks of 512 bits.
union Block
{
    // 8 x 64 = 512 - dealing with block as bytes
    BYTE bytes[64];
    // 32 x 16 = 512 - dealing with block as words
    WORD words[16];
    // 64 x 8 = 512 - dealing with the last 64 bits of last block.
    uint64_t sixf[8];
};

// Acts as a flag.
enum Status
{
    READ,
    PAD,
    END
};

// Returns 1 if it created a new block from original message or pdding.
// Returns 0 if all padded message has already been consumed.
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
        else if (numbytes < 56)
        {
            // Append 1 bit (and 7 bits to make a full byte.)
            B->bytes[numbytes] = 0x80; // in bits 1000000
            // Append enough 64 bits, leaving 0 at the end.
            for (numbytes++; numbytes < 56; numbytes++) {
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
            B->bytes[numbytes++] = 0x80;
            // Append 0 bits.
            for (numbytes++; numbytes < 64; numbytes++) {
                // Error: this line was trying to write to B->nobytes[64]
                B->bytes[numbytes] = 0x00; // in bits 00000000
            }
            // Change the status to PAD.
            *S = PAD;
        }
    }
    else if (*S == PAD)
    {
        // Append 0 bits.
        for (numbytes = 0; numbytes < 56; numbytes++) {
            B->bytes[numbytes] = 0x00; // in bits 00000000
        }
        // Append nobits as an integer.
        B->sixf[7] = *nobits;
        // Change the status to PAD.
        *S = END;
    }

    return 1;
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