#include <stdio.h>
#include <math.h>

int main() {

    int radius;
    double pi;
    double area;
    int areaInteger;

    printf("Enter an integer radius: ");
    scanf("%d", &radius);

    pi = acos(-1);
    area = pi * radius * radius;
    areaInteger = (int) area;

    printf("For a circle of radius %.1f the area is %d\n", (double) radius, areaInteger);

    return 0;
}