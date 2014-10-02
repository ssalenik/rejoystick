#include "list.h"
#include <stdlib.h>
#include <stdio.h>

list_t* list_new()
{
	list_t* list = malloc(sizeof(list_t));
	list->data = list;
	list->next = NULL;
	return list;
}

void list_add(list_t* list, void* data)
{
	list_t* element = malloc(sizeof(list_t));
	element->data = data;
	element->next = NULL;
	if (list == list->data) {
		list->data = element;
		list->next = element;
	} else {
		((list_t*)(list->data))->next = element;
		list->data = element;
	}
}

void list_free(list_t** list)
{
	list_t* it = *list;
	list_t* tmp;

	/* If true, then there's no elements added */
	if (it->data == it) {
		free(*list);
		return ;
	}

	/* First elements data contains only the address to the last element */
	it = it->next;

	while (it != NULL) {
		free(it->data);
		tmp = it;
		it = it->next;
		free(tmp);
	}
	free(*list);
}
