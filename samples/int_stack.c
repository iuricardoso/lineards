#include <stdio.h>
#include <lineards.h>


int main() {
    int i, e;
    LINEAR_DS * s = lds_new_vector(4, sizeof(int));

    if (s == NULL) {
        printf("ERROR: unable to allocate stack.\n");
        return 1;
    }

    e = 10;
    lds_stack_push(s, &e); // same as: lds_stack_push(s, MAKE_PTR(10));
    lds_stack_push(s, MAKE_PTR(20));
    lds_stack_push(s, MAKE_PTR(30));
    lds_stack_push(s, MAKE_PTR(40));

    while(! lds_empty(s)) {
        lds_stack_pop(s, &e);
        printf("%d\n", e);
    }

    lds_free(s);
    return 0;
}
