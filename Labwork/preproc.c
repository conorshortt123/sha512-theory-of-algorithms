#include <stdio.h>
#include <inttypes.h>

// Defining constants, WORD is a 32-bit integer value, PF is a constant
// for the formatted printing of 32-bit integers.
#define WORD uint64_t
#define PF PRIx64
#define W 64
#define BYTE uint8_t

// SHA256 works on blocks of 512 bits.
union Block
{
    BYTE bytes[128];
    WORD words[16];
    uint64_t sixf[16];
};

// Acts as a flag.
enum Status
{
    READ,
    PAD,
    END
};

// Returns 1 if it created a new block from original message or padding.
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
        // Try to read 128 bytes.
        numbytes = fread(B->bytes, 1, 128, f);
        // Calculate the total bits read so far.
        *nobits = *nobits + (8 * numbytes);
        // Enough room for padding.
        if (numbytes == 128)
        {
            return 1;
        }
        else if (numbytes < 120)
        {
            // Append 1 bit (and 7 bits to make a full byte.)
            B->bytes[numbytes] = 0x80; // in bits 1000000
            // Append enough 64 bits, leaving 0 at the end.
            for (numbytes++; numbytes < 120; numbytes++) {
                B->bytes[numbytes] = 0x00; // in bits 00000000
            }
            // Append length of original input (Check endianess).
            B->sixf[15] = *nobits;
            // Say this is the last block
            *S = END;
        }
        else
        {
            // Reached the end of the input message, not enough room in this block
            // for all the padding. Append a 1 bit and seven 0 bits to make a full byte.
            B->bytes[numbytes++] = 0x80;
            // Append 0 bits.
            for (numbytes++; numbytes < 128; numbytes++) {
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
        for (numbytes = 0; numbytes < 120; numbytes++) {
            B->bytes[numbytes] = 0x00; // in bits 00000000
        }
        // Append nobits as an integer.
        B->sixf[15] = *nobits;
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
            printf("%016" PF " ", B.words[i]);
        }
        printf("\n");
    }
    
    // Close file.
    fclose(f);
    printf("Total bits read is %ld\n", nobits);

    return 0;
}