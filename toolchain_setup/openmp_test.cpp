#include <iostream>
#include <omp.h>

int main()
{
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int thread_count = omp_get_num_threads();

        #pragma omp critical
        {
            std::cout << "Hello from thread "
                      << thread_id
                      << " of "
                      << thread_count
                      << std::endl;
        }
    }

    return 0;
}