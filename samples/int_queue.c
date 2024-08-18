#include <stdio.h>
#include <lineards.h>


int main() {
    LINEAR_DS * q = lds_new_vector(6, sizeof(int));

    if (q == NULL) {
        printf("ERROR: unable to allocate queue.\n");
        return 1;
    }

    int e;
    e = 1;
    lds_enqueue(q, &e); // same: lds_enqueue(q, MAKE_PTR(1)))
    lds_enqueue(q, MAKE_PTR(2));
    lds_enqueue(q, MAKE_PTR(3));
    lds_enqueue(q, MAKE_PTR(4));
    lds_enqueue(q, MAKE_PTR(5));
    lds_enqueue(q, MAKE_PTR(5));

    while(! lds_empty(q)) {
        lds_dequeue(q, &e);
        printf("%d\n", e);
    }

    lds_free(q);
    return 0;
}
