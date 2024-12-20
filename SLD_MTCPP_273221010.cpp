#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <random>
#include <chrono>

// Function to sort a segment of the vector
void sortSegment(std::vector<int>& data, int start, int end) {
    std::sort(data.begin() + start, data.begin() + end);
}

// Function to merge two sorted segments
void mergeSegments(std::vector<int>& data, int start, int mid, int end) {
    std::inplace_merge(data.begin() + start, data.begin() + mid, data.begin() + end);
}

int main() {
    int dataSize = 10000000; // 10 million elements
    int numThreads = 4;

    std::vector<int> data(dataSize);
    std::generate(data.begin(), data.end(), std::rand);

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    int segmentSize = dataSize / numThreads;

    // Sort segments in parallel
    for (int t = 0; t < numThreads; ++t) {
        int startIdx = t * segmentSize;
        int endIdx = (t == numThreads - 1) ? dataSize : startIdx + segmentSize;
        threads.emplace_back(sortSegment, std::ref(data), startIdx, endIdx);
    }

    for (auto& th : threads) {
        th.join();
    }

    // Merge sorted segments
    for (int step = segmentSize; step < dataSize; step *= 2) {
        for (int start = 0; start < dataSize; start += 2 * step) {
            int mid = start + step;
            int end = std::min(start + 2 * step, dataSize);
            if (mid < end) {
                mergeSegments(data, start, mid, end);
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Sorting Time: " << elapsed.count() << " seconds\n";

    return 0;
}
