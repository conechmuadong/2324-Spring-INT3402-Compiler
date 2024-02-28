# VNU-UET's Course INT3402 1: Compiler - Assignment 1 

## Authors
- Nguyen Duy Hùng, 21020436, VNU-UET, Computer Engineering Student, Class of 2025 
- Nguyen Thanh An, 21020886, VNU-UET, Computer Engineering Student, Class of 2025
- Nguyen Thu Hiền, 21020141, VNU-UET, Computer Engineering Student, Class of 2025

## Introduction

An simple token parser for scanning step of a simple programing language compiler with following rules of syntax:
The programing language just have only one statement type: `if cond then stmt else stmt;`
Where: 
- `if`, `then`, `else` are keywords
- `cond` is a greater comparison between two integers, two variables or an integer and a variable, such as: `a>b`, `1>a`, `2>1`
- `stmt` is a asignment, example: `a=b`

## Running the parser

The following example starts the parser for generating token from file `input.in` and saves the result to `out/output.out`:

```
./parser input.in out/output.out
```

For changing the source code and re-compile the parser:
```
g++ ./src/parser.cpp -o parser
```

## Output format

The output file is formated as following:
```
<TokenType, Value(Optional)>,
<TokenType, Value(Optional)>,
....
<TokenType, Value(Optional)>
```
In the case there is an error in the input file, the output file return: 
```
Error at line ... at position ..., can't resolve the symbols ... 
```
