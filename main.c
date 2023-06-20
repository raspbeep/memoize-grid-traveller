#include <stdio.h>
#include <stdlib.h>

struct Memo_struct {
    int m;
    int n;
    int value;
};

typedef struct Memo_struct Memo;
typedef struct Memoize_struct Memoize;

struct Memoize_struct {
    Memo *memo;
    int size;
    void (*init)(Memoize *self);
    void (*set)(Memoize *self, int m, int n, int value);
    int (*get)(Memoize *self, int m, int n);
};

void memoize_init(Memoize *self) {
    self->memo = (Memo *) malloc(sizeof(Memo));
    self->size = 0;
}

void memoize_set(Memoize *self, int m, int n, int value) {
    self->memo[self->size].m = m;
    self->memo[self->size].n = n;
    self->memo[self->size].value = value;
    self->size++;
    Memo *ptr = (Memo *) realloc(self->memo, sizeof(Memo) * (self->size + 1));
    if (ptr != NULL) {
        self->memo = ptr;
    }
}

int memoize_get(Memoize *self, int m, int n) {
    for (int i=0; i < self->size; i++) {
        if (self->memo[i].m == m && self->memo[i].n == n) {
            return self->memo[i].value;
        }
    }
    return -1;
}

int grid_traveller(int m, int n, Memoize *memo) {
    if (m == 1 && n == 1) return 1;
    if (m == 0 || n == 0) return 0;
    int memoized_value = memo->get(memo, m, n);
    if (memoized_value != -1) return memoized_value;

    int computed_value = grid_traveller(m - 1, n, memo) + grid_traveller(m, n - 1, memo);
    if (memo->get(memo, m, n) == -1) {
        memo->set(memo, m, n, computed_value);
    }
    return computed_value;
}

int main() {
    Memoize memoize;
//    memoize.self = &memoize;
    memoize.init = &memoize_init;
    memoize.set = &memoize_set;
    memoize.get = &memoize_get;
    memoize.init(&memoize);

    printf("%d\n", grid_traveller(1, 1, &memoize));
    printf("%d\n", grid_traveller(3, 4, &memoize));
    printf("%d\n", grid_traveller(5, 6, &memoize));
    printf("%d\n", grid_traveller(18, 16, &memoize));
    return 0;
}