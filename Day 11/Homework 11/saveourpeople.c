#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CITY_LIMIT 200
#define CHARACTER_LIMIT 200
#define MAX_NAME_LENGTH 256
#define MAX_LINE_LENGTH 8192
#define MAX_FIELDS 64

/**
 * Represents one city from the CSV file.
 */
typedef struct {
  char name[MAX_NAME_LENGTH];
  int nameLength;
  long long population;
} City;

/**
 * Compares two strings without considering uppercase or lowercase letters.
 *
 * @param first the first string
 * @param second the second string
 * @return 1 if the strings are equal, or 0 otherwise
 */
int stringsEqualIgnoreCase(const char *first, const char *second) {
  while (*first != '\0' && *second != '\0') {
    unsigned char firstCharacter = (unsigned char) *first;
    unsigned char secondCharacter = (unsigned char) *second;

    if (tolower(firstCharacter) != tolower(secondCharacter)) {
      return 0;
    }

    first++;
    second++;
  }

  return *first == '\0' && *second == '\0';
}

/**
 * Removes whitespace from the beginning and end of a string.
 *
 * @param text the string to trim
 * @return a pointer to the trimmed string
 */
char *trimWhitespace(char *text) {
  char *end;

  while (isspace((unsigned char) *text)) {
    text++;
  }

  if (*text == '\0') {
    return text;
  }

  end = text + strlen(text) - 1;

  while (end > text && isspace((unsigned char) *end)) {
    end--;
  }

  end[1] = '\0';

  return text;
}

/**
 * Parses one CSV line.
 *
 * This function handles fields surrounded by quotation marks and commas
 * inside quoted fields.
 *
 * @param line the CSV line to parse
 * @param fields an array that will contain pointers to each field
 * @param maximumFields the maximum number of fields allowed
 * @return the number of fields found
 */
int parseCsvLine(char *line, char *fields[], int maximumFields) {
  char *source = line;
  char *destination = line;
  int fieldCount = 0;
  int insideQuotes = 0;

  if (maximumFields <= 0) {
    return 0;
  }

  fields[fieldCount] = destination;
  fieldCount++;

  while (*source != '\0') {
    char currentCharacter = *source;
    source++;

    if (insideQuotes) {
      if (currentCharacter == '"') {
        if (*source == '"') {
          *destination = '"';
          destination++;
          source++;
        } else {
          insideQuotes = 0;
        }
      } else {
        *destination = currentCharacter;
        destination++;
      }
    } else {
      if (currentCharacter == '"') {
        insideQuotes = 1;
      } else if (currentCharacter == ',') {
        *destination = '\0';
        destination++;

        if (fieldCount < maximumFields) {
          fields[fieldCount] = destination;
          fieldCount++;
        }
      } else if (currentCharacter == '\n'
                 || currentCharacter == '\r') {
        break;
      } else {
        *destination = currentCharacter;
        destination++;
      }
    }
  }

  *destination = '\0';

  return fieldCount;
}

/**
 * Finds the index of a column in the CSV header.
 *
 * @param fields the CSV header fields
 * @param fieldCount the number of header fields
 * @param columnName the name of the desired column
 * @return the index of the column, or -1 if it is not found
 */
int findColumn(
    char *fields[],
    int fieldCount,
    const char *columnName
) {
  for (int index = 0; index < fieldCount; index++) {
    char *headerName = trimWhitespace(fields[index]);

    if (stringsEqualIgnoreCase(headerName, columnName)) {
      return index;
    }
  }

  return -1;
}

/**
 * Reads all valid cities from the CSV file.
 *
 * @param filename the name of the CSV file
 * @param cityArrayAddress the address where the allocated array is stored
 * @return the number of cities read, or -1 if reading fails
 */
int readCities(
    const char *filename,
    City **cityArrayAddress
) {
  FILE *inputFile = fopen(filename, "r");

  if (inputFile == NULL) {
    fprintf(stderr, "Could not open %s.\n", filename);
    return -1;
  }

  char line[MAX_LINE_LENGTH];
  char *fields[MAX_FIELDS];

  if (fgets(line, sizeof(line), inputFile) == NULL) {
    fprintf(stderr, "The CSV file is empty.\n");
    fclose(inputFile);
    return -1;
  }

  int headerFieldCount =
      parseCsvLine(line, fields, MAX_FIELDS);

  /*
   * Remove a possible UTF-8 byte order mark from the first header.
   */
  if (headerFieldCount > 0
      && (unsigned char) fields[0][0] == 0xEF
      && (unsigned char) fields[0][1] == 0xBB
      && (unsigned char) fields[0][2] == 0xBF) {
    memmove(
        fields[0],
        fields[0] + 3,
        strlen(fields[0] + 3) + 1
    );
  }

  int cityColumn =
      findColumn(fields, headerFieldCount, "city");

  /*
   * Some versions of the data use city_ascii instead of city.
   */
  if (cityColumn == -1) {
    cityColumn =
        findColumn(fields, headerFieldCount, "city_ascii");
  }

  int populationColumn =
      findColumn(fields, headerFieldCount, "population");

  if (cityColumn == -1 || populationColumn == -1) {
    fprintf(
        stderr,
        "The CSV file must contain city and population columns.\n"
    );

    fclose(inputFile);
    return -1;
  }

  int allocatedSize = 500;
  int cityCount = 0;

  City *cities =
      malloc((size_t) allocatedSize * sizeof(City));

  if (cities == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    fclose(inputFile);
    return -1;
  }

  while (fgets(line, sizeof(line), inputFile) != NULL) {
    int fieldCount =
        parseCsvLine(line, fields, MAX_FIELDS);

    if (cityColumn >= fieldCount
        || populationColumn >= fieldCount) {
      continue;
    }

    char *cityName =
        trimWhitespace(fields[cityColumn]);

    char *populationText =
        trimWhitespace(fields[populationColumn]);

    if (*cityName == '\0' || *populationText == '\0') {
      continue;
    }

    char *endPointer;
    long long population =
        strtoll(populationText, &endPointer, 10);

    if (endPointer == populationText || population <= 0) {
      continue;
    }

    if (cityCount == allocatedSize) {
      allocatedSize *= 2;

      City *largerArray =
          realloc(
              cities,
              (size_t) allocatedSize * sizeof(City)
          );

      if (largerArray == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(cities);
        fclose(inputFile);
        return -1;
      }

      cities = largerArray;
    }

    snprintf(
        cities[cityCount].name,
        MAX_NAME_LENGTH,
        "%s",
        cityName
    );

    cities[cityCount].nameLength =
        (int) strlen(cities[cityCount].name);

    cities[cityCount].population = population;

    cityCount++;
  }

  fclose(inputFile);

  *cityArrayAddress = cities;

  return cityCount;
}

