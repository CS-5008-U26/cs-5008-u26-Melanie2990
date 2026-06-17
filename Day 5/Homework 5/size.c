#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

/*
 * Removes the newline character from the end of a string.
 * fgets usually keeps the newline, so this function removes it.
 */
void killNewline(char *str) {
    int len = strlen(str);

    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int main(int argc, char *argv[]) {
    char filename[1000];
    struct stat fileInfo;

    /*
     * If the user gives a file name as a command-line argument,
     * use that file name.
     *
     * Example:
     *     ./size ../../Resources/uscities.csv
     */
    if (argc >= 2) {
        strcpy(filename, argv[1]);
    } else {
        /*
         * If no command-line argument is given,
         * ask the user to type a file name.
         */
        printf("Enter the name of a file: ");
        fgets(filename, 1000, stdin);
        killNewline(filename);
    }

    /*
     * stat gets information about the file.
     * If stat returns 0, it worked.
     * If stat returns something else, it failed.
     */
    if (stat(filename, &fileInfo) != 0) {
        printf("Could not get information about %s\n", filename);
        return 1;
    }

    /*
     * st_size stores the size of the file in bytes.
     */
    printf("%s has %lld bytes\n", filename, (long long) fileInfo.st_size);

    return 0;
}