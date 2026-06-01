/* Paycheck Program - Homework 2
 *
 * Put your name here: Mengling Zhang
 * CS-5008 Spring 2025
 *
 * This program asks the user for an hourly rate and number of hours worked.
 * It calculates regular pay or overtime pay, then repeats until the user
 * enters a number less than or equal to zero.
 */

#include <stdio.h>

int main() {

    float rate;
    float hours;
    float pay;

    printf("Enter an hourly rate: ");
    scanf("%f", &rate);

    while (rate > 0) {
        printf("Enter the number of hours worked: ");
        scanf("%f", &hours);

        if (hours <= 0) {
            return 0;
        }

        if (hours <= 40) {
            pay = rate * hours;
        } else {
            pay = 40 * rate + (hours - 40) * rate * 1.5;
        }

        printf("You should be paid %g\n", pay);

        printf("Enter an hourly rate: ");
        scanf("%f", &rate);
    }

    return 0;
}