/* Letter Grade Switch Program - Homework 2
 *
 * Put your name here: Mengling Zhang
 * CS-5008 Spring 2025
 *
 * This program asks the user for a number score from 1 to 10
 * and prints the matching letter grade using switch.
 */

#include <stdio.h>


int main() {

    int score;

    printf("Enter the number score: ");
    scanf("%d", &score);

    switch (score) {
        case 1:
        case 2:
        case 3:
            printf("Your grade is F\n");
            break;
        case 4:
        case 5:
            printf("Your grade is E\n");
            break;
        case 6:
            printf("Your grade is D\n");
            break;
        case 7:
            printf("Your grade is C\n");
            break;
        case 8:
            printf("Your grade is B\n");
            break;
        case 9:
        case 10:
            printf("Your grade is A\n");
            break;
        default:
            printf("Error: score must be between 1 and 10\n");
            break;
    }

    return 0;
}