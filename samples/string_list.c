#include <stdio.h>
#include <lineards.h>
#include <string.h>

#define STR_LENGTH 20

int main() {
    int i;
    char fruit[STR_LENGTH];
    LINEAR_DS * l = lds_new_list(STR_LENGTH);

    if (l == NULL) {
        printf("ERROR: unable to allocate list.\n");
        return 1;
    }

    strcpy(fruit, "banana"); // Note: fruit is already padded with 20 characters (STR_LENGTH).
    lds_insert(l, 0, fruit); // same as: lds_insert(l, 0, STRING_PAD("banana", STR_LENGTH));
    lds_insert(l, 0, STRING_PAD("apple", STR_LENGTH));
    lds_insert(l, 1, STRING_PAD("grape", STR_LENGTH));
    lds_insert(l, 3, STRING_PAD("orange", STR_LENGTH));
    lds_insert(l, 2, STRING_PAD("strawberry", STR_LENGTH));

    lds_set(l, 3, STRING_PAD("lemon", STR_LENGTH));
    lds_remove(l,2, NULL);

    for(i=0; i<lds_size(l); i++) {
        lds_get(l, i, &fruit);
        printf("%d: %s\n", i, fruit);
    }

    lds_free(l);
    return 0;
}
