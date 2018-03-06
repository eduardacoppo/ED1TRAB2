#include "list.h"
#include <stdlib.h>

struct list {
	Item *first, *last;
};

struct item {
	Item *before, *after;
	void *data;
};

//Função privadas
void freeItem(Item *item);
//Função privadas
void remove(List *list, Item *item);

List* list_new() {
	List *list = malloc(sizeof(List));
	if (list == NULL) {
		return NULL;
	}
	list->first = list->last = NULL;
	return list;
}

Item* list_newItem(void *data) {

	if (data == NULL) {
		return NULL;
	}

	Item *item = malloc(sizeof(Item));
	if (item == NULL) {
		return NULL;
	}
	item->after = item->before = NULL;
	item->data = data;
	return item;
}

void list_pushOnFirst(List *list, Item *item) {
	if (item == NULL) {
		return;
	}
	if (list == NULL) {
		return;
	}

	if (list->first == NULL) {
		list->first = list->last = item;
		return;
	}

	list->first->before = item;
	item->after = list->first;
	item->before = NULL;
	list->first = item;
	return;
}

void list_pushOnLast(List *list, Item *item) {
	if (item == NULL) {
		return;
	}
	if (list == NULL) {
		return;
	}

	if (list->first == NULL) {
		list->first = list->last = item;
		return;
	}

	list->last->after = item;
	item->before = list->last;
	item->after = NULL;
	list->last = item;
	return;
}

int list_countItems(List *list) {
	if (list == NULL) {
		return 0;
	}
	if (list->first == NULL) {
		return 0;
	}

	int count = 0;

	Item *aux = list->first;

	while (aux != NULL) {
		count++;
		aux = aux->after;
	}

	return count;
}

//int list_searchList(List *list, bool(*callback)(void*)) {
//	if (list == NULL) {
//		return;
//	}
//	if (callback == NULL) {
//		return;
//	}
//	if (list->first == NULL) {
//		return;
//	}
//
//	int index = 0;
//
//	Item *aux = list->first;
//
//	while (aux != NULL) {
//		if (callback(aux->data)) {
//			index++;
//			return index;
//		}
//		index++;
//		aux = aux->after;
//	}
//
//	return;
//}

void* list_pull(List* list, int index) {

	if (list == NULL) {
		return NULL;
	}

	if (list->first == NULL) {
		return NULL;
	}

	if (index >= list_countItems(list)) {
		return NULL;
	}

	Item *aux;
	int i;

	aux = list->first;

	for (i = 0; i < index; i++) {
		aux = aux->after;
	}

	void* data = aux->data;

	remove(list, aux);

	return data;
}

void* list_pullWithoutRemove(List* list, int index) {

	if (list == NULL) {
		return NULL;
	}

	if (list->first == NULL) {
		return NULL;
	}

	if (index >= list_countItems(list)) {
		return NULL;
	}

	Item *aux;
	int i;

	aux = list->first;

	for (i = 0; i < index; i++) {
		aux = aux->after;
	}

	return aux->data;
}

void list_order(List * list, int(*callback)(void*, void*)) {
	if ((list == NULL) || (callback == NULL)){
		return;
	}
	
	if (list->first == NULL) {
		return;
	}
	
	int nItens = list_countItems(list);

	if (nItens <= 1) {
		return;
	}

	int i, j;

	Item *item;
	void *aux;

	for (i = 0; i < nItens; i++) {
		item = list->first;
		for (j = 0; j < (nItens - 1); j++) {
			if (callback(item->data, item->after->data) < 0) {
				aux = item->after->data;
				item->after->data = item->data;
				item->data = aux;
			}
			item = item->after;
		}
	}
}

void list_print(List* list, void(callback)(void*)) {
	if ((list == NULL) || (callback == NULL)) {
		return;
	}
	if (list->first == NULL) {
		return;
	}

	Item* aux = list->first;

	while (aux != NULL) {
		callback(aux->data);
		aux = aux->after;
	}
}

void remove(List *list, Item *item) {

	if (item == NULL) {
		return;
	}

	if (list == NULL) {
		return;
	}

	if (list->first == item) {
		list->first = list->first->after;
		if(list->first != NULL)
			list->first->before = NULL;
	}
	else if (list->last == item) {
		list->last = list->last->before;
		if(list->last != NULL)
			list->last->after = NULL;
	}
	else {
		item->before->after = item->after;
		item->after->before = item->before;
	}
	
	freeItem(item);
	return;
}

void freeItem(Item *item) {

	if (item == NULL) {
		return;
	}

	free(item);
}

void list_free(List *list, void(*callback)(void*)) {

	if (list == NULL) {
		return;
	}

	if (list->first == NULL) {
		free(list);
		return;
	}

	Item *aux = list->first;

	while (true) {
		if (aux->after == NULL) {
			callback(aux->data);
			remove(list, aux);
			break;
		}
		else {
			aux = aux->after;
			callback(aux->before->data);
			remove(list, aux->before);
		}
	}

	free(list);

	return;
}