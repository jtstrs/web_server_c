#include "helpers.h"
#include "../containers/list.h"

#include <stdio.h>
#include <stdlib.h>

int32_t parse_int32(char *token) {
    if (!token) {
        return -1;
    }

    char *endptr;

    int32_t value = (int32_t) strtoul(token, &endptr, 10);

    if (endptr == token) {
        return -1;
    }

    if (value < 0 || value >= UINT32_MAX) {
        return -1;
    }

    return value;
}


void print_list(List *list) {
    if (!list) {
        printf("[]\n");
        return;
    }

    printf("[ ");
    for (int32_t index = 0; index < get_list_size(list); ++index) {
        // Now assume that there is an integer list
        int32_t item = *((int32_t *) get_list_item(list, index));
        printf("%d ", item);
    }
    printf("]\n");
}