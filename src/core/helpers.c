#include "helpers.h"
#include "../containers/list.h"

#include <stdio.h>

void print_list(struct List *list) {
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