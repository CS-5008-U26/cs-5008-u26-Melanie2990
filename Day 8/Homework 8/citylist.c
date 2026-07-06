#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 10000
#define FIELD_SIZE 512
#define MAX_FIELDS 32
#define INITIAL_CAPACITY 10

/* Stores the data needed for one city. */
typedef struct cityData {
  char name[100];
  char state[3];
  int countyFips;
  double latitude;
  double longitude;
  int population;
} City;

/* Generic vector that stores void pointers. */
typedef struct vector {
  void **data;
  int size;
  int capacity;
} Vector;

/* Binary search tree node that stores a pointer to a city. */
typedef struct treeNode {
  City *city;
  struct treeNode *left;
  struct treeNode *right;
} TreeNode;

/* Initialize an empty vector. */
void initVector(Vector *vector) {
  if (vector == NULL) {
    return;
  }

  vector->size = 0;
  vector->capacity = INITIAL_CAPACITY;
  vector->data = malloc(sizeof(void *) * vector->capacity);

  if (vector->data == NULL) {
    printf("Could not allocate vector memory.\n");
    exit(1);
  }
}

/* Add one item to the end of the vector. */
void pushVector(Vector *vector, void *item) {
  void **newData;
  int newCapacity;

  if (vector == NULL) {
    return;
  }

  if (vector->size >= vector->capacity) {
    newCapacity = vector->capacity * 2;
    newData = realloc(vector->data, sizeof(void *) * newCapacity);

    if (newData == NULL) {
      printf("Could not resize vector.\n");
      exit(1);
    }

    vector->data = newData;
    vector->capacity = newCapacity;
  }

  vector->data[vector->size] = item;
  vector->size++;
}

/* Return the item at the given vector index. */
void *getVector(Vector *vector, int index) {
  if (vector == NULL || index < 0 || index >= vector->size) {
    return NULL;
  }

  return vector->data[index];
}

/* Free the vector array. The city structs are freed separately. */
void freeVector(Vector *vector) {
  if (vector == NULL) {
    return;
  }

  free(vector->data);
  vector->data = NULL;
  vector->size = 0;
  vector->capacity = 0;
}

/*
 * Try several possible file locations so the program can run from VSCode
 * without needing command-line arguments.
 */
FILE *openCityFile(void) {
  FILE *file;

  file = fopen("../../Resources/uscities.csv", "r");
  if (file != NULL) {
    return file;
  }

  file = fopen("../Resources/uscities.csv", "r");
  if (file != NULL) {
    return file;
  }

  file = fopen("Resources/uscities.csv", "r");
  if (file != NULL) {
    return file;
  }

  file = fopen("uscities.csv", "r");
  if (file != NULL) {
    return file;
  }

  return NULL;
}

/*
 * Split a CSV line into fields.
 * This handles commas inside quoted fields.
 */
int splitCsvLine(char line[], char fields[][FIELD_SIZE], int maxFields) {
  int field = 0;
  int pos = 0;
  int inQuotes = 0;
  int i = 0;

  while (line[i] != '\0' && field < maxFields) {
    char ch = line[i];

    if (ch == '"') {
      if (inQuotes && line[i + 1] == '"') {
        if (pos < FIELD_SIZE - 1) {
          fields[field][pos] = '"';
          pos++;
        }
        i++;
      } else {
        inQuotes = !inQuotes;
      }
    } else if (ch == ',' && !inQuotes) {
      fields[field][pos] = '\0';
      field++;
      pos = 0;
    } else if (ch != '\n' && ch != '\r') {
      if (pos < FIELD_SIZE - 1) {
        fields[field][pos] = ch;
        pos++;
      }
    }

    i++;
  }

  if (field < maxFields) {
    fields[field][pos] = '\0';
    field++;
  }

  return field;
}

