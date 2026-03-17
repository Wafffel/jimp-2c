#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int list_contains(List *head, int value) {
  for (List *cur = head; cur != NULL; cur = cur->next)
    if (cur->data == value)
      return 1;
  return 0;
}

int list_prepend(List **head, int value) {
  List *node = (List *)malloc(sizeof(List));
  if (node == NULL) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    return MEMORY_ERROR;
  }
  node->data = value;
  node->next = *head;
  *head = node;
  return SUCCESS;
}

void list_free(List *head) {
  while (head != NULL) {
    List *tmp = head->next;
    free(head);
    head = tmp;
  }
}
