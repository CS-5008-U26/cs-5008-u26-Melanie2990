#include <stdio.h>
#include <stdlib.h>
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

static void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

/* Makes sure the heap backing array can hold at least required elements. */
static void ensure_capacity(int **heap, size_t *capacity, size_t required) {
  int *resized;
  size_t new_capacity = *capacity;

  if (required <= *capacity) {
    return;
  }

  while (new_capacity < required) {
    new_capacity *= 2;
  }

  resized = realloc(*heap, new_capacity * sizeof(int));
  if (resized == NULL) {
    free(*heap);
    fprintf(stderr, "Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }

  *heap = resized;
  *capacity = new_capacity;
}

/* Moves a new value upward until the min-heap property is restored. */
static void heapify_up(int *heap, size_t index) {
  while (index > 0) {
    size_t parent = (index - 1) / 2;

    if (heap[parent] <= heap[index]) {
      break;
    }

    swap(&heap[parent], &heap[index]);
    index = parent;
  }
}

/* Moves the root downward until the min-heap property is restored. */
static void heapify_down(int *heap, size_t size, size_t index) {
  while (1) {
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    size_t smallest = index;

    if (left < size && heap[left] < heap[smallest]) {
      smallest = left;
    }
    if (right < size && heap[right] < heap[smallest]) {
      smallest = right;
    }
    if (smallest == index) {
      break;
    }

    swap(&heap[index], &heap[smallest]);
    index = smallest;
  }
}

/* Inserts one value into the min-heap. */
static void heap_insert(int **heap, size_t *size, size_t *capacity, int value) {
  ensure_capacity(heap, capacity, *size + 1);
  (*heap)[*size] = value;
  heapify_up(*heap, *size);
  (*size)++;
}

/* Removes and returns the smallest value at the root. */
static int heap_remove_min(int *heap, size_t *size) {
  int smallest = heap[0];

  swap(&heap[0], &heap[*size - 1]);
  (*size)--;

  if (*size > 0) {
    heapify_down(heap, *size, 0);
  }

  return smallest;
}

static void enqueue_random(int **heap, size_t *size, size_t *capacity,
                           size_t amount) {
  size_t i;

  for (i = 0; i < amount; i++) {
    heap_insert(heap, size, capacity, rand() % MAX_VALUE);
  }
}

static void dequeue_smallest(int *heap, size_t *size, size_t amount) {
  size_t i;
  size_t removed = amount;

  if (removed > *size) {
    removed = *size;
  }

  for (i = 0; i < removed; i++) {
    printf("%d", heap_remove_min(heap, size));
    if (i + 1 < removed) {
      printf("  ");
    }
  }
  printf("\n\n");
}

int main(void) {
  int *heap = malloc(INITIAL_CAPACITY * sizeof(int));
  size_t size = 0;
  size_t capacity = INITIAL_CAPACITY;
  size_t amount;

  if (heap == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return EXIT_FAILURE;
  }

  srand((unsigned int) time(NULL));

  while (1) {
    if (!read_count("How many numbers to add:  ", &amount)) {
      break;
    }
    printf("\n");

    enqueue_random(&heap, &size, &capacity, amount);

    if (!read_count("How many numbers to remove:  ", &amount)) {
      break;
    }
    printf("\n");

    dequeue_smallest(heap, &size, amount);

    if (size == 0) {
      break;
    }
  }

  free(heap);
  return EXIT_SUCCESS;
}
