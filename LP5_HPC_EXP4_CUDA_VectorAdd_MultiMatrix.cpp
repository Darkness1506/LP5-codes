#include <iostream>
#include <cuda.h>

using namespace std;

// ======================================================
// VECTOR ADDITION KERNEL
// ======================================================

__global__ void vectorAdd(int *A, int *B, int *C, int n) {

    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n) {
        C[i] = A[i] + B[i];
    }
}

// ======================================================
// MATRIX MULTIPLICATION KERNEL
// ======================================================

__global__ void matrixMul(int *A, int *B, int *C, int rowsA, int colsA, int colsB) {

    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < rowsA && col < colsB) {

        int sum = 0;

        for (int k = 0; k < colsA; k++) {
            sum += A[row * colsA + k] * B[k * colsB + col];
        }

        C[row * colsB + col] = sum;
    }
}

// ======================================================
// MAIN FUNCTION
// ======================================================

int main() {

    // ==================================================
    // VECTOR ADDITION
    // ==================================================

    int n;

    cout << "Enter size of vectors: ";
    cin >> n;

    int size = n * sizeof(int);

    int *h_A = new int[n];
    int *h_B = new int[n];
    int *h_C = new int[n];

    cout << "Enter elements of Vector A:\n";

    for (int i = 0; i < n; i++) {
        cin >> h_A[i];
    }

    cout << "Enter elements of Vector B:\n";

    for (int i = 0; i < n; i++) {
        cin >> h_B[i];
    }

    int *d_A, *d_B, *d_C;

    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    int threads = 256;
    int blocks = (n + threads - 1) / threads;

    vectorAdd<<<blocks, threads>>>(d_A, d_B, d_C, n);

    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    cout << "\nVector Addition Result:\n";

    for (int i = 0; i < n; i++) {
        cout << h_C[i] << " ";
    }

    cout << endl;

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    // ==================================================
    // MATRIX MULTIPLICATION
    // ==================================================

    int rowsA, colsA, rowsB, colsB;

    cout << "\nEnter rows and columns of Matrix A: ";
    cin >> rowsA >> colsA;

    cout << "Enter rows and columns of Matrix B: ";
    cin >> rowsB >> colsB;

    if (colsA != rowsB) {

        cout << "Matrix multiplication not possible!\n";
        return 0;
    }

    int sizeA = rowsA * colsA * sizeof(int);
    int sizeB = rowsB * colsB * sizeof(int);
    int sizeC = rowsA * colsB * sizeof(int);

    int *h_MA = new int[rowsA * colsA];
    int *h_MB = new int[rowsB * colsB];
    int *h_MC = new int[rowsA * colsB];

    cout << "\nEnter Matrix A elements:\n";

    for (int i = 0; i < rowsA * colsA; i++) {
        cin >> h_MA[i];
    }

    cout << "\nEnter Matrix B elements:\n";

    for (int i = 0; i < rowsB * colsB; i++) {
        cin >> h_MB[i];
    }

    int *d_MA, *d_MB, *d_MC;

    cudaMalloc((void**)&d_MA, sizeA);
    cudaMalloc((void**)&d_MB, sizeB);
    cudaMalloc((void**)&d_MC, sizeC);

    cudaMemcpy(d_MA, h_MA, sizeA, cudaMemcpyHostToDevice);
    cudaMemcpy(d_MB, h_MB, sizeB, cudaMemcpyHostToDevice);

    dim3 threadsPerBlock(16, 16);

    dim3 blocksPerGrid(
        (colsB + 15) / 16,
        (rowsA + 15) / 16
    );

    matrixMul<<<blocksPerGrid, threadsPerBlock>>>(
        d_MA,
        d_MB,
        d_MC,
        rowsA,
        colsA,
        colsB
    );

    cudaMemcpy(h_MC, d_MC, sizeC, cudaMemcpyDeviceToHost);

    cout << "\nMatrix Multiplication Result:\n";

    for (int i = 0; i < rowsA; i++) {

        for (int j = 0; j < colsB; j++) {

            cout << h_MC[i * colsB + j] << " ";
        }

        cout << endl;
    }

    cudaFree(d_MA);
    cudaFree(d_MB);
    cudaFree(d_MC);

    delete[] h_A;
    delete[] h_B;
    delete[] h_C;

    delete[] h_MA;
    delete[] h_MB;
    delete[] h_MC;

    return 0;
}