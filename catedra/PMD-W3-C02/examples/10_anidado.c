#include <stdio.h>
#include <omp.h>

int main(void) {
    //BEGINkernel
    omp_set_dynamic(0);
    omp_set_max_active_levels(2);
    #pragma omp parallel num_threads(2) default(none)
    {
        int exterior = omp_get_thread_num();
        #pragma omp parallel num_threads(2) default(none) firstprivate(exterior)
        {
            printf("exterior=%d interior=%d equipo=%d nivel_activo=%d\n",
                   exterior, omp_get_thread_num(),
                   omp_get_num_threads(), omp_get_active_level());
        }
    }
    //ENDkernel
    return 0;
}
