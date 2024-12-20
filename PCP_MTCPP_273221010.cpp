#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::queue<int> buffer;
std::mutex mtx;
std::condition_variable cv;
const int maxSize = 10;
bool done = false;

// Producer function
void producer(int numItems) {
    for (int i = 0; i < numItems; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return buffer.size() < maxSize; });

        buffer.push(i);
        std::cout << "Produced: " << i << std::endl;

        lock.unlock();
        cv.notify_all();
    }
    done = true;
    cv.notify_all();
}

// Consumer function
void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !buffer.empty() || done; });

        if (!buffer.empty()) {
            int item = buffer.front();
            buffer.pop();
            std::cout << "Consumed: " << item << std::endl;
        } else if (done) {
            break;
        }

        lock.unlock();
        cv.notify_all();
    }
}

int main() {
    int numItems = 20;

    std::thread producerThread(producer, numItems);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    return 0;
}
