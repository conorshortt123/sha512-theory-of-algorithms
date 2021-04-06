#include <stdio.h>
#include <inttypes.h>

// Endianess. Adapted from:
//   https://developer.ibm.com/technologies/systems/articles/au-endianc/
#include <byteswap.h>
const int _i = 1;
#define islilend() ((*(char*)&_i) != 0)

// Words and bytes.
#define WORD uint64_t
#define PF PRIx64
#define BYTE uint8_t

// Page 5 of the secure hash standard.
#define ROTL(_x,_n) ((_x << _n) | (_x >> ((sizeof(_x)*8) - _n)))
#define ROTR(_x,_n) ((_x >> _n) | (_x << ((sizeof(_x)*8) - _n)))
#define SHR(_x,_n) (_x >> _n)

// Page 10 of the secure hash standard.
#define CH(_x,_y,_z) ((_x & _y) ^ (~_x & _z))
#define MAJ(_x,_y,_z) ((_x & _y) ^ (_x & _z) ^ (_y & _z))

#define SIG0(_x) (ROTR(_x,28) ^ ROTR(_x,34) ^ ROTR(_x,39))
#define SIG1(_x) (ROTR(_x,14)  ^ ROTR(_x,18) ^ ROTR(_x,41))
#define Sig0(_x) (ROTR(_x,1)  ^ ROTR(_x,8) ^ SHR(_x,7))
#define Sig1(_x) (ROTR(_x,19) ^ ROTR(_x,61) ^ SHR(_x,6))

// SHA512 works on blocks of 1024 bits.
union Block {
    // 8 x 128 = 1024 - dealing with block as bytes.
    BYTE bytes[128];
    // 64 x 16 = 1024 - dealing with block as words.
    WORD words[16];
    // 64 x 8 = 512 - dealing with the last 64 bits of last block.
    uint64_t sixf[16]; // might need to be 16
};

// For keeping track of where we are with the input message/padding.
enum Status {
    READ, PAD, END
};

// Section 4.2.3
const WORD K[] = {
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
    0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
    0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
    0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
    0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
    0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
    0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
    0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
    0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
    0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
    0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
    0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
    0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
    0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

// Returns 1 if it created a new block from original message or padding.
// Returns 0 if all padded message has already been consumed.
int next_block(FILE *f, union Block *M, enum Status *S, uint64_t *nobits) {

    // Number of bytes read.
    size_t nobytes;

    if (*S == END) {
        // Finish.
        return 0;
    } else if (*S == READ) {
        // Try to read 64 bytes from the input file.
        nobytes = fread(M->bytes, 1, 128, f);
        // Calculate the total bits read so far.
        *nobits = *nobits + (8 * nobytes);
        // Enough room for padding.
        if (nobytes == 128) {
            // This happens when we can read 64 bytes from f.
            // Do nothing.
        } else if (nobytes < 120) {
            // This happens when we have enough roof for all the padding.
            // Append a 1 bit (and seven 0 bits to make a full byte).
            M->bytes[nobytes] = 0x80; // In bits: 10000000.
            // Append enough 0 bits, leaving 64 at the end.
            for (nobytes++; nobytes < 128; nobytes++) {
                M->bytes[nobytes] = 0x00; // In bits: 00000000
            }
            // Append nobits as a big endian integer.
            M->sixf[15] = (islilend() ? bswap_64(*nobits) : *nobits);
            // Say this is the last block.
            *S = END;
        } else {
            // Got to the end of the input message and not enough room
            // in this block for all padding.
            // Append a 1 bit (and seven 0 bits to make a full byte.)
            M->bytes[nobytes] = 0x80;
            // Append 0 bits.
            for (nobytes++; nobytes < 128; nobytes++) {
                 // Error: trying to write to 
                M->bytes[nobytes] = 0x00; // In bits: 00000000
            }
            // Change the status to PAD.
            *S = PAD;
        }
    } else if (*S == PAD) {
        // Append 0 bits.
        for (nobytes = 0; nobytes < 120; nobytes++) {
            M->bytes[nobytes] = 0x00; // In bits: 00000000
        }
        // Append nobits as a big endian integer.
        M->sixf[15] = (islilend() ? bswap_64(*nobits) : *nobits);
        // Change the status to END.
        *S = END;
    }

    // Swap the byte order of the words if we're little endian.
    if (islilend())
        for (int i = 0; i < 16; i++)
            M->words[i] = bswap_32(M->words[i]);

    return 1;
}

int next_hash(union Block *M, WORD H[]) {
    
    // for (int i = 0; i < 8; i++)
    // {
    //     printf("H[%d] = 0x%" PF "\n", i, H[i]);
    // }

    // Message schedule, Section 6.4.2
    WORD W[80];
    // Iterator.
    int t;
    // Temporary variables.
    WORD a, b, c, d, e, f, g, h, T1, T2;

    // Section 6.4.2, part 1.
    for (t = 0; t < 16; t++)
        W[t] = M->words[t];
    for (t = 16; t < 80; t++)
        W[t] = Sig1(W[t-2]) + W[t-7] + Sig0(W[t-15]) + W[t-16];

    // Section 6.4.2, part 2.
    a = H[0]; b = H[1]; c = H[2]; d = H[3];
    e = H[4]; f = H[5]; g = H[6]; h = H[7];

    // Section 6.4.2, part 3.
    for (t = 0; t < 80; t++) {
        T1 = h + SIG1(e) + CH(e, f, g) + K[t] + W[t];
        T2 = SIG0(a) + MAJ(a, b, c);
        h = g; g = f; f = e; e = d + T1; d = c; c = b; b = a; a = T1 + T2;
    }

    // Section 6.4.2, part 4.
    H[0] = a + H[0]; H[1] = b + H[1]; H[2] = c + H[2]; H[3] = d + H[3];
    H[4] = e + H[4]; H[5] = f + H[5]; H[6] = g + H[6]; H[7] = h + H[7];

    return 0;
}


int sha512(FILE *f, WORD H[]) {
    // The function that performs/orchestrates the SHA256 algorithm on
    // message f.

    // The current block.
    union Block M;

    // Total number of bits read.
    uint64_t nobits = 0;

    // Current status of reading input.
    enum Status S = READ;

    // Loop through the (preprocessed) blocks.
    while (next_block(f, &M, &S, &nobits)) {
        next_hash(&M, H);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    // Section 5.3.5
    WORD H[] = {
        0x6a09e667f3bcc908,
        0xbb67ae8584caa73b,
        0x3c6ef372fe94f82b,
        0xa54ff53a5f1d36f1,
        0x510e527fade682d1,
        0x9b05688c2b3e6c1f,
        0x1f83d9abfb41bd6b,
        0x5be0cd19137e2179
    };

    // File pointer for reading.
    FILE *f;
    // Open file from command line for reading.
    f = fopen(argv[1], "r");

    // Calculate the SHA512 of f.
    sha512(f, H);

    // Print the final SHA512 hash.
    for (int i = 0; i < 8; i++)
        printf("%016" PF, H[i]);
    printf("\n");

    // Close the file.
    fclose(f);

    return 0;
}