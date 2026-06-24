#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 300
#define MAX_CANDIES 200
#define NUM_ATTRIBUTES 9

typedef struct Candy {
    char *name;

    int chocolate;
    int fruity;
    int caramel;
    int peanutalmondy;
    int nougat;
    int crispedricewafer;
    int hard;
    int bar;
    int pluribus;

    double sugarpercent;
    double pricepercent;
    double winpercent;
} Candy;

/*
 * Makes a heap copy of a string.
 */
char *copyString(char *source) {
    char *copy = malloc(strlen(source) + 1);

    if (copy == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    strcpy(copy, source);
    return copy;
}

/*
 * Removes a newline character from the end of a string.
 */
void removeNewline(char *line) {
    int length = strlen(line);

    if (length > 0 && line[length - 1] == '\n') {
        line[length - 1] = '\0';
    }
}

/*
 * Reads one CSV line and creates one Candy struct on the heap.
 */
Candy *createCandyFromLine(char *line) {
    Candy *candy = malloc(sizeof(Candy));

    if (candy == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    removeNewline(line);

    char *token = strtok(line, ",");
    candy->name = copyString(token);

    token = strtok(NULL, ",");
    candy->chocolate = atoi(token);

    token = strtok(NULL, ",");
    candy->fruity = atoi(token);

    token = strtok(NULL, ",");
    candy->caramel = atoi(token);

    token = strtok(NULL, ",");
    candy->peanutalmondy = atoi(token);

    token = strtok(NULL, ",");
    candy->nougat = atoi(token);

    token = strtok(NULL, ",");
    candy->crispedricewafer = atoi(token);

    token = strtok(NULL, ",");
    candy->hard = atoi(token);

    token = strtok(NULL, ",");
    candy->bar = atoi(token);

    token = strtok(NULL, ",");
    candy->pluribus = atoi(token);

    token = strtok(NULL, ",");
    candy->sugarpercent = atof(token);

    token = strtok(NULL, ",");
    candy->pricepercent = atof(token);

    token = strtok(NULL, ",");
    candy->winpercent = atof(token);

    return candy;
}

/*
 * Reads the candy CSV file into an array of Candy pointers.
 */
int readCandies(char *filename, Candy *candies[]) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Could not open file: %s\n", filename);
        exit(1);
    }

    char line[MAX_LINE];
    int count = 0;

    // Discard the header line.
    fgets(line, MAX_LINE, file);

    while (fgets(line, MAX_LINE, file) != NULL && count < MAX_CANDIES) {
        candies[count] = createCandyFromLine(line);
        count++;
    }

    fclose(file);
    return count;
}

/*
 * Prints all candy names.
 */
void printCandyNames(Candy *candies[], int count) {
    printf("All candies:\n");

    for (int i = 0; i < count; i++) {
        printf("%s\n", candies[i]->name);
    }

    printf("\n");
}

/*
 * Prints a string in uppercase.
 */
void printUppercase(char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        printf("%c", toupper(text[i]));
    }
}

/*
 * Prints a string in lowercase.
 */
void printLowercase(char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        printf("%c", tolower(text[i]));
    }
}

/*
 * Prints chocolate candies. If the candy also has caramel, print uppercase.
 * Otherwise, print lowercase.
 */
void printChocolateCandies(Candy *candies[], int count) {
    int chocolateCount = 0;
    int chocolateAndCaramelCount = 0;

    printf("Chocolate candies:\n");

    for (int i = 0; i < count; i++) {
        if (candies[i]->chocolate == 1) {
            chocolateCount++;

            if (candies[i]->caramel == 1) {
                chocolateAndCaramelCount++;
                printUppercase(candies[i]->name);
            } else {
                printLowercase(candies[i]->name);
            }

            printf("\n");
        }
    }

    if (chocolateCount > 0) {
        double percent = 100.0 * chocolateAndCaramelCount / chocolateCount;
        printf("Percent of chocolate candies with caramel: %.2f%%\n", percent);
    }

    printf("\n");
}

/*
 * Returns the selected attribute value from a Candy.
 */
