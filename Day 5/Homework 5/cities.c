#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Removes the newline character from the end of a string.
 * fgets keeps the newline, so this function removes it.
 */
void killNewline(char *str) {
    int len = strlen(str);

    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

/*
 * Removes double quotes around a field, if the field starts
 * and ends with double quotes.
 *
 * Example:
 *     "New York"
 * becomes:
 *     New York
 */
void removeQuotes(char *field) {
    int len = strlen(field);

    if (len >= 2 && field[0] == '"' && field[len - 1] == '"') {
        int i;

        field[len - 1] = '\0';

        for (i = 0; field[i + 1] != '\0'; i++) {
            field[i] = field[i + 1];
        }

        field[i] = '\0';
    }
}

/*
 * Gets the next field from a CSV line.
 *
 * start: where we begin reading in the line
 * separator: the character that separates fields, usually ','
 * out: where the field will be copied
 *
 * Returns a pointer to the next field.
 * Returns NULL if there are no more fields.
 */
char *getNextField(char *start, char separator, char *out) {
    char *sepPointer;
    int numChars;

    if (*start == '\0') {
        return NULL;
    }

    sepPointer = strchr(start, separator);

    if (sepPointer == NULL) {
        strcpy(out, start);
        removeQuotes(out);
        return start + strlen(start);
    } else {
        numChars = sepPointer - start;

        strncpy(out, start, numChars);
        out[numChars] = '\0';

        removeQuotes(out);

        return sepPointer + 1;
    }
}

/*
 * Tries to open the city data file.
 *
 * The first path works when the program is run from:
 *     Day 5/Homework 5
 *
 * The second path works when the program is run from:
 *     the repository root folder
 */
FILE *openCitiesFile() {
    FILE *inFile;

    inFile = fopen("../../Resources/uscities.csv", "r");

    if (inFile == NULL) {
        inFile = fopen("Resources/uscities.csv", "r");
    }

    return inFile;
}

int main() {
    FILE *inFile;
    char line[1000];
    char field[1000];

    int cityCount = 0;
    long totalPopulation = 0;

    double northernmostLatitude = -1000.0;
    char northernmostCity[1000] = "";

    inFile = openCitiesFile();

    if (inFile == NULL) {
        printf("Could not open uscities.csv\n");
        return 1;
    }

    /*
     * Read and ignore the first line.
     * The first line contains the column names.
     */
    fgets(line, 1000, inFile);

    /*
     * Process only the first 20 cities after the header line.
     */
    while (cityCount < 20 && fgets(line, 1000, inFile) != NULL) {
        char *p;
        char cityName[1000] = "";
        char latitudeString[1000] = "";
        char populationString[1000] = "";

        int columnNumber = 1;
        int population;
        double latitude;

        killNewline(line);

        p = line;

        /*
         * Go through each field in the current line.
         * We only need:
         * column 2: city ASCII name
         * column 7: latitude
         * column 9: population
         */
        while (p != NULL) {
            p = getNextField(p, ',', field);

            if (p != NULL) {
                if (columnNumber == 2) {
                    strcpy(cityName, field);
                } else if (columnNumber == 7) {
                    strcpy(latitudeString, field);
                } else if (columnNumber == 9) {
                    strcpy(populationString, field);
                }

                columnNumber++;
            }
        }

        /*
         * Convert the population and latitude from strings to numbers.
         */
        population = atoi(populationString);
        latitude = atof(latitudeString);

        /*
         * Add this city's population to the total.
         */
        totalPopulation = totalPopulation + population;

        /*
         * If this city has the largest latitude so far,
         * remember this city as the northernmost city.
         */
        if (latitude > northernmostLatitude) {
            northernmostLatitude = latitude;
            strcpy(northernmostCity, cityName);
        }

        cityCount++;
    }

    fclose(inFile);

    printf("Sum of the populations: %ld\n", totalPopulation);
    printf("Northernmost city is %s\n", northernmostCity);

    return 0;
}