/* A First C Program - for Homework 1
 *
 * This code will start you off. Modify it as instructed in Homework 1.
 *
 * Put your name here: Mengling Zhang
 * CS-5008 Spring 2025
 */

#include <stdio.h>

int main() {

    int x;                          // x will be the first number entered by the user
    int y;                          // y will be the second number entered by the user
    int sum;                        // sum will hold the total of x and y
    char s[100];                    // s is used to pause the console

    /* Ask for two numbers and print their sum */
    printf("Enter a number:\n\n");
    scanf("%d", &x);

    printf("\nEnter another number:\n\n");
    scanf("%d", &y);

    sum = x + y;
    printf("\nTheir sum is %d\n", sum);

    /* Pause the console so we can look at it before it vanishes */
    fgets(s, 100, stdin);
    fgets(s, 100, stdin);

    return 0;
}