int getAttribute(Candy *candy, int index) {
    if (index == 0) {
        return candy->chocolate;
    } else if (index == 1) {
        return candy->fruity;
    } else if (index == 2) {
        return candy->caramel;
    } else if (index == 3) {
        return candy->peanutalmondy;
    } else if (index == 4) {
        return candy->nougat;
    } else if (index == 5) {
        return candy->crispedricewafer;
    } else if (index == 6) {
        return candy->hard;
    } else if (index == 7) {
        return candy->bar;
    } else {
        return candy->pluribus;
    }
}

/*
 * Prints average sugar percent, price percent, and win percent
 * for candies with a selected attribute.
 */
void printAttributeSummary(Candy *candies[], int count) {
    char *attributeNames[NUM_ATTRIBUTES] = {
        "chocolate",
        "fruity",
        "caramel",
        "peanutalmondy",
        "nougat",
        "crispedricewafer",
        "hard",
        "bar",
        "pluribus"
    };

    printf("Summary by attribute:\n");

    for (int attribute = 0; attribute < NUM_ATTRIBUTES; attribute++) {
        double sugarTotal = 0.0;
        double priceTotal = 0.0;
        double winTotal = 0.0;
        int attributeCount = 0;

        for (int i = 0; i < count; i++) {
            if (getAttribute(candies[i], attribute) == 1) {
                sugarTotal += candies[i]->sugarpercent;
                priceTotal += candies[i]->pricepercent;
                winTotal += candies[i]->winpercent;
                attributeCount++;
            }
        }

        if (attributeCount > 0) {
            printf("%s: sugar %.3f, price %.3f, win %.3f\n",
                   attributeNames[attribute],
                   sugarTotal / attributeCount,
                   priceTotal / attributeCount,
                   winTotal / attributeCount);
        }
    }

    printf("\n");
}

/*
 * Prints summary for candies whose sugar percent is higher than average
 * and candies whose price percent is higher than average.
 */
void printAboveAverageSummaries(Candy *candies[], int count) {
    double totalSugar = 0.0;
    double totalPrice = 0.0;

    for (int i = 0; i < count; i++) {
        totalSugar += candies[i]->sugarpercent;
        totalPrice += candies[i]->pricepercent;
    }

    double averageSugar = totalSugar / count;
    double averagePrice = totalPrice / count;

    double sugarGroupSugar = 0.0;
    double sugarGroupPrice = 0.0;
    double sugarGroupWin = 0.0;
    int sugarGroupCount = 0;

    double priceGroupSugar = 0.0;
    double priceGroupPrice = 0.0;
    double priceGroupWin = 0.0;
    int priceGroupCount = 0;

    for (int i = 0; i < count; i++) {
        if (candies[i]->sugarpercent > averageSugar) {
            sugarGroupSugar += candies[i]->sugarpercent;
            sugarGroupPrice += candies[i]->pricepercent;
            sugarGroupWin += candies[i]->winpercent;
            sugarGroupCount++;
        }

        if (candies[i]->pricepercent > averagePrice) {
            priceGroupSugar += candies[i]->sugarpercent;
            priceGroupPrice += candies[i]->pricepercent;
            priceGroupWin += candies[i]->winpercent;
            priceGroupCount++;
        }
    }

    printf("Candies with sugar percent above average sugar percent:\n");
    printf("sugar %.3f, price %.3f, win %.3f\n",
           sugarGroupSugar / sugarGroupCount,
           sugarGroupPrice / sugarGroupCount,
           sugarGroupWin / sugarGroupCount);

    printf("Candies with price percent above average price percent:\n");
    printf("sugar %.3f, price %.3f, win %.3f\n",
           priceGroupSugar / priceGroupCount,
           priceGroupPrice / priceGroupCount,
           priceGroupWin / priceGroupCount);

    printf("\n");
}

/*
 * Frees all heap memory used by the candies.
 */
void freeCandies(Candy *candies[], int count) {
    for (int i = 0; i < count; i++) {
        free(candies[i]->name);
        free(candies[i]);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./candy filename\n");
        return 1;
    }

    Candy *candies[MAX_CANDIES];

    int count = readCandies(argv[1], candies);

    printCandyNames(candies, count);
    printChocolateCandies(candies, count);
    printAttributeSummary(candies, count);
    printAboveAverageSummaries(candies, count);

    freeCandies(candies, count);

    return 0;
}