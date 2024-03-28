# VNU-UET's Course INT3402 1: Compiler - Assignment 1

## Authors

- Nguyen Duy Hùng, 21020436, VNU-UET, Computer Engineering Student, Class of 2025 
- Nguyen Thanh An, 21020886, VNU-UET, Computer Engineering Student, Class of 2025
- Nguyen Thu Hiền, 21020141, VNU-UET, Computer Engineering Student, Class of 2025

## Introduction

A simple, built-from-scratch scanner and a C/C++ FLEX-generated scanner in the UPL programming language are built with the following rules of syntax:
- `begin`, `end`, `if`, `else`, `then`, `do`, `while`, `print`, `int`, `bool` are keywords.
- There are 3 commparison operations: greater than `>`, greater than or equal `>=`, equal `==`. 
- A variable name must start with a letter, any numbers included must be at the end. Example: `a1`, `c33`, `de114` are correct variable names.
- Variables must be declared before use. Variables can be initialized using assignment operators `=`.
- There are 2 arithmetic operations: addition `+`, multiplication `*`.
- Comments are written using `//` for single-line comments or enclosing multi-line comments within `/*` and `*/`.

The context-free grammar for the UPL is designed.

## File structure

- The `Context-free grammar.md` contains definitions for context-free grammar for the UPL.
- Source code for the built-from-scratch scanner and the C/C++ FLEX scanner is included in `./src` folder.
- `./out` folder contains binary files of above scanners.
- `./example` contains the example input file and example output file for the scanners.

## Context-free grammar

- A Statement List `SL` may contain a statement `S`, a block statement `BS`, a comment `cmt` or null `E`. Here, a statement can be a declaration statement `DS` or a computational statement `CS`; while a block statement contains condition `IF` and loop `DW`.
- An expression can be addition `+`, multiplication `*` or comparison `relop`. The order of precedence from highest to lowest is: `*`, `+`, `relop`.  

## Running the scanner

The following examples generate tokens from file `./example/input.in` and save the result to `example/output.out` by using:

- The built-from-scratch scanner:
    ```
    .out/scanner ./example/input.in example/output.out
    ```
- The C/C++ FLEX scanner:
    ```
    .out/scanner_lex ./example/input.in example/output.out
    ```

To change the two scanners by changing the source code:
- Change the built-from-scratch scanner:
    ```
    g++ ./src/scanner.cpp -o ./out/scanner
    ```
- Change the C/C++ FLEX scanner:
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
Error at line ... at position ..., can't resolve the symbol: ... 
```
