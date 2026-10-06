#include "work.h"

int main(void) {
    #pragma omp parallel
    {
        Work1();

        #pragma omp single
        {
            Work2();
            Work3();
        }

        Work4();
    }
    return 0;
}
