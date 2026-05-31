/* A Third C Program - for Homework 1
 *
 * Put your name here: Mengling Zhang
 * CS-5008 Spring 2025
 *
 * This program asks the user for their name and age.
 * It then prints two responses using both pieces of information.
 */

#include <stdio.h>

int main() {

    char name[100];                 // name stores the value entered by the user
    char pause[100];                // pause is used to keep the console window open
    int age;                        // age stores the user's age

    /* Ask for the user's name */
    printf("What is your name? ");
    scanf("%99s", name);

    /* Ask for one more input */
    printf("How old are you? ");
    scanf("%d", &age);

    /* Print the first output */
    printf("Hello, %s!\n", name);

    /* Print one more output */
    printf("You are %d years old.\n", age);

    /* Pause the console so we can look at it before it vanishes */
    fgets(pause, 100, stdin);
    fgets(pause, 100, stdin);

    return 0;
}