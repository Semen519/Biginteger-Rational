
# BigInteger and Rational Classes

## Overview
This project consists of two primary classes: `BigInteger` and `Rational`. The `BigInteger` class is designed to handle very large integers, far exceeding the typical range of standard integer types in C++. The `Rational` class is built to handle rational numbers, represented as a fraction of two `BigInteger` objects, allowing for operations on fractions with very large numerators and denominators.

## Features

### BigInteger
- Handles arithmetic operations (addition, subtraction, multiplication, division, modulus) on large integers.
- Supports comparison operators (less than, greater than, equality, etc.).
- Includes unary operations such as increment and decrement.
- Provides a method to convert the `BigInteger` into a string representation.
- Overloaded input and output stream operators for easy interaction.

### Rational
- Represents rational numbers as a fraction of two `BigInteger` objects (numerator and denominator).
- Supports basic arithmetic operations (addition, subtraction, multiplication, division) between rational numbers.
- Allows comparison between rational numbers.
- Includes a method to reduce the fraction to its simplest form.
- Provides a method for converting the rational number into a string representation.
- Offers a method to represent the rational number as a decimal with specified precision.
- Overloaded input and output stream operators for easy interaction.

## Usage

### BigInteger
```cpp
BigInteger bigInt1("12345678901234567890");
BigInteger bigInt2 = 1234567890_bi;
BigInteger result;

result = bigInt1 + bigInt2;  // Addition
result = bigInt1 - bigInt2;  // Subtraction
result = bigInt1 * bigInt2;  // Multiplication
result = bigInt1 / bigInt2;  // Division
result = bigInt1 % bigInt2;  // Modulus
```

### Rational
```cpp
Rational frac1(BigInteger("12345678901234567890"), BigInteger("98765432109876543210"));
Rational frac2(123, 456);
Rational result;

result = frac1 + frac2;  // Addition
result = frac1 - frac2;  // Subtraction
result = frac1 * frac2;  // Multiplication
result = frac1 / frac2;  // Division
```

## Special Notes
- The `BigInteger` class uses a vector of integers to store the digits of the large number. This allows for operations on numbers larger than the standard integer types can handle.
- The `Rational` class automatically reduces fractions to their simplest form after every operation.
- The arithmetic operations in both classes are implemented with efficiency in mind, but for extremely large numbers, operations might still be time-consuming.

## Installation
- Include the `BigInteger` and `Rational` class definitions in your C++ project.
- Compile your project with a C++11 (or later) compatible compiler.
