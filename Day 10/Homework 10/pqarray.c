#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INITIAL_CAPACITY 16
#define MAX_VALUE 100000000

/* Reads a nonnegative integer. Returns 0 if the user presses ENTER or EOF. */
static int read_count(const char *prompt, size_t *count) {
  char line[128];
  char *end;
  long value;

  printf("%s", prompt);
  fflush(stdout);

  if (fgets(line, sizeof(line), stdin) == NULL || line[0] == '\n') {
    return 0;
  }

  value = strtol(line, &end, 10);
  while (*end == ' ' || *end == '\t') {
    end++;
  }

  if (end == line || (*end != '\n' && *end != '\0') || value < 0) {
    printf("Please enter a nonnegative whole number.\n\n");
    return read_count(prompt, count);
  }

  *count = (size_t) value;
  return 1;
}

/* Comparator used by qsort to sort integers from smallest to largest. */
static int compare_ints(const void *left, const void *right) {
  int a = *(const int *) left;
  int b = *(const int *) right;

  if (a < b) {
    return -1;
  }
  if (a > b) {
    return 1;
  }
  return 0;
}

/* Makes sure the backing array can hold at least required elements. */
static void ensure_capacity(int **queue, size_t *capacity, size_t required) {
  int *resized;
  size_t new_capacity = *capacity;

  if (required <= *capacity) {
    return;
  }

  while (new_capacity < required) {
    new_capacity *= 2;
  }

  resized = realloc(*queue, new_capacity * sizeof(int));
  if (resized == NULL) {
    free(*queue);
    fprintf(stderr, "Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }

  *queue = resized;
  *capacity = new_capacity;
}

/* Adds random values, then sorts the entire priority queue. */
static void enqueue_random(int **queue, size_t *size, size_t *capacity,
                           size_t amount) {
  size_t i;

  ensure_capacity(queue, capacity, *size + amount);

  for (i = 0; i < amount; i++) {
    (*queue)[*size + i] = rand() % MAX_VALUE;
  }

  *size += amount;
  qsort(*queue, *size, sizeof(int), compare_ints);
}

/* Removes and prints the smallest values from the front of the array. */
static void dequeue_smallest(int *queue, size_t *size, size_t amount) {
  size_t i;
  size_t removed = amount;

  if (removed > *size) {
    removed = *size;
  }

  for (i = 0; i < removed; i++) {
    printf("%d", queue[i]);
    if (i + 1 < removed) {
      printf("  ");
    }
  }
  printf("\n\n");

  memmove(queue, queue + removed, (*size - removed) * sizeof(int));
  *size -= removed;
}

int main(void) {
  int *queue = malloc(INITIAL_CAPACITY * sizeof(int));
  size_t size = 0;
  size_t capacity = INITIAL_CAPACITY;
  size_t amount;

  if (queue == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return EXIT_FAILURE;
  }

  srand((unsigned int) time(NULL));

  while (1) {
    if (!read_count("How many numbers to add:  ", &amount)) {
      break;
    }
    printf("\n");

    enqueue_random(&queue, &size, &capacity, amount);

    if (!read_count("How many numbers to remove:  ", &amount)) {
      break;
    }
    printf("\n");

    dequeue_smallest(queue, &size, amount);

    if (size == 0) {
      break;
    }
  }

  free(queue);
  return EXIT_SUCCESS;
}
