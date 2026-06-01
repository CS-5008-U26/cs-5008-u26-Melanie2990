#include <stdio.h>

int main() {

    int score;
    int lastDigit;
    char grade;
    char sign;

    printf("Enter the number score: ");
    scanf("%d", &score);

    if (score < 0 || score > 100) {
        printf("Error: score must be between 0 and 100\n");
        return 0;
    }

    if (score <= 60) {
        printf("Your grade is F\n");
        return 0;
    } else if (score <= 70) {
        grade = 'D';
    } else if (score <= 80) {
        grade = 'C';
    } else if (score <= 90) {
        grade = 'B';
    } else {
        grade = 'A';
    }

    lastDigit = score % 10;

    if (lastDigit == 1 || lastDigit == 2 || lastDigit == 3) {
        sign = '-';
    } else if (lastDigit == 8 || lastDigit == 9 || lastDigit == 0) {
        sign = '+';
    } else {
        sign = ' ';
    }

    if (sign == ' ') {
        printf("Your grade is %c\n", grade);
    } else {
        printf("Your grade is %c%c\n", grade, sign);
    }

    return 0;
}