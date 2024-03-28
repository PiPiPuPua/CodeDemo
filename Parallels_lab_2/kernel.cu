#include "cuda_runtime.h"
#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

#define iter 1000
__global__ void JacobiGPU(double* A, double* F, double* x_now, double* x_next, int N) {

    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < N) {
        double sigma = 0.0;
        int idx_Ai = idx * N;
        for (int i = 0; i < N; i++) {
            if (idx != i) sigma += A[idx_Ai + i] * x_now[i];

        }
        x_next[idx] = (F[idx] - sigma) / A[idx_Ai + idx];

    }
}

void Jacobi(int N, double** A, double* F, double* x_now, double* x_next)
{
    int i, j;
    double sigma;

    for (i = 0; i < N; i++)
    {
        sigma = 0.0;
        for (j = 0; j < N; j++)
        {
            if (i != j)
                sigma += A[i][j] * x_now[j];
        }
        x_next[i] = (F[i] - sigma) / A[i][i];
    }
}
int main(int argc, char** argv)
{
    char* filename = "input.txt";
    /*if (argv[1] != NULL) {
        filename = argv[1];
    }
    else {
        printf("Text file needed\n");
        return 0;
    }*/

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Couldn't open the input file");
    }

    int n;
    fscanf(file, "%d", &n);

    double** A = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++)
        A[i] = (double*)malloc(n * sizeof(double));
    double* F = (double*)malloc(n * sizeof(double));
    double* X = (double*)malloc(n * sizeof(double));
    double* T = (double*)malloc(n * sizeof(double));

    for (unsigned long int i = 0; i < n; ++i){
        for (unsigned long int j = 0; j < n; ++j){
            
            fscanf(file, "%lf", A[i] + j);
            if (i != j)
                A[i][j] /= 10000;
        }

        fscanf(file, "%lf\n", F + i);
        T[i] = 0;
        X[i] = 0;
    }

    fclose(file);

    double* newAr;
    newAr = (double*)malloc(n * n * sizeof(double));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            newAr[i * n + j] = A[i][j];
        }
    }

    double* X_GPU, * T_GPU, * A_GPU, * F_GPU;

    cudaMalloc((void**)&X_GPU, n * sizeof(double));
    cudaMalloc((void**)&T_GPU, n * sizeof(double));
    cudaMalloc((void**)&A_GPU, n * n * sizeof(double));
    cudaMalloc((void**)&F_GPU, n * sizeof(double));

    cudaMemcpy(T_GPU, T, sizeof(double) * n, cudaMemcpyHostToDevice);
    cudaMemcpy(A_GPU, newAr, sizeof(double) * n * n, cudaMemcpyHostToDevice);
    cudaMemcpy(X_GPU, X, sizeof(double) * n, cudaMemcpyHostToDevice);
    cudaMemcpy(F_GPU, F, sizeof(double) * n, cudaMemcpyHostToDevice);
    
    double timeStart = omp_get_wtime();
    for (int k = 0; k < iter; k++) {
        if (k % 2)
            Jacobi(n, A, F, X, T);
        else
            Jacobi(n, A, F, T, X);
    }

    //for (int i = 0; i < n; i++)
        //printf("%lf", X[i]);

    double workingTime = omp_get_wtime() - timeStart;
    printf("Algorithm done, time: %lf\n", workingTime);
    int tileSize = 1024;
    printf("Starting GPU algorithm with %d Tiles and %d Tile size\n", 1, tileSize);
    timeStart = omp_get_wtime();
    for (int k = 0; k < iter; k++) {
        if (k % 2)
            JacobiGPU << < 1, tileSize >> > (A_GPU, F_GPU, X_GPU, T_GPU, n);

        else
            JacobiGPU << < 1, tileSize >> > (A_GPU, F_GPU, T_GPU, X_GPU, n);
    }
    workingTime = omp_get_wtime() - timeStart;
    printf("Algorithm done, time: %lf\n", workingTime);
    if (n < 50) {
        printf("Printing results\n");
        FILE* CPUres, * GPUres;
        GPUres = fopen("GPUres.txt", "w+");
        CPUres = fopen("CPUres.txt", "w+");
        if (GPUres == NULL || CPUres == NULL) {
            printf("Can`t print results\n");
            return 0;
        }
        double* outGPU;
        outGPU = (double*)malloc(n * sizeof(double));
        cudaMemcpy(outGPU, T_GPU, sizeof(double) * n, cudaMemcpyDeviceToHost);
        for (int i = 0; i < n; i++) {
            fprintf(CPUres, "%lf\n", T[i]);
            fprintf(GPUres, "%lf\n", outGPU[i]);
        }
        fclose(GPUres); fclose(CPUres);
        for (int i = 0; i < n; i++) {
            if (T[i] != outGPU[i])
            {
                printf("Values are not equal\n");
                return 0;
            }

        }
        printf("Values are equal\n");

    }
    for (int i = 0; i < n; i++)
        free(A[i]);
    free(F); free(X); free(T); free(newAr); free(A);
    cudaFree(X_GPU); cudaFree(T_GPU); cudaFree(A_GPU); cudaFree(F_GPU);
    return 0;
}
