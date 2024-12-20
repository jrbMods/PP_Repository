#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// Function to multiply a portion of the matrix
void multiplyMatrixPart(int startRow, int endRow, int n, const std::vector<std::vector<int>>& A, 
                        const std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& C) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    int n = 1000; // Matrix size
    int numThreads = 4;

    std::vector<std::vector<int>> A(n, std::vector<int>(n, 1));
    std::vector<std::vector<int>> B(n, std::vector<int>(n, 2));
    std::vector<std::vector<int>> C(n, std::vector<int>(n, 0));

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    int rowsPerThread = n / numThreads;
    for (int t = 0; t < numThreads; ++t) {
        int startRow = t * rowsPerThread;
        int endRow = (t == numThreads - 1) ? n : startRow + rowsPerThread;
        threads.emplace_back(multiplyMatrixPart, startRow, endRow, n, std::ref(A), std::ref(B), std::ref(C));
    }

    for (auto& th : threads) {
        th.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Matrix Multiplication Time: " << elapsed.count() << " seconds\n";

    return 0;
}
