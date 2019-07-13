#include <stdlib.h>
#include "SymbolTable.h"
#include "slist.h"

static snode* snode_create(void* data)
{
	snode* node = (snode*)malloc(sizeof(snode));
	if (node) {
		node->data = data;
		node->next = NULL;
	}

	return node;
}

slist* slist_create(void)
{
	slist* list = malloc(sizeof(slist));
	if (list) {
		list->head = NULL;
		list->tail = NULL;
		list->count = 0;
	}

	return list;
}

void slist_empty(slist* list)
{
	snode* node, * temp;
	node = list->head;
	while (node != NULL) {
		temp = node->next;
		delete_ID_Information((ID_Information*)node->data);
		free(node);
		node = temp;
	}
}

void slist_delete(slist* list)
{
	if (list) {
		slist_empty(list);
		free(list);
	}
}

void slist_add_tail(slist* list, void* data)
{
	snode* node = snode_create(data);
	if (list->head == NULL) {
		/* Adding the first node */
		list->head = node;
		list->tail = node;
	}
	else {
		list->tail->next = node;
		list->tail = node;
	}
	list->count++;
}

void slist_add_head(slist* list, void* data)
{
	snode* node = snode_create(data);
	if (list->tail == NULL) {
		/* Adding the first node */
		list->head = node;
		list->tail = node;
	}
	else {
		node->next = list->head;
		list->head = node;
	}
	list->count++;
}

void* slist_remove_head(slist* list)
{
	void* data = NULL;

	if (list->head) {
		snode* temp = list->head;
		if (list->head->next) {
			list->head = list->head->next;
		}
		else {
			/* List is now empty */
			list->head = NULL;
			list->tail = NULL;
		}
		data = temp->data;
		free(temp);
		list->count--;
		if (list->count == 1) {
			list->tail = list->head;
		}
	}

	return data;
}

void* slist_remove_tail(slist* list)
{
	void* data = NULL;

	if (list->tail) {
		snode* current, * previous = NULL;
		current = list->head;
		while (current->next) {
			previous = current;
			current = current->next;
		}
		data = current->data;
		free(current);
		if (previous) {
			previous->next = NULL;
		}
		else {
			/* List is now empty */
			list->head = NULL;
			list->tail = NULL;
		}
		list->count--;
		if (list->count == 1) {
			list->head = list->tail;
		}
	}

	return data;
}

void* slist_remove(slist* list, snode* node, snode* previous)
{
	void* data;
	if (node == list->head) {
		data = slist_remove_head(list);
	}
	else {
		previous->next = node->next;
		data = node->data;
		list->count--;
		if (list->count == 1) {
			list->tail = list->head;
		}
		else if (node == list->tail) {
			list->tail = previous;
		}
		free(node);
	}
	return data;
}

void slist_insert_before(slist* list, snode* node, snode* previous, void* data)
{
	if (node == list->head) {
		slist_add_head(list, data);
	}
	else {
		snode* newnode = snode_create(data);
		newnode->next = node;
		previous->next = newnode;
		list->count++;
	}
}

snode* slist_insert_after(slist* list, snode* node, void* data)
{
	snode* newnode;
	if (node == NULL) {
		slist_add_head(list, data);
		newnode = list->head;
	}
	else {
		newnode = snode_create(data);
		if (newnode) {
			newnode->next = node->next;
			node->next = newnode;
			if (node == list->tail) {
				list->tail = newnode;
			}
		}
		list->count++;
	}
	return newnode;
}

void slist_for_each(const slist* list, slist_forfn fun)
{
	snode* node;
	for (node = list->head; node != NULL; node = node->next) {
		fun(node->data);
	}
}

unsigned int slist_get_count(const slist* list)
{
	return list->count;
}

void* slistsConcat(slist* headList, slist* tailList) {
	if (headList == NULL && tailList == NULL)
		return NULL;

	if (headList == NULL)
		return tailList;

	if (tailList == NULL)
		return headList;

	slist* newList = slist_create();
	snode* node = headList->head;
	while (node != NULL)
	{
		slist_add_tail(newList, node->data);
		node = node->next;
	}

	node = tailList->head;
	while (node != NULL)
	{
		slist_add_tail(newList, node->data);
		node = node->next;
	}

	return newList;
}

//EXAMPLE PROGRAM:
/*

#include <stdio.h>
#include <string.h>

#include <slist.h>

int main(void)
{
	slist * list = slist_create();
	char * elements[] = {"A", "B", "C", "D", "E", "F"};
	const unsigned int n = sizeof(elements) / sizeof(const char*);
	unsigned int i;
	snode * node, * previous = NULL;
	unsigned int found = 0;

	// Populate the list with A, B, ..., F 
for (i = 0; i < n; i++) {
	slist_add_tail(list, elements[i]);
}

//* Insert X and Y either side of C 
for (node = list->head; node != NULL && !found; node = node->next) {
	if (strcmp((const char*)node->data, "C") == 0) {
		slist_insert_before(list, node, previous, "X");
		slist_insert_after(list, node, "Y");
		found = 1;
	}
	previous = node;
}

//* Forward iteration 
for (node = list->head; node != NULL; node = node->next) {
	printf("%s\n", (const char*)node->data);
}

slist_delete(list);

return 0;
}

*/