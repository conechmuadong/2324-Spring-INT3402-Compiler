# VNU-UET's Course INT3402 1: Compiler - Assignment 1

## Authors
- Nguyen Duy Hùng, 21020436, VNU-UET, Computer Engineering Student, Class of 2025 
- Nguyen Thanh An, 21020886, VNU-UET, Computer Engineering Student, Class of 2025
- Nguyen Thu Hiền, 21020141, VNU-UET, Computer Engineering Student, Class of 2025

## Introduction

A simple built from scratch and a C/C++ FLEX-generated scanner in a simple programming language compiler with the following rules of syntax:
- a
- b
- c
- d

And the context-free grammar for this programming language

## File structure
- The `grammar.txt` contains definitions for context-free grammar for this programming language.
- Source code for the scanner built from scratch and the C/C++ FLEX scanner is included in `./src` folder
- `./out` folder contains binary files of above scanners.
- `./example` contains the example input file and example output file for the scanners.

## Running the parser

The following examples start the parser for generating token from file `./example/input.in` and saves the result to `example/output.out`:

- By built-from-scratch scanner:
    ```
    .out/scanner ./example/input.in example/output.out
    ```
- By C/C++ FLEX scanner:
    ```
    .out/scanner_lex ./example/input.in example/output.out
    ```
For changing the parser by changing the source code:
- Change built-from-scratch scanner:
    ```
    g++ ./src/scanner.cpp -o ./out/scanner
    ```
- Change C/C++ FLEX scanner:
    ```
    cd ./src/lex
    lex scanner.l
    gcc lex.yy.c -o ../../out/scanner_lex
    ```

## Output format

The output file is formatted as following:
```
<TokenType, Value(Optional)>,
<TokenType, Value(Optional)>,
....
<TokenType, Value(Optional)>
```
If there is an error in the input file, the output file returns: 
```
Error at line ... at position ..., can't resolve the symbols ... 
```
