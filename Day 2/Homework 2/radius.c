/* Radius Program - Homework 2
 *
 * Put your name here: Mengling Zhang
 * CS-5008 Spring 2025
 *
 * This program asks the user for an integer radius
 * and prints an integer approximation of the circle's area.
 */

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