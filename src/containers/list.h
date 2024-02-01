#ifndef LIST_H
#define LIST_H

#include <stdint.h>

typedef struct List List;

List *create_list(void (*deleter)(void *));
void release_list(List *list);

void insert_list_item(List *list, void *data, int32_t position);
void remove_list_item(List *list, int32_t position);

void add_list_item(List *list, void *data);

void push_list_item(List *list, void *data);
void pop_list_item(List *list);

void *get_first_list_item(List *list);
void *get_last_list_item(List *list);
void *get_list_item(List *list, int32_t position);
int32_t get_list_size(List *list);

#endif