/* Create a City struct from one CSV line. */
City *createCityFromLine(char line[]) {
  char fields[MAX_FIELDS][FIELD_SIZE];
  int count;
  City *city;

  count = splitCsvLine(line, fields, MAX_FIELDS);

  /*
   * Required columns:
   * column 2: name
   * column 3: state
   * column 5: county FIPS
   * column 7: latitude
   * column 8: longitude
   * column 9: population
   */
  if (count < 9) {
    return NULL;
  }

  city = malloc(sizeof(City));
  if (city == NULL) {
    printf("Could not allocate city memory.\n");
    exit(1);
  }

  strncpy(city->name, fields[1], sizeof(city->name) - 1);
  city->name[sizeof(city->name) - 1] = '\0';

  strncpy(city->state, fields[2], sizeof(city->state) - 1);
  city->state[sizeof(city->state) - 1] = '\0';

  city->countyFips = atoi(fields[4]);
  city->latitude = atof(fields[6]);
  city->longitude = atof(fields[7]);
  city->population = atoi(fields[8]);

  return city;
}

/* Print one city in the assignment format. */
void printCity(City *city) {
  if (city == NULL) {
    return;
  }

  printf("%s %s, population %d, at (%.4f, %.4f)\n",
         city->name,
         city->state,
         city->population,
         city->latitude,
         city->longitude);
}

/* Create one new BST node. */
TreeNode *createTreeNode(City *city) {
  TreeNode *node = malloc(sizeof(TreeNode));

  if (node == NULL) {
    printf("Could not allocate tree node.\n");
    exit(1);
  }

  node->city = city;
  node->left = NULL;
  node->right = NULL;

  return node;
}

/* Insert a city into a BST ordered by latitude. */
TreeNode *insertByLatitude(TreeNode *root, City *city) {
  if (city == NULL) {
    return root;
  }

  if (root == NULL) {
    return createTreeNode(city);
  }

  if (city->latitude < root->city->latitude) {
    root->left = insertByLatitude(root->left, city);
  } else {
    root->right = insertByLatitude(root->right, city);
  }

  return root;
}

/*
 * Insert a city into a BST ordered by county FIPS code.
 * Duplicate FIPS codes are ignored.
 */
TreeNode *insertByFips(TreeNode *root, City *city, int *inserted) {
  if (city == NULL) {
    return root;
  }

  if (root == NULL) {
    if (inserted != NULL) {
      *inserted = 1;
    }
    return createTreeNode(city);
  }

  if (city->countyFips < root->city->countyFips) {
    root->left = insertByFips(root->left, city, inserted);
  } else if (city->countyFips > root->city->countyFips) {
    root->right = insertByFips(root->right, city, inserted);
  }

  return root;
}

/*
 * Traverse the BST in order.
 * Because this is a BST, inorder traversal creates a sorted array.
 */
void inorderToArray(TreeNode *root, City *array[], int *index) {
  if (root == NULL || array == NULL || index == NULL) {
    return;
  }

  inorderToArray(root->left, array, index);
  array[*index] = root->city;
  (*index)++;
  inorderToArray(root->right, array, index);
}

/* Use linear search to find New York in an array of city pointers. */
int linearSearchNewYork(City *array[], int size) {
  int i;

  if (array == NULL) {
    return -1;
  }

  for (i = 0; i < size; i++) {
    if (array[i] != NULL && strcmp(array[i]->name, "New York") == 0) {
      return i;
    }
  }

  return -1;
}

