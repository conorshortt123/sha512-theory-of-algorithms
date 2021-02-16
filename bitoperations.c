#include <stdio.h>

void bin_print(int i) {
    // Number of bits in an int
    int j = sizeof(int) * 8;

    // Temp var
    int k;

    // Figure out what this does: & = bitwise AND, ? = Ternary operator, like an IF statement.
    for(j-- ;j >= 0; j--) {
        k = ((1 << j) & i) ? 1 : 0;
        printf("%d", k);
    }
}

int main(int argc, char *argv[]) {

    int i = 241;

    printf("Original:  ");
    bin_print(i);
    printf("\n");

    for(int j = 0; j < 40; j++) {
        printf("%3d << %2d: ", i, j);
        bin_print(i << j);
        printf("\n");
    }

    return 0;
}
