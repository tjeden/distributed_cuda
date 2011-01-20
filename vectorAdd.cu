/*
 * Copyright 1993-2010 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

/* Vector addition: C = A + B.
 *
 * This sample is a very basic sample that implements element by element
 * vector addition. It is the same as the sample illustrating Chapter 3
 * of the programming guide with some additions like error checking.
 *
 */

// Includes
#include <stdio.h>
#include <cutil_inline.h>
#include <time.h>

#define REMOTE2
#define GPU
#define ITER 1000

// Variables
float* h_A;
float* h_B;
float* h_C;
float* d_A;
float* d_B;
float* d_C;
bool noprompt = false;

// Functions
void Cleanup(void);
void RandomInit(float*, int);
void DataInit(float*, int, const char*);
void ParseArguments(int, char**);

// Device code
__global__ void VecAdd(const float* A, const float* B, float* C, int N)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    int n = ITER;
    if (i < N){
      C[i]= 0;
      for (int j = 0; j< n; j++)
      {
        C[i]= (C[i]+A[i]) / B[i];
      }
    }
}

void Host(const float* A, const float* B, float* C, int N)
{
  int n = ITER;
  for (int i =0; i < N; i++)
  {
    if (i < N){
      C[i] = 0;
      for (int j = 0; j< n; j++)
      {
        C[i]= (C[i]+A[i]) / B[i];
      }
    }
  }
}

// Host code
extern "C" 
int calculate(char *data, char *result)
{
#ifdef REMOTE
    int N = 10000;
#else 
    int N = 10000000;
#endif
    size_t size = N * sizeof(float);

    // Allocate input vectors h_A and h_B in host memory
    h_A = (float*)malloc(size);
    if (h_A == 0) Cleanup();
    h_B = (float*)malloc(size);
    if (h_B == 0) Cleanup();
    h_C = (float*)malloc(size);
    if (h_C == 0) Cleanup();
    
    // Initialize input vectors
#ifdef REMOTE
    ////printf("dane: %s\n", data);
    char *pre;
    pre = strtok(data," ");
    int i =0;
    while (pre != NULL)
    {
      h_A[i]= atof(pre);
      h_B[i]= atof(pre);
      pre = strtok (NULL, " ");
      i ++;
    }
    N = i;
    size = N * sizeof(float);
#else
    DataInit(h_A, N, data);
    DataInit(h_B, N, data);
#endif

    // Allocate vectors in device memory
    cutilSafeCall( cudaMalloc((void**)&d_A, size) );
    cutilSafeCall( cudaMalloc((void**)&d_B, size) );
    cutilSafeCall( cudaMalloc((void**)&d_C, size) );

    clock_t start = clock();
#ifdef GPU
    // Copy vectors from host memory to device memory
    cutilSafeCall( cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice) );
    cutilSafeCall( cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice) );

    // Invoke kernel
    int threadsPerBlock = 256;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;

    /* Code you want timed here */
    VecAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, N);

    cutilCheckMsg("kernel launch failure");
#ifdef _DEBUG
    cutilSafeCall( cudaThreadSynchronize() );
#endif

    // Copy result from device memory to host memory
    // h_C contains the result in host memory
    cutilSafeCall( cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost) );
    
#else
    Host(h_A, h_B, h_C, N);
#endif
    printf("Time elapsed: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC);

    Cleanup();
    return 0;
}

void Cleanup(void)
{
    // Free device memory
    if (d_A)
        cudaFree(d_A);
    if (d_B)
        cudaFree(d_B);
    if (d_C)
        cudaFree(d_C);

    // Free host memory
    if (h_A)
        free(h_A);
    if (h_B)
        free(h_B);
    if (h_C)
        free(h_C);
        
    cutilSafeCall( cudaThreadExit() );
    
    /*if (!noprompt) {
        printf("\nPress ENTER to exit...\n");
        fflush( stdout);
        fflush( stderr);
        getchar();
    }

    exit(0);*/
}

// Allocates an array with random float entries.
void RandomInit(float* data, int n)
{
    for (int i = 0; i < n; ++i)
        data[i] = rand() / (float)RAND_MAX;
}

void DataInit(float* data, int n, const char* fill)
{
    for (int i = 0; i < n; ++i)
        data[i] = atof(fill);
}
// Parse program arguments
void ParseArguments(int argc, char** argv)
{
    for (int i = 0; i < argc; ++i)
        if (strcmp(argv[i], "--noprompt") == 0 ||
			strcmp(argv[i], "-noprompt") == 0) 
		{
            noprompt = true;
            break;
        }
}
