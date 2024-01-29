#include "list.h"

#include <stdlib.h>
#include <string.h>

struct ListNode {
    void *data;
    struct ListNode *next;
    struct ListNode *prev;
};

struct List {
    struct ListNode *head;
    int32_t size;
};


struct List *create_list() {
    struct List *list = (struct List *) malloc(sizeof(struct List));

    if (!list) {
        return NULL;
    }

    struct ListNode *sentinel_node = (struct ListNode *) malloc(sizeof(struct ListNode));

    if (!sentinel_node) {
        free(list);
        return NULL;
    }

    memset((char *) sentinel_node, 0, sizeof(struct ListNode));

    list->head = sentinel_node;
    list->size = 0;

    return list;
}

void release_list(struct List *list) {
    struct ListNode *forwarder = list->head;
    while (forwarder) {
        struct ListNode *temp = forwarder->next;

        free(forwarder->data);
        free(forwarder);

        forwarder = temp;
    }
    free(list);
}

void insert_item(struct List *list, void *data, int32_t position) {
    if (!list) {
        return;
    }

    if (position < 0 || position > list->size) {
        return;
    }

    struct ListNode *forwarder = list->head;

    for (int32_t index = 0; index < position; ++index) {
        forwarder = forwarder->next;
    }

    struct ListNode *next_item = (struct ListNode *) malloc(sizeof(struct ListNode));

    if (!next_item) {
        return;
    }

    next_item->data = data;

    struct ListNode *current_next = forwarder->next;

    next_item->prev = forwarder;
    next_item->next = current_next;

    if (current_next) {
        current_next->prev = next_item;
    }

    forwarder->next = next_item;

    list->size++;
}

void add_item(struct List *list, void *data) {
    insert_item(list, data, 0);
}

void push_item(struct List *list, void *data) {
    insert_item(list, data, list->size);
}

void pop_item(struct List *list) {
    remove_item(list, list->size - 1);
}

void *first_item(struct List *list) {
    if (!list) {
        return NULL;
    }

    struct ListNode *first_node = list->head->next;

    if (!first_node) {
        return NULL;
    }

    return first_node->data;
}

void *last_item(struct List *list) {
    if (!list) {
        return NULL;
    }

    struct ListNode *forwarder = list->head->next;
    while (forwarder && forwarder->next) {
        forwarder = forwarder->next;
    }

    return forwarder->data;
}

void *get_list_item(struct List *list, int32_t position) {
    if (!list) {
        return NULL;
    }

    if (position < 0 || position >= list->size) {
        return NULL;
    }

    struct ListNode *forwarder = list->head->next;

    for (int32_t index = 0; index < position; ++index) {
        forwarder = forwarder->next;
    }

    return forwarder->data;
}

int32_t get_list_size(struct List *list) {
    if (!list) {
        return 0;
    }
    return list->size;
}

void remove_item(struct List *list, int32_t position) {
    if (!list) {
        return;
    }

    if (position < 0 || position >= list->size) {
        return;
    }

    struct ListNode *forwarder = list->head;

    for (int32_t index = 0; index <= position; ++index) {
        forwarder = forwarder->next;
    }

    struct ListNode *forwarder_next = forwarder->next;
    struct ListNode *forwarder_prev = forwarder->prev;

    if (forwarder_next) {
        forwarder_prev->next = forwarder_next;
        forwarder_next->prev = forwarder_prev;
    } else {
        forwarder_prev->next = NULL;
    }

    free(forwarder->data);
    free(forwarder);

    list->size--;
}