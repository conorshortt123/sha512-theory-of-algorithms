#include <stdio.h>
#include <inttypes.h>

// Defining constants, WORD is a 32-bit integer value, PF is a constant
// for the formatted printing of 32-bit integers.
#define WORD uint32_t
#define PF PRIX32

// Logical function used in Secure Hash Algorithms, where x, y and z and 32-bit integers.
// Formula = Ch(x, y, z) = (x & y) ^ (~x & z)
// & = Bitwise AND operator
// ^ = Bitwise XOR operator (exclusive OR)
// ~ = Bitwise NOT operator (inverts all bits)
WORD Ch(WORD x, WORD y, WORD z) {
    return (x & y) ^ (~x & z);
}

int main(int argc, char *argv[]) {

    WORD x = 0x0F0F0F0F;
    WORD y = 0x0A0A0A0A;
    WORD z = 0x0B0B0B0B;

    WORD ans = Ch(x, y, z);

    printf("Ch(%08" PF ",%08" PF ",%08" PF ")=%08" PF "\n", x, y, z, ans);

    return 0;
}