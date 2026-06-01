/* Prime Factors Program - Homework 3
 *
 * Name: Mengling Zhang
 * CS-5008
 *
 * This program asks the user for a number and prints its prime factors.
 * The factors are printed from smallest to largest, separated by " * ".
 * This version uses iteration and does not use extra function calls.
 */

#include <stdio.h>

int main() {

    int number;
    int factor;
    int firstFactor;

    printf("Enter a number: ");
    scanf("%d", &number);

    /* Prime factorization is only meaningful for integers greater than 1. */
    if (number <= 1) {
        printf("Error: number must be greater than 1\n");
        return 0;
    }

    printf("The prime factors are ");

    /* Start with the smallest possible prime factor. */
    factor = 2;

    /* firstFactor controls whether to print " * " before a factor. */
    firstFactor = 1;

    /* Keep factoring until the number has been reduced to 1. */
    while (number > 1) {

        /* Print the current factor as many times as it divides the number. */
        while (number % factor == 0) {
            if (firstFactor == 1) {
                printf("%d", factor);
                firstFactor = 0;
            } else {
                printf(" * %d", factor);
            }

            number = number / factor;
        }

        /* Try the next possible factor. */
        factor = factor + 1;
    }

    printf("\n");

    return 0;
}