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

typedef struct doubleNode {
    void *data;
    struct doubleNode *next;
    struct doubleNode *prev;
} dNode;

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
 * Creates a city struct from one CSV line.
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

/*
 * Prints one city.
 */
void printCity(City *city) {
    if (city != NULL) {
        printf("%s %s, population %d\n", city->city, city->state, city->population);
    }
}

/*
 * Creates a doubly linked list node that stores a city pointer.
 */
dNode *createNode(City *city) {
    dNode *node = malloc(sizeof(dNode));

    if (node == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    node->data = city;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

/*
 * Adds a node to the front of the list.
 */
dNode *addFront(dNode *head, dNode *node) {
    if (node == NULL) {
        return head;
    }

    node->prev = NULL;
    node->next = head;

    if (head != NULL) {
        head->prev = node;
    }

    return node;
}

/*
 * Adds a node to the end of the list.
 */
dNode *addEnd(dNode *head, dNode *node) {
    if (node == NULL) {
        return head;
    }

    node->next = NULL;

    if (head == NULL) {
        node->prev = NULL;
        return node;
    }

    dNode *current = head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = node;
    node->prev = current;

    return head;
}

/*
 * Gets the n-th node. n starts at 1.
 */
dNode *getNthNode(dNode *head, int n) {
    if (n <= 0) {
        return NULL;
    }

    dNode *current = head;
    int position = 1;

    while (current != NULL && position < n) {
        current = current->next;
        position++;
    }

    return current;
}

/*
 * Deletes a given node from the list.
 */
dNode *deleteNode(dNode *head, dNode *target) {
    if (head == NULL || target == NULL) {
        return head;
    }

    if (target->prev != NULL) {
        target->prev->next = target->next;
    } else {
        head = target->next;
    }

    if (target->next != NULL) {
        target->next->prev = target->prev;
    }

    City *city = (City *) target->data;
    free(city->city);
    free(city->state);
    free(city);
    free(target);

    return head;
}

/*
 * Gets the length of the list.
 */
int getLength(dNode *head) {
    int length = 0;
    dNode *current = head;

    while (current != NULL) {
        length++;
        current = current->next;
    }

    return length;
}

/*
 * Reverses the doubly linked list.
 */
dNode *reverseList(dNode *head) {
    dNode *current = head;
    dNode *newHead = NULL;

    while (current != NULL) {
        dNode *next = current->next;

        current->next = current->prev;
        current->prev = next;

        newHead = current;
        current = next;
    }

    return newHead;
}

/*
 * Moves the n-th node to the front of the list.
 */
dNode *moveNthToFront(dNode *head, int n) {
    if (head == NULL || n <= 1) {
        return head;
    }

    dNode *target = getNthNode(head, n);

    if (target == NULL) {
        return head;
    }

    if (target->prev != NULL) {
        target->prev->next = target->next;
    }

    if (target->next != NULL) {
        target->next->prev = target->prev;
    }

    target->prev = NULL;
    target->next = head;
    head->prev = target;

    return target;
}

/*
 * Prints the first n cities in the list.
 */
void printFirstNCities(dNode *head, int n) {
    dNode *current = head;
    int count = 0;

    while (current != NULL && count < n) {
        City *city = (City *) current->data;
        printCity(city);

        current = current->next;
        count++;
    }
}

/*
 * Reads the first 20 cities from the CSV file into a doubly linked list.
 */
dNode *readCities(char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Could not open file: %s\n", filename);
        exit(1);
    }

    char line[MAX_LINE];
    dNode *head = NULL;
    int count = 0;

    // Discard header line.
    fgets(line, MAX_LINE, file);

    while (fgets(line, MAX_LINE, file) != NULL && count < NUM_CITIES) {
        City *city = createCityFromLine(line);

        if (city != NULL) {
            dNode *node = createNode(city);
            head = addEnd(head, node);
            count++;
        }
    }

    fclose(file);

    return head;
}

/*
 * Frees the whole linked list.
 */
void freeList(dNode *head) {
    dNode *current = head;

    while (current != NULL) {
        dNode *next = current->next;

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
        printf("Usage: ./double filename\n");
        return 1;
    }

    dNode *list = readCities(argv[1]);

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

            dNode *target = getNthNode(list, n);
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