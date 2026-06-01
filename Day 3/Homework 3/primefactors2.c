/* Recursive Prime Factors Program - Homework 3
 *
 * Name: Mengling Zhang
 * CS-5008
 *
 * This program asks the user for a number and prints its prime factors.
 * It uses a recursive function to print one factor at a time.
 */

#include <stdio.h>

/* Print the prime factors of number using recursion. */
int printFactors(int number, int factor, int firstFactor) {

    /* Stop when the number has been completely factored. */
    if (number == 1) {
        return 0;
    }

    /* If factor divides number, print it. */
    if (number % factor == 0) {

        if (firstFactor == 1) {
            printf("%d", factor);
        } else {
            printf(" * %d", factor);
        }

        return printFactors(number / factor, factor, 0);
    } else {
        /* If factor does not divide number, try the next possible factor. */
        return printFactors(number, factor + 1, firstFactor);
    }
}

int main() {

    int number;

    printf("Enter a number: ");
    scanf("%d", &number);

    if (number <= 1) {
        printf("Error: number must be greater than 1\n");
        return 0;
    }

    printf("The prime factors are ");

    printFactors(number, 2, 1);

    printf("\n");

    return 0;
}