#ifndef SLIST_H
#define SLIST_H

typedef struct snode {
	void* data;
	struct snode* next;
}snode;

typedef struct slist {
	snode* head;
	snode* tail;
	unsigned int count;
}slist;


typedef void (*slist_forfn)(void*);

snode* snode_create(void* data);
slist* slist_create(void);
void slist_empty(slist* list);
void slist_delete(slist* list);
void slist_add_tail(slist* list, void* data);
void slist_add_head(slist* list, void* data);
void* slist_remove_head(slist* list);
void* slist_remove_tail(slist* list);
void* slist_remove(slist* list, snode* node, snode* previous);
void slist_insert_before(slist* list, snode* node, snode* previous, void* data);
snode* slist_insert_after(slist* list, snode* node, void* data);
void slist_for_each(const slist* list, slist_forfn fun);
unsigned int slist_get_count(const slist* list);
void* slistsConcat(slist* headList, slist* tailList);
void* getNElementInList(slist* list, int n);
void printListWithNamesAndTypes(slist* listToPrint);

#endif /* SLIST_H */