#include "work.h"

int main(void) {
    #pragma omp parallel sections
    {
        #pragma omp section
        { Work1(); }

        #pragma omp section
        {
            Work2();
            Work3();
        }

        #pragma omp section
        { Work4(); }
    }
    return 0;
}
