#ifndef list_h
#define list_h

typedef struct list_t list_t;

struct list_t {
	void*	data;
	list_t*	next;
};

list_t* list_new();
void list_add(list_t* list, void* data);
void list_free(list_t** list);

#endif
