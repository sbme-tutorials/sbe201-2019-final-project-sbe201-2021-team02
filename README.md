# Huffman Coding
A C++ compression and decompression program based on Huffman Coding.

## Introduction

This project is to design compression and decompression programs based on Huffman Coding.
The idea of Huffman Coding is to minimize the weighted expected length of the code by means of assigning shorter codes to frequently-used characters and longer codes to seldom-used code.

### Compression

1. Open input file, count and store the frequencies of different characters and store them in a map
2. Construct Priority Queue and then Huffman Tree;
3. Construct a map to store the huffman code for each character.
4. stream the desired file and convert all the characters to their huffman code then convert eah 8 bits to decimal, after that, convert each decimal number to a single character using ASCII code
5. print these characters along with the frequency contatining map in the desired output file.

#### Decompression

1. Open file, recreate Huffman Tree based on the frequency containing map;
2. Decode the file based on the tree and store it to the desired file.
