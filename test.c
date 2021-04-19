#include <stdio.h>
#include <assert.h>
#include "sha512.c"

int main(int argc, char *argv[]) {

    // SHA512 of string 'abc' as calculated from: https://abunchofutils.com/u/computing/sha512-hash-calculator/
    const char* expected = "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f";

    // File pointer for reading.
    FILE *f;
    // Open file from command line for reading.
    f = fopen(argv[1], "r");

    printf("Expected SHA512 hash:\n%s", expected);
    printf("\nResult SHA512 hash:\n");

    // Calculate the SHA512 of f.
    const char* result = sha512(f);

    // Assert expected and result string, if not equal program will bomb out with an error.
    assert(strcmp(result, expected) == 0);

    // Close the file.
    fclose(f);

    

    return 0;
}