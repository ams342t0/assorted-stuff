#include <iostream>
#include <cstdlib>
#include <chrono>
#include <immintrin.h>

using namespace std;			

// Device code
__global__ void VecAdd(float* A, float* B, float* C, int N)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (i < N)    C[i] = A[i] * B[i]*-1.0f;
}
            

			
int main()
{
    int N = 100000000;
	size_t size = N * sizeof(float);
	chrono::high_resolution_clock::time_point t1,t2;
	chrono::duration<double> t;
	__m256 a,b,c,n=_mm256_set1_ps(-1.0f);
    float* d_A;
    float* d_B;
    float* d_C;
	

    // Allocate input vectors h_A and h_B in host memory
    float* h_A = (float*)malloc(size);
    float* h_B = (float*)malloc(size);
    float* h_C = (float*)malloc(size);


    // Initialize input vectors with random stuff
    for(int i=0;i<N;++i)
	{
		h_A[i] = (float)rand();
		h_B[i] = (float)rand();
	}
	std::cout << "Initialized " << N << " float array" << std::endl;

    // Allocate vectors in device memory
    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    // Copy vectors from host memory to device memory
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);


	// Straight-up single-thread ALU operation on large float array
	t1 = chrono::high_resolution_clock::now();
	for(int i=0;i<N;i++)
	{
        h_C[i] = h_A[i] * h_B[i]*-1.0f;
	}
	t2 = chrono::high_resolution_clock::now();
	t = t2 - t1;
	std::cout << "CPU only :" << 1000.0f*t.count() << " ms" << std::endl;


	// try stuff using only CPU SIMD; Doing 8 32-bit floats at a time, single-thread
	t1 = chrono::high_resolution_clock::now();
	for(int i=0;i<N;i+=8)
	{
		a = _mm256_loadu_ps(h_A+i);
		b = _mm256_loadu_ps(h_B+i);
		c = _mm256_mul_ps(a,b);
		c = _mm256_mul_ps(c,n);
		_mm256_store_ps(h_C+i,c);
	}
	t2 = chrono::high_resolution_clock::now();
	t = t2 - t1;
	std::cout << "CPU SIMD: " << 1000.0f*t.count() << " ms" << std::endl;


	// now with GPU using CUDA. This one on a 3GB 1060GTX
    int threadsPerBlock = 1024;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;

	t1 = chrono::high_resolution_clock::now();

    // Invoke kernel
    VecAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, N);
	t2 = chrono::high_resolution_clock::now();
	t = t2 - t1;
	std::cout << "CUDA   :" << 1000.0f*t.count() << " ms" << std::endl;


    // Copy result from device memory to host memory
    // h_C contains the result in host memory
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

	/*
	CUDA appears only slightly faster than single-threaded SIMD operation. The transfer
	from GPU memory to system memory with cudaMemcpy is very slow, but the floating-point
	operation on the array itself is very fast.
	*/



    // Free device memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
            
    // Free host memory
    free(h_A);
	free(h_B);
	free(h_C);
}