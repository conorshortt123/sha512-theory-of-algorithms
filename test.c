#include <stdio.h>
#include "sha512.c"

int test(int argc, char *argv[]) {

    // File pointer for reading.
    FILE *f;
    // Open file from command line for reading.
    f = fopen(argv[1], "r");
    
    // Calculate the SHA512 of f.
    sha512(f);

    // Close the file.
    fclose(f);

    return 0;
}