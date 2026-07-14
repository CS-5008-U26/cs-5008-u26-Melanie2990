/* BST Sort
 *
 * This program uses a binary search tree to sort an array.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TO_PRINT 100
#define MAX_VALUE 100000000

/* test case */

int test1[] =       {3};
int expected1[] =   {3};
int n1 =            1;
int test2[] =       {5, 3};
int expected2[] =   {3, 5};
int n2 =            2;
int test3[] =       {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int expected3[] =   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int n3 =            10;
int test4[] =       {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
int expected4[] =   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int n4 =            10;
int test5[] =       {2, 4, 6, 8, 10, 1, 3, 5, 7, 9};
int expected5[] =   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int n5 =            10;
int test6[] =       {1, 2, 3, 2, 2};
int expected6[] =   {1, 2, 2, 2, 3};
int n6 =            5;

/* B I N A R Y   S E A R C H   T R E E */

typedef struct bstnode {
    int value;
    struct bstnode *left;
    struct bstnode *right;
} BstNode;

// insertNode - insert one value into the BST
BstNode *insertNode(BstNode *root, int value) {
    if (root == NULL) {
        root = malloc(sizeof(BstNode));
        if (root == NULL) {
            printf("malloc failed\n");
            exit(1);
        }

        root->value = value;
        root->left = NULL;
        root->right = NULL;
    } else if (value < root->value) {
        root->left = insertNode(root->left, value);
    } else {
        // Equal values go into the right subtree.
        root->right = insertNode(root->right, value);
    }

    return root;
}

// treeToArray - use in-order traversal to put values back into the array
void treeToArray(BstNode *root, int a[], int *index) {
    if (root != NULL) {
        treeToArray(root->left, a, index);
        a[*index] = root->value;
        (*index)++;
        treeToArray(root->right, a, index);
    }
}

// freeBst - free children before freeing the current node
void freeBst(BstNode *root) {
    if (root != NULL) {
        freeBst(root->left);
        freeBst(root->right);
        free(root);
    }
}

/* Y O U R   S O R T   F U N C T I O N */

void sortarray(int a[], int n) {
    // BST Sort: build a BST, copy it back in order, and free the tree.
    BstNode *root = NULL;
    int index = 0;

    for (int i = 0; i < n; i++) {
        root = insertNode(root, a[i]);
    }

    treeToArray(root, a, &index);
    freeBst(root);
}

/* U T I L I T Y   F U N C T I O N S */

// printarray = print an array of ints
void printarray(int a[], int n) {
    int numberToPrint = (n < MAX_TO_PRINT) ? n : MAX_TO_PRINT;

    for (int i = 0; i < numberToPrint; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

// genarray - return an array of random ints on the heap
int *genarray(int numberofelements) {
    int *result = malloc(numberofelements * sizeof(int));

    if (result == NULL) {
        printf("malloc failed\n");
    } else {
        for (int i = 0; i < numberofelements; i++) {
            long long int r1 = rand();
            long long int r2 = rand();
            long long int randomValue = (r1 << 14) + (r2 & 0x3fff);
            result[i] = (int)(randomValue % MAX_VALUE);
        }
    }

    return result;
}

// timedsort - run a sort and print the elapsed time
void timedsort(int a[], int n) {
    clock_t startTime = clock();
    sortarray(a, n);
    clock_t endTime = clock();
    double elapsedTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    printf("Result: ");
    printarray(a, n);
    printf("Elapsed time: %f seconds\n\n", elapsedTime);
}

// testsort - run a test case with known expected results
void testsort(int a[], int n, int expected[]) {
    printf("Test case: ");
    printarray(a, n);
    sortarray(a, n);
    printf("Result: ");
    printarray(a, n);

    int ok = 1;
    for (int i = 0; ok && i < n; i++) {
        ok = a[i] == expected[i];
    }

    printf("%s\n\n", ok ? "PASSED" : "FAILED");
}

/* M A I N   F U N C T I O N */

int main(void) {
    char buffer[100];
    int nelements;
    int *data;

    srand((unsigned int)time(NULL));

    // Run test cases.
    testsort(test1, n1, expected1);
    testsort(test2, n2, expected2);
    testsort(test3, n3, expected3);
    testsort(test4, n4, expected4);
    testsort(test5, n5, expected5);
    testsort(test6, n6, expected6);

    // Run timed sorts with random data.
    int keepgoing = 1;
    while (keepgoing) {
        printf("Random data - How many elements? ");
        fgets(buffer, 100, stdin);

        if (buffer[0] == '\n') {
            keepgoing = 0;
        } else {
            nelements = atoi(buffer);
            if (nelements <= 0) {
                printf("Must be a positive number of elements\n");
            } else {
                data = genarray(nelements);
                if (data != NULL) {
                    timedsort(data, nelements);
                    free(data);
                }
            }
        }
    }

    // Run timed sorts with data already sorted from 0 to N - 1.
    keepgoing = 1;
    while (keepgoing) {
        printf("Sorted data - How many elements? ");
        fgets(buffer, 100, stdin);

        if (buffer[0] == '\n') {
            keepgoing = 0;
        } else {
            nelements = atoi(buffer);
            if (nelements <= 0) {
                printf("Must be a positive number of elements\n");
            } else {
                data = malloc(nelements * sizeof(int));
                if (data == NULL) {
                    printf("malloc failed\n");
                } else {
                    for (int i = 0; i < nelements; i++) {
                        data[i] = i;
                    }
                    timedsort(data, nelements);
                    free(data);
                }
            }
        }
    }

    // Run timed sorts with data sorted in reverse order.
    keepgoing = 1;
    while (keepgoing) {
        printf("Reverse data - How many elements? ");
        fgets(buffer, 100, stdin);

        if (buffer[0] == '\n') {
            keepgoing = 0;
        } else {
            nelements = atoi(buffer);
            if (nelements <= 0) {
                printf("Must be a positive number of elements\n");
            } else {
                data = malloc(nelements * sizeof(int));
                if (data == NULL) {
                    printf("malloc failed\n");
                } else {
                    for (int i = 0; i < nelements; i++) {
                        data[i] = nelements - 1 - i;
                    }
                    timedsort(data, nelements);
                    free(data);
                }
            }
        }
    }

    return 0;
}
