#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 20000
#define MAX_INPUT 50
#define NUM_CITIES 20

typedef struct cityData {
    char *city;
    char *state;
    int population;
} City;

typedef struct singleNode {
    void *data;
    struct singleNode *next;
} sNode;

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
 * Removes quotes from the beginning and end of a string if they exist.
 */
void removeQuotes(char *text) {
    int length = strlen(text);

    if (length >= 2 && text[0] == '"' && text[length - 1] == '"') {
        for (int i = 0; i < length - 2; i++) {
            text[i] = text[i + 1];
        }

        text[length - 2] = '\0';
    }
}

/*
 * Creates a city struct from one CSV line.
 * The file starts with city, city_ascii, state_id, state_name, county_fips,
 * county_name, lat, lng, population, density, ...
 */
City *createCityFromLine(char *line) {
    City *city = malloc(sizeof(City));

    if (city == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    char cityName[100];
    char cityAscii[100];
    char stateId[10];
    char stateName[100];
    char countyFips[50];
    char countyName[100];
    char latText[50];
    char lngText[50];
    char populationText[50];

    int fieldsRead = sscanf(line,
                            "\"%99[^\"]\",\"%99[^\"]\",\"%9[^\"]\",\"%99[^\"]\",\"%49[^\"]\",\"%99[^\"]\",\"%49[^\"]\",\"%49[^\"]\",\"%49[^\"]\"",
                            cityName,
                            cityAscii,
                            stateId,
                            stateName,
                            countyFips,
                            countyName,
                            latText,
                            lngText,
                            populationText);

    if (fieldsRead < 9) {
        printf("Could not read city line correctly:\n%s\n", line);
        free(city);
        return NULL;
    }

    city->city = copyString(cityName);
    city->state = copyString(stateId);
    city->population = atoi(populationText);

    return city;
}

void printCity(City *city) {
    if (city != NULL) {
        printf("%s %s, population %d\n", city->city, city->state, city->population);
    }
}

/*
 * Creates a singly linked list node that stores a city pointer.
 */
sNode *createNode(City *city) {
    sNode *node = malloc(sizeof(sNode));

    if (node == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    node->data = city;
    node->next = NULL;

    return node;
}

/*
 * Adds a node to the front of the list.
 */
sNode *addFront(sNode *head, sNode *node) {
    if (node == NULL) {
        return head;
    }

    node->next = head;
    return node;
}

/*
 * Adds a node to the end of the list.
 */
sNode *addEnd(sNode *head, sNode *node) {
    if (node == NULL) {
        return head;
    }

    if (head == NULL) {
        return node;
    }

    sNode *current = head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = node;
    node->next = NULL;

    return head;
}

/*
 * Gets the n-th node. n starts at 1.
 */
sNode *getNthNode(sNode *head, int n) {
    if (n <= 0) {
        return NULL;
    }

    sNode *current = head;
    int position = 1;

    while (current != NULL && position < n) {
        current = current->next;
        position++;
    }

    return current;
}

/*
 * Deletes a given node from the list.
 * This removes the node and also frees the city data inside it.
 */
sNode *deleteNode(sNode *head, sNode *target) {
    if (head == NULL || target == NULL) {
        return head;
    }

    if (head == target) {
        sNode *newHead = head->next;
        City *city = (City *) head->data;
        free(city->city);
        free(city->state);
        free(city);
        free(head);
        return newHead;
    }

    sNode *current = head;

    while (current->next != NULL && current->next != target) {
        current = current->next;
    }

    if (current->next == target) {
        current->next = target->next;

        City *city = (City *) target->data;
        free(city->city);
        free(city->state);
        free(city);
        free(target);
    }

    return head;
}

/*
 * Gets the length of the list.
 */
int getLength(sNode *head) {
    int length = 0;
    sNode *current = head;

    while (current != NULL) {
        length++;
        current = current->next;
    }

    return length;
}

/*
 * Reverses the singly linked list.
 */
sNode *reverseList(sNode *head) {
    sNode *previous = NULL;
    sNode *current = head;
    sNode *next = NULL;

    while (current != NULL) {
        next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }

    return previous;
}

/*
 * Moves the n-th node to the front of the list.
 * This re-links the node without freeing its data.
 */
sNode *moveNthToFront(sNode *head, int n) {
    if (head == NULL || n <= 1) {
        return head;
    }

    sNode *previous = NULL;
    sNode *current = head;
    int position = 1;

    while (current != NULL && position < n) {
        previous = current;
        current = current->next;
        position++;
    }

    if (current == NULL) {
        return head;
    }

    previous->next = current->next;
    current->next = head;

    return current;
}

/*
 * Prints the first n cities in the list.
 */
void printFirstNCities(sNode *head, int n) {
    sNode *current = head;
    int count = 0;

    while (current != NULL && count < n) {
        City *city = (City *) current->data;
        printCity(city);

        current = current->next;
        count++;
    }
}

/*
 * Reads the first 20 cities from the CSV file into a linked list.
 */
sNode *readCities(char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Could not open file: %s\n", filename);
        exit(1);
    }

    char line[MAX_LINE];
    sNode *head = NULL;
    int count = 0;

    // Discard header line.
    fgets(line, MAX_LINE, file);

    while (fgets(line, MAX_LINE, file) != NULL && count < NUM_CITIES) {
        City *city = createCityFromLine(line);
        sNode *node = createNode(city);

        head = addEnd(head, node);
        count++;
    }

    fclose(file);

    return head;
}

/*
 * Frees the whole linked list.
 */
void freeList(sNode *head) {
    sNode *current = head;

    while (current != NULL) {
        sNode *next = current->next;

        City *city = (City *) current->data;
        free(city->city);
        free(city->state);
        free(city);
        free(current);

        current = next;
    }
}

/*
 * Main command loop.
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./single filename\n");
        return 1;
    }

    sNode *list = readCities(argv[1]);

    char command[MAX_INPUT];

    while (1) {
        printf("size, delete, reverse, get, or print: ");
        scanf("%s", command);

        if (strcmp(command, "size") == 0) {
            printf("Size is %d\n", getLength(list));
        } else if (strcmp(command, "delete") == 0) {
            int n;
            printf("Enter a number: ");
            scanf("%d", &n);

            sNode *target = getNthNode(list, n);
            list = deleteNode(list, target);
        } else if (strcmp(command, "reverse") == 0) {
            list = reverseList(list);
        } else if (strcmp(command, "get") == 0) {
            int n;
            printf("Enter a number: ");
            scanf("%d", &n);

            list = moveNthToFront(list, n);
        } else if (strcmp(command, "print") == 0) {
            int n;
            printf("Enter a number: ");
            scanf("%d", &n);

            printFirstNCities(list, n);
        } else {
            break;
        }
    }

    freeList(list);

    return 0;
}