/* Cents Program - Homework 2
 *
 * Put your name here: Mengling Zhang
 * CS-5008 Spring 2025
 *
 * This program asks the user for a number of cents and prints
 * the number of quarters, dimes, nickels, and pennies required.
 */

#include <stdio.h>

int main() {

    int cents;
    int quarters;
    int dimes;
    int nickels;
    int pennies;

    printf("Enter a number of cents: ");
    scanf("%d", &cents);

    if (cents < 1 || cents > 100) {
        printf("Error: cents must be between 1 and 100\n");
        return 0;
    }

    quarters = cents / 25;
    cents = cents % 25;

    dimes = cents / 10;
    cents = cents % 10;

    nickels = cents / 5;
    cents = cents % 5;

    pennies = cents;

    printf("That requires ");

    if (quarters > 0) {
        if (quarters == 1) {
            printf("%d quarter", quarters);
        } else {
            printf("%d quarters", quarters);
        }
    }

    if (dimes > 0) {
        if (quarters > 0) {
            printf(", ");
        }

        if (dimes == 1) {
            printf("%d dime", dimes);
        } else {
            printf("%d dimes", dimes);
        }
    }

    if (nickels > 0) {
        if (quarters > 0 || dimes > 0) {
            printf(", ");
        }

        if (nickels == 1) {
            printf("%d nickel", nickels);
        } else {
            printf("%d nickels", nickels);
        }
    }

    if (pennies > 0) {
        if (quarters > 0 || dimes > 0 || nickels > 0) {
            printf(", and ");
        }

        if (pennies == 1) {
            printf("%d penny", pennies);
        } else {
            printf("%d pennies", pennies);
        }
    }

    printf(".\n");

    return 0;
}