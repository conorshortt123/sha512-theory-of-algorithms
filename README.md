# Theory of Algorithms - Conor Shortt
The contents of this repository are for my fourth-year module, *Theory of Algorithms*.
The task was to write a C program that calculates the [SHA512](https://www.nist.gov/publications/secure-hash-standard) value of any given input file. The program takes in the path of the file as a command line argument, the file contents are then parsed and passed into the [SHA512](https://www.nist.gov/publications/secure-hash-standard) algorithm. The message then goes through pre-processing which involves padding, parsing, and setting initial hash values. The actual hash values are then computed the program then prints out the correct SHA512 digest.


## How to run:
Make sure you have [make](https://www.gnu.org/software/make/) installed, then navigate into the root directory of the cloned repository:
```
> make
```
This creates the executable file *main*. You will then type:
```
> ./main input.txt
```
The output of the program is a 128-character hexadecimal string, which translates to the 512-bit message as output from the algorithm.


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

The SHA512 algorithm is a hashing algorithm that hashes data given to the it. The SHA512 algorithm is important as it plays a vital role in digital security and cryptography. The reason it is so vital is because it is impossible to reverse a hashed message. This algorithm is used to hash passwords among other sensitive information. It's also extremely useful in a computing sense, because the time it takes to encrypt a message is extremely small and trying to decrypt a hashed message would take an infeasible amount of time. The message can be any form of data, as long as its size is less than 2^128 bits. The messaged is processed in blocks of 1024 bits, and the message digest, which is the output of the algorithm, is 512 bits. Hence the name, SHA512.

## Why can't we reverse a message digest produced by the algorithm?

The SHA512 algorithm is a one-way function. A one-way function is a function that is easy to compute given any input, but hard to invert given the output of a random input. In relation to the SHA512, the output is impossible to reverse. Currently, no one knows how to find the input of a SHA-512 algorithm better than guessing the input, which can take a very long time. You could use a dictionary of common passwords to use as input and possibly obtain the desired hash as output, but if a password includes enough random characters etc, it's completely infeasible. According to [this](https://stackoverflow.com/questions/6776050/how-long-to-brute-force-a-salted-sha-512-hash-salt-provided) StackOverflow post, it would take 3.17 x 10 ^ 64 years to brute force a hashed password. The reason a hash is impossible to reverse is, for example, take a simple mathematical operation, such as subtraction. Subtraction takes two inputs and produces one output, for example 66 = 78 - 12. But equally, 66 = 81 - 15, so if you only know the output, how do you know what the two inputs are? What happens during a real hash function is many one-way operations take place, and if you were to use trial and error to guess and work your way backwards through the algorithm, there would simply be too many possible combinations (more than atoms in the known universe).

## Can you design an algorithm that can find input messages that give each of the possible 512-bit strings?

In short, yes, given enough time (which would be an incomprehensible length of time) a brute force algorithm could find input messages for each possible 512-bit strings. But, given that there are 2^512 possible combinations, this is completely and utterly infeasible. A brute force algorithm is exactly what it sounds like, a straightforward method of solving a problem that relies entirely on computational power and tries every single possibility rather than other techniques that improve power or efficiency. To understand how infeasible a try-each-possible-combination attack would be, the number of possible inputs into the sha-512 algorithm is 2^(2^128 -1)-1. And although this number is incomprehensible, you will have exhausted all possible outputs before trying the many possible inputs. A different method of obtaining inputs from output is a [preimage attack](https://en.wikipedia.org/wiki/Preimage_attack). In mathematics, a [preimage](https://en.wikipedia.org/wiki/Image_(mathematics)#Inverse_image), is a set of possible inputs for a given output. And, in cryptography, a preimage attack on a hash function tries to find a message that has a specific hash value. A cryptographic hash function should have a certain amount of resistance to preimage attacks:
* *Preimage Resistance* - For all pre-specified outputs, it is computationally infeasible to find any input that hashes to that output. I.e., given y, it is difficult to find x such that h(x) = y.
* *Second-preimage Resistance* - It's computationally infeasible to find any other input that has the same hash as that of the specified input.

## How difficult is it to find a hash digest beginning with at least twelve zeros?

This is a very common task for cryptocurrency miners. The goal of crypto mining is to solve a "block", each block has a target hash value with a certain number of leading zeros depending on the difficulty. The target hash is obtained by manipulating a blocks hash value by adding a "nonce" which is a number used once. The "nonce" is a random value and changes the output of the hash value. The miner's computer will do this millions of times per second until the block is "solved" when a hash value lower than the target value is found. The reason this works is that the miner is awarded a certain amount of bitcoin to compensate for the electricity cost in solving the block, which can be very high depending on hardware. To calculate the probability of finding a hash digest that begins with at least 12 zeros you can use the formula (1/base)^n, where n is the number of zeros required at the start of a hash digest. When plugging in the numbers for hexidecimal digest, (1/16)^12, it evaluates to 3.55 × 10-15. This is an extremely low probability. To get the average number of hashes needed to find one that will start with 12 zeros, you take the reciprocal of it, so 1/3.55e-15 = 281 trillion average hashes. In terms of how long a computer would take to calculate this many hashes, you need to find the hashrate of a particular computer. My personal computer, which has a fairly powerful graphics card, can get up to 30Mh/s = 30 million hashes per second. This means that my computer would take 9'382'499 seconds, or 108 days. This is a generous hashrate though as most computers wouldn't have a powerful graphics card. A 2014 macbook can do around 100'000 hashes per second, meaning it would take this macbook about 89 years to find this hash. A fair estimate of the cost of running my graphics card at 30Mh/s for 108 days would be about €28. This number was calculated from [here](https://www.cryptocompare.com/mining/calculator/).

### References
Project Brief; Dr Ian McLoughlin; https://github.com/ianmcloughlin/theory-algos-project

SHA512 Algorithm; FIPS; https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf

Formula for Calculating Probability; StackOverflow; https://bitcoin.stackexchange.com/questions/81655/creating-a-hash-that-starts-wtih-9-zeros?fbclid=IwAR23yRwKWfj-iy6oLp62Je_fpHeJqKehCLOswOCZEl5w3rG42KZmuvZxTdg