/**
 * Compares two cities so they can be sorted from largest population
 * to smallest population.
 *
 * @param firstAddress a pointer to the first city
 * @param secondAddress a pointer to the second city
 * @return a value used by qsort
 */
int compareCitiesByPopulation(
    const void *firstAddress,
    const void *secondAddress
) {
  const City *firstCity = firstAddress;
  const City *secondCity = secondAddress;

  if (firstCity->population < secondCity->population) {
    return 1;
  }

  if (firstCity->population > secondCity->population) {
    return -1;
  }

  return strcmp(firstCity->name, secondCity->name);
}

/**
 * Uses zero-one knapsack dynamic programming to find the maximum
 * population that can be saved.
 *
 * @param cities the array containing the largest cities
 * @param cityCount the number of cities being considered
 * @param selected records which cities are selected
 * @return the maximum total population
 */
long long findMaximumPopulation(
    const City cities[],
    int cityCount,
    int selected[]
) {
  long long dynamicTable[CITY_LIMIT + 1]
                            [CHARACTER_LIMIT + 1] = {{0}};

  for (int cityNumber = 1;
       cityNumber <= cityCount;
       cityNumber++) {

    int cityLength =
        cities[cityNumber - 1].nameLength;

    long long cityPopulation =
        cities[cityNumber - 1].population;

    for (int capacity = 0;
         capacity <= CHARACTER_LIMIT;
         capacity++) {

      long long populationWithoutCity =
          dynamicTable[cityNumber - 1][capacity];

      dynamicTable[cityNumber][capacity] =
          populationWithoutCity;

      if (cityLength <= capacity) {
        long long populationWithCity =
            dynamicTable[cityNumber - 1]
                        [capacity - cityLength]
            + cityPopulation;

        if (populationWithCity
            > populationWithoutCity) {
          dynamicTable[cityNumber][capacity] =
              populationWithCity;
        }
      }
    }
  }

  /*
   * Work backward through the dynamic programming table to determine
   * which cities produced the maximum population.
   */
  int remainingCapacity = CHARACTER_LIMIT;

  for (int cityNumber = cityCount;
       cityNumber > 0;
       cityNumber--) {

    if (dynamicTable[cityNumber][remainingCapacity]
        != dynamicTable[cityNumber - 1]
                       [remainingCapacity]) {

      selected[cityNumber - 1] = 1;

      remainingCapacity -=
          cities[cityNumber - 1].nameLength;
    }
  }

  return dynamicTable[cityCount][CHARACTER_LIMIT];
}

/**
 * Prints the selected cities and the final totals.
 *
 * @param cities the city array
 * @param cityCount the number of cities considered
 * @param selected records which cities were selected
 * @param maximumPopulation the maximum total population
 */
void printResults(
    const City cities[],
    int cityCount,
    const int selected[],
    long long maximumPopulation
) {
  int selectedCityCount = 0;
  int totalCharacters = 0;

  printf("Cities selected for rescue:\n\n");

  for (int index = 0; index < cityCount; index++) {
    if (selected[index] == 1) {
      printf(
          "%s - population %lld - %d characters\n",
          cities[index].name,
          cities[index].population,
          cities[index].nameLength
      );

      selectedCityCount++;
      totalCharacters += cities[index].nameLength;
    }
  }

  printf("\nNumber of selected cities: %d\n",
         selectedCityCount);

  printf("Total name characters: %d\n",
         totalCharacters);

  printf("Maximum total population: %lld\n",
         maximumPopulation);
}

/**
 * Runs the Aliens Attack city rescue program.
 *
 * @return 0 if successful, or 1 if an error occurs
 */
int main(void) {
  City *allCities = NULL;

  int totalCityCount =
      readCities("uscities.csv", &allCities);

  if (totalCityCount <= 0) {
    free(allCities);
    return 1;
  }

  /*
   * Sort every city by population, then only use the first 200.
   */
  qsort(
      allCities,
      (size_t) totalCityCount,
      sizeof(City),
      compareCitiesByPopulation
  );

  int cityCount = totalCityCount;

  if (cityCount > CITY_LIMIT) {
    cityCount = CITY_LIMIT;
  }

  int selected[CITY_LIMIT] = {0};

  long long maximumPopulation =
      findMaximumPopulation(
          allCities,
          cityCount,
          selected
      );

  printResults(
      allCities,
      cityCount,
      selected,
      maximumPopulation
  );

  free(allCities);

  return 0;
}