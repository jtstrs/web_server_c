#ifndef LIST_H
#define LIST_H

#include <stdint.h>

struct List;

struct List *create_list();
void release_list(struct List *list);

void insert_item(struct List *list, void *data, int32_t position);
void remove_item(struct List *list, int32_t position);

void add_item(struct List *list, void *data);

void push_item(struct List *list, void *data);
void pop_item(struct List *list);

void *first_item(struct List *list);
void *last_item(struct List *list);
void *get_list_item(struct List *list, int32_t position);
int32_t get_list_size(struct List *list);

#endif