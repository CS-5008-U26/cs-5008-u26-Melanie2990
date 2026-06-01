/* Roman Numeral Program - Homework 2
 *
 * Put your name here: Mengling Zhang
 * CS-5008 Spring 2025
 *
 * This program asks the user for a number between 1 and 4000
 * and prints the number as a Roman numeral.
 */

#include <stdio.h>

int main() {

    int number;

    printf("Enter a number between 1 and 4000: ");
    scanf("%d", &number);

    if (number < 1 || number > 4000) {
        printf("Error: number must be between 1 and 4000\n");
        return 0;
    }

    printf("The Roman Numeral: ");

    while (number >= 1000) {
        printf("M");
        number = number - 1000;
    }

    if (number >= 900) {
        printf("CM");
        number = number - 900;
    }

    if (number >= 500) {
        printf("D");
        number = number - 500;
    }

    if (number >= 400) {
        printf("CD");
        number = number - 400;
    }

    while (number >= 100) {
        printf("C");
        number = number - 100;
    }

    if (number >= 90) {
        printf("XC");
        number = number - 90;
    }

    if (number >= 50) {
        printf("L");
        number = number - 50;
    }

    if (number >= 40) {
        printf("XL");
        number = number - 40;
    }

    while (number >= 10) {
        printf("X");
        number = number - 10;
    }

    if (number >= 9) {
        printf("IX");
        number = number - 9;
    }

    if (number >= 5) {
        printf("V");
        number = number - 5;
    }

    if (number >= 4) {
        printf("IV");
        number = number - 4;
    }

    while (number >= 1) {
        printf("I");
        number = number - 1;
    }

    printf("\n");

    return 0;
}