/* Letter Grade Program - Homework 2
 *
 * Put your name here: Mengling Zhang
 * CS-5008 Spring 2025
 *
 * This program asks the user for a number score from 1 to 10
 * and prints the matching letter grade using if and else.
 */

#include <stdio.h>


int main() {

    int score;

    printf("Enter the number score: ");
    scanf("%d", &score);

    if (score < 1 || score > 10) {
        printf("Error: score must be between 1 and 10\n");
    } else if (score <= 3) {
        printf("Your grade is F\n");
    } else if (score <= 5) {
        printf("Your grade is E\n");
    } else if (score == 6) {
        printf("Your grade is D\n");
    } else if (score == 7) {
        printf("Your grade is C\n");
    } else if (score == 8) {
        printf("Your grade is B\n");
    } else {
        printf("Your grade is A\n");
    }

    return 0;
}