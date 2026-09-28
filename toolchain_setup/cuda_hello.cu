#include <cuda_runtime.h>
#include <cstdio>

__global__ void hello_from_gpu()
{
    printf("Hello from GPU! Block %d, Thread %d\n",
           blockIdx.x, threadIdx.x);
}

int main()
{
    hello_from_gpu<<<1, 4>>>();

    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        fprintf(stderr, "CUDA kernel launch failed: %s\n",
                cudaGetErrorString(err));
        return 1;
    }

    err = cudaDeviceSynchronize();
    if (err != cudaSuccess)
    {
        fprintf(stderr, "CUDA execution failed: %s\n",
                cudaGetErrorString(err));
        return 1;
    }

    return 0;
}