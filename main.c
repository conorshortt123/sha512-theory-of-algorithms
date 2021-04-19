#include <stdio.h>
#include "sha512.c"

int main(int argc, char *argv[]) {

    // File pointer for reading.
    FILE *f;

    // Open file from command line for reading.
    if(argv[1]) {
        f = fopen(argv[1], "r");
        // Calculate the SHA512 of f.
        sha512(f);
        // Close the file.
        fclose(f);
    }
    else
        printf("Please provide a file input as an argument.\n");

    return 0;
}