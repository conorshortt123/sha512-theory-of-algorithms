#include <stdio.h>

void bin_print(unsigned int i) {
    // Number of bits in an int
    int j = sizeof(unsigned int) * 8;

    // Temp var
    int k;

    // Loop over the number of bits in i (4 * 8)
    for(j-- ;j >= 0; j--) {
        // 
        k = ((1 << j) & i) ? 1 : 0;
        // Print k
        printf("%d", k);
    }
}

int main(int argc, char *argv[]) {

    // Set i to a literal value, Hex value of 241.
    unsigned int i = 0xffffffff;

    // Print original value in binary.
    printf("Original:  ");
    bin_print(i);
    printf("\t%x\t%u\n", i, i);

    for(int j = 0; j < 40; j++) {
        // What the operation is.
        printf("%3u << %2d: ", i, j);
        // i in binary shifted left j times.
        bin_print(i << j);
        // End line
        printf("\n");
    }

    return 0;
}
