# Theory of Algorithms - Conor Shortt
The contents of this repository are for my fourth year module, *Theory of Algorithms*.
The task was to write a C program that calculates the [SHA512](https://www.nist.gov/publications/secure-hash-standard) value of any given input file. The program takes in the path of the file as a command line argument, and prints out the correct SHA512 digest.


## How to run:
Make sure you have [make](https://www.gnu.org/software/make/) installed, then navigate into the root directory of the cloned repository:
```
> make
```
This creates the executable file *main*. You will then type:
```
> ./main input.txt
```
The output of the program is a 128 character hexidecimal string, which translates to the 512-bit message as output from the algorithm.


## Testing the program:
Again, make sure you have [make](https://www.gnu.org/software/make/) installed, and navigate to the root directory:
```
> make test
```
This creates the executable file *test*. Make sure the input.txt file only contains "abc", without any leading or trailing whitespace:
```
> ./test input.txt
```
This outputs the expected and actual result of the sha512 program, if they aren't equal the assertion will fail and the subsequent output will be displayed on the command line.

## What is the SHA512 Algorithm, and why is it important?

The SHA512 algorithm is a hashing algorithm that hashes data given to the it. The SHA512 algorithm is important as it plays a vital role in digital security and cryptography. The reason it is so vital is because it is impossible to reverse a hashed message. This algorithm is used to hash passwords among other sensitive information. It's also extremely useful in a computing sense, because the time it takes to encrypt a message is extremely small, and trying to decrypt a message would take an infeasible amount of time. The message can be any form of data, as long as it's size is less than 2^128 bits. The messaged is processed in blocks of 1024 bits, and the message digest, which is the output of the algorithm, is 512 bits. Hence the name, SHA512.
