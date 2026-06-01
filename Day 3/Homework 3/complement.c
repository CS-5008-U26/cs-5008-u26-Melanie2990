/* Complement Program - Homework 3
 *
 * Name: Mengling Zhang
 * CS-5008
 *
 * This program asks the user for an integer and prints its 10's complement.
 * The complement is calculated by replacing each digit with 9 minus that digit.
 * This program uses a recursive function.
 */

#include <stdio.h>

/* Function prototype */
int complement(int x);

int main() {

    int number;
    int result;

    printf("Enter an integer: ");
    scanf("%d", &number);

    result = complement(number);

    printf("The complement is %d\n", result);

    return 0;
}

/* Recursively calculate the 10's complement of x */
int complement(int x) {

    int rightDigit;
    int rest;
    int restComplement;
    int newDigit;

    if (x < 10) {
        return 9 - x;
    }

    rightDigit = x % 10;
    rest = x / 10;

    restComplement = complement(rest);
    newDigit = 9 - rightDigit;

    return restComplement * 10 + newDigit;
}