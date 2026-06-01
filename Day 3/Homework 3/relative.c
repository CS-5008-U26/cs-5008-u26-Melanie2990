/* Relative Prime Program - Homework 3
 *
 * Name: Mengling Zhang
 * CS-5008
 *
 * This program asks the user for an integer n.
 * It prints et(x) for every integer from 2 to n.
 * The function et(x) counts how many numbers less than x
 * are relatively prime with x.
 */

#include <stdio.h>

/* Count how many numbers less than x are relatively prime with x. */
int et(int x) {

    int count;
    int i;
    int factor;
    int relativelyPrime;

    count = 0;

    /* Check every number from 1 to x - 1. */
    for (i = 1; i < x; i++) {

        relativelyPrime = 1;

        /* Check if i and x have a common factor greater than 1. */
        for (factor = 2; factor <= i; factor++) {
            if (i % factor == 0 && x % factor == 0) {
                relativelyPrime = 0;
            }
        }

        if (relativelyPrime == 1) {
            count = count + 1;
        }
    }

    return count;
}

int main() {

    int n;
    int i;

    printf("Enter an integer: ");
    scanf("%d", &n);

    if (n < 2) {
        printf("Error: number must be at least 2\n");
        return 0;
    }

    /* Print et(i) for every number from 2 to n. */
    for (i = 2; i <= n; i++) {
        printf("et(%d) = %d\n", i, et(i));
    }

    return 0;
}