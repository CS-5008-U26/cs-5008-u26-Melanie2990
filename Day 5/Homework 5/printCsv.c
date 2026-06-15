#include <stdio.h>
#include <string.h>

/*
 * Removes the newline character from the end of a string.
 * 
 * fgets usually keeps the '\n' when the user presses Enter
 * or when it reads a line from a file. This function replaces
 * that '\n' with '\0', which marks the end of the string.
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
 *     "Seattle"
 * becomes:
 *     Seattle
 */
void removeQuotes(char *field) {
    int len = strlen(field);

    if (len >= 2 && field[0] == '"' && field[len - 1] == '"') {
        int i;

        /*
         * Replace the ending quote with the end-of-string character.
         */
        field[len - 1] = '\0';

        /*
         * Move every character one position to the left.
         * This removes the beginning quote.
         */
        for (i = 0; field[i + 1] != '\0'; i++) {
            field[i] = field[i + 1];
        }

        /*
         * Add the end-of-string character after the moved text.
         */
        field[i] = '\0';
    }
}

/*
 * Gets the next field from a CSV line.
 *
 * start: the place where we begin looking in the string
 * separator: the separator character, which is ',' for CSV files
 * out: where we store the field that we found
 *
 * This function returns a pointer to the next place in the line.
 * If there are no more fields, it returns NULL.
 */
char *getNextField(char *start, char separator, char *out) {
    char *sepPointer;
    int numChars;

    /*
     * If start points to the end of the string, there are no fields left.
     */
    if (*start == '\0') {
        return NULL;
    }

    /*
     * Find the next separator character.
     */
    sepPointer = strchr(start, separator);

    if (sepPointer == NULL) {
        /*
         * If there is no separator, this is the last field.
         * Copy everything from start into out.
         */
        strcpy(out, start);

        /*
         * Remove quotes if this field is surrounded by double quotes.
         */
        removeQuotes(out);

        /*
         * Return a pointer to the end of the string.
         */
        return start + strlen(start);
    } else {
        /*
         * If we found a separator, copy only the characters before it.
         */
        numChars = sepPointer - start;

        strncpy(out, start, numChars);
        out[numChars] = '\0';

        /*
         * Remove quotes if this field is surrounded by double quotes.
         */
        removeQuotes(out);

        /*
         * Return the position after the separator.
         */
        return sepPointer + 1;
    }
}

int main() {
    char filename[1000];
    char line[1000];
    char field[1000];

    /*
     * Ask the user to enter the CSV file name.
     */
    printf("Enter the file name: ");
    fgets(filename, 1000, stdin);
    killNewline(filename);

    /*
     * Open the file for reading.
     */
    FILE *inFile = fopen(filename, "r");

    if (inFile == NULL) {
        printf("Could not open file %s\n", filename);
        return 1;
    }

    /*
     * Read the file one line at a time.
     */
    while (fgets(line, 1000, inFile) != NULL) {
        char *p;

        /*
         * Remove the newline from the line read from the file.
         */
        killNewline(line);

        /*
         * Print the whole line first.
         */
        printf(">%s<\n", line);

        /*
         * Now split this line into fields.
         */
        p = line;

        while (p != NULL) {
            p = getNextField(p, ',', field);

            /*
             * If p is not NULL, that means a field was found.
             */
            if (p != NULL) {
                printf(" >%s<\n", field);
            }
        }
    }

    /*
     * Always close the file after finishing reading it.
     */
    fclose(inFile);

    return 0;
}