#ifndef UTILS_H
#define UTILS_H

enum {
	SUCCESS = 0,
    ARGUMENTS_ERROR = 1,
	FILE_ERROR = 2,
	INPUT_FORMAT_ERROR = 3,
	ALGORITHM_ERROR = 4,
	MEMORY_ERROR = 5,
};

typedef struct List {
  int data;
  struct List *next;
} List;

int  list_contains(List *head, int value);
void list_prepend(List **head, int value);
void list_free(List *head);

#endif
