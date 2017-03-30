# File Compression

Compress any file format faster with Huffman Encoding. 

## Dependency

This command-line application is written in C/C++ and requires gcc/g++ compiler.


## Installation 

Makefile is provided. $make can be used. Or manually compiler the driver file.
```
$ make
```

## Algorithm

Command line application written in C++ which compresses files despite the format with [huffman coding](https://en.wikipedia.org/wiki/Huffman_coding) algorithm.

File is read from command line which allow reading binary format of any files. After reading, all the bits are inserted into Binary Heap which provides bit manipulations and applying huffman encodings.
