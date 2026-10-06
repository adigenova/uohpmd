#include "work.h"

int main(void) {
    #pragma omp parallel num_threads(1)
    {
        Work1();

        #pragma omp parallel num_threads(5)
        {
            Work2();
        }
    }
    return 0;
}
