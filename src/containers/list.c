#include "list.h"

#include <string.h>

typedef struct ListNode ListNode;

struct ListNode {
    void *data;
    ListNode *next;
    ListNode *prev;
};

struct List {
    ListNode *head;
    size_t size;

    void (*deleter)(void *);
};


List *create_list(void (*deleter)(void *)) {
    List *list = (List *) malloc(sizeof(List));

    if (!list) {
        return NULL;
    }

    ListNode *sentinel_node = (ListNode *) malloc(sizeof(ListNode));

    if (!sentinel_node) {
        free(list);
        return NULL;
    }

    memset((char *) sentinel_node, 0, sizeof(ListNode));

    list->head = sentinel_node;
    list->size = 0;

    list->deleter = deleter ? deleter : free;

    return list;
}

void release_list(List *list) {
    ListNode *forwarder = list->head;
    while (forwarder) {
        ListNode *temp = forwarder->next;

        list->deleter(forwarder->data);
        free(forwarder);

        forwarder = temp;
    }
    free(list);
}

size_t get_list_size(List *list) {
    if (!list) {
        return 0;
    }
    return list->size;
}

void insert_list_item(List *list, void *data, size_t position) {
    if (!list) {
        return;
    }

    if (position < 0 || position > list->size) {
        return;
    }

    ListNode *forwarder = list->head;

    for (size_t index = 0; index < position; ++index) {
        forwarder = forwarder->next;
    }

    ListNode *next_item = (ListNode *) malloc(sizeof(ListNode));

    if (!next_item) {
        return;
    }

    next_item->data = data;

    ListNode *current_next = forwarder->next;

    next_item->prev = forwarder;
    next_item->next = current_next;

    if (current_next) {
        current_next->prev = next_item;
    }

    forwarder->next = next_item;

    list->size++;
}

void add_list_item(List *list, void *data) {
    insert_list_item(list, data, 0);
}

void push_list_item(List *list, void *data) {
    insert_list_item(list, data, list->size);
}

void pop_list_item(List *list) {
    remove_list_item(list, list->size - 1);
}

void *get_first_list_item(List *list) {
    if (!list) {
        return NULL;
    }

    ListNode *first_node = list->head->next;

    if (!first_node) {
        return NULL;
    }

    return first_node->data;
}

void *get_last_list_item(List *list) {
    if (!list) {
        return NULL;
    }

    if (!get_list_size(list)) {
        return NULL;
    }

    ListNode *forwarder = list->head->next;
    while (forwarder && forwarder->next) {
        forwarder = forwarder->next;
    }

    return forwarder->data;
}

void *get_list_item(List *list, size_t position) {
    if (!list) {
        return NULL;
    }

    if (position < 0 || position >= list->size) {
        return NULL;
    }

    ListNode *forwarder = list->head->next;

    for (size_t index = 0; index < position; ++index) {
        forwarder = forwarder->next;
    }

    return forwarder->data;
}

void remove_list_item(List *list, size_t position) {
    if (!list) {
        return;
    }

    if (position < 0 || position >= list->size) {
        return;
    }

    ListNode *forwarder = list->head;

    for (size_t index = 0; index <= position; ++index) {
        forwarder = forwarder->next;
    }

    ListNode *forwarder_next = forwarder->next;
    ListNode *forwarder_prev = forwarder->prev;

    if (forwarder_next) {
        forwarder_prev->next = forwarder_next;
        forwarder_next->prev = forwarder_prev;
    } else {
        forwarder_prev->next = NULL;
    }

    list->deleter(forwarder->data);
    free(forwarder);

    list->size--;
}