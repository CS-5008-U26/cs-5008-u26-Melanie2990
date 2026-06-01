#include <stdio.h>

int main() {

    float rate;
    float hours;
    float pay;

    while (1) {

        printf("Enter an hourly rate: ");
        scanf("%f", &rate);

        if (rate <= 0) {
            return 0;
        }

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

        printf("You should be paid %f\n", pay);
    }

    return 0;
}