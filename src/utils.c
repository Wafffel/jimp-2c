#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

int list_contains(List *head, int value)
{
  for (List *cur = head; cur != NULL; cur = cur->next)
    if (cur->data == value) return 1;
  return 0;
}

void list_prepend(List **head, int value)
{
  List *node = (List *)malloc(sizeof(List));
  node->data = value;
  node->next = *head;
  *head = node;
}

void list_free(List *head)
{
  while (head != NULL)
  {
    List *tmp = head->next;
    free(head);
    head = tmp;
  }
}