/* Use binary search to find a FIPS code in a sorted array. */
int binarySearchFips(City *array[], int size, int targetFips) {
  int low = 0;
  int high = size - 1;

  while (low <= high) {
    int mid = low + (high - low) / 2;

    if (array[mid]->countyFips == targetFips) {
      return mid;
    } else if (array[mid]->countyFips < targetFips) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  return -1;
}

/* Free all BST nodes. This does not free the city structs. */
void freeTree(TreeNode *root) {
  if (root == NULL) {
    return;
  }

  freeTree(root->left);
  freeTree(root->right);
  free(root);
}

/* Free the city structs stored in the vector. */
void freeCities(Vector *cities) {
  int i;

  if (cities == NULL) {
    return;
  }

  for (i = 0; i < cities->size; i++) {
    free(cities->data[i]);
  }
}

int main(void) {
  Vector cities;
  FILE *file;
  char line[LINE_SIZE];
  int requestedCities;
  int readCities = 0;
  int i;

  TreeNode *latitudeTree = NULL;
  City **latitudeArray;
  int latitudeIndex = 0;
  int newYorkIndex;

  TreeNode *fipsTree = NULL;
  City **fipsArray;
  int fipsIndex = 0;
  int uniqueFipsCount = 0;
  int fipsSearchIndex;

  initVector(&cities);

  printf("How many cities: ");
  if (scanf("%d", &requestedCities) != 1 || requestedCities <= 0) {
    printf("Please enter a positive number.\n");
    freeVector(&cities);
    return 1;
  }

  file = openCityFile();
  if (file == NULL) {
    printf("Could not open uscities.csv.\n");
    printf("Make sure the Resources folder is in the correct location.\n");
    freeVector(&cities);
    return 1;
  }

  /* Skip the header line in the CSV file. */
  fgets(line, LINE_SIZE, file);

  /*
   * Task 1:
   * Read the requested number of cities and store city pointers in the vector.
   */
  while (readCities < requestedCities && fgets(line, LINE_SIZE, file) != NULL) {
    City *city = createCityFromLine(line);

    if (city != NULL) {
      pushVector(&cities, city);
      readCities++;
    }
  }

  fclose(file);

  /* Print the cities in the same order they were read. */
  for (i = 0; i < cities.size; i++) {
    City *city = getVector(&cities, i);
    printCity(city);
  }

  /*
   * Task 2:
   * Build a BST ordered by latitude.
   */
  for (i = 0; i < cities.size; i++) {
    City *city = getVector(&cities, i);
    latitudeTree = insertByLatitude(latitudeTree, city);
  }

  latitudeArray = malloc(sizeof(City *) * cities.size);
  if (latitudeArray == NULL) {
    printf("Could not allocate latitude array.\n");
    freeTree(latitudeTree);
    freeCities(&cities);
    freeVector(&cities);
    return 1;
  }

  /*
   * Use inorder traversal to copy the cities into an array sorted by latitude.
   * Then use linear search to find New York.
   */
  inorderToArray(latitudeTree, latitudeArray, &latitudeIndex);
  newYorkIndex = linearSearchNewYork(latitudeArray, latitudeIndex);

  if (newYorkIndex >= 0) {
    printf("By latitude, New York is index %d\n", newYorkIndex);
  } else {
    printf("New York was not found by latitude.\n");
  }

  /*
   * Task 3:
   * Build a BST ordered by county FIPS code.
   * Duplicate FIPS codes are ignored.
   */
  for (i = 0; i < cities.size; i++) {
    int inserted = 0;
    City *city = getVector(&cities, i);

    fipsTree = insertByFips(fipsTree, city, &inserted);

    if (inserted) {
      uniqueFipsCount++;
    }
  }

  fipsArray = malloc(sizeof(City *) * uniqueFipsCount);
  if (fipsArray == NULL) {
    printf("Could not allocate FIPS array.\n");
    free(latitudeArray);
    freeTree(latitudeTree);
    freeTree(fipsTree);
    freeCities(&cities);
    freeVector(&cities);
    return 1;
  }

  /*
   * Use inorder traversal to copy the cities into an array sorted by FIPS code.
   * Then use binary search to find FIPS code 36081.
   */
  inorderToArray(fipsTree, fipsArray, &fipsIndex);
  fipsSearchIndex = binarySearchFips(fipsArray, fipsIndex, 36081);

  if (fipsSearchIndex >= 0) {
    printf("By FIPS code, %s is index %d\n",
           fipsArray[fipsSearchIndex]->name,
           fipsSearchIndex);
  } else {
    printf("FIPS code 36081 was not found.\n");
  }

  /* Free all allocated memory before ending the program. */
  free(latitudeArray);
  free(fipsArray);
  freeTree(latitudeTree);
  freeTree(fipsTree);
  freeCities(&cities);
  freeVector(&cities);

  return 0;
}