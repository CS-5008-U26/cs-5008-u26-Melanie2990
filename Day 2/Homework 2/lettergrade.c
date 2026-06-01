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