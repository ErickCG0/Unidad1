#include <thread>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

class Summation {
    int number_thread;
    int total;

    std::mt19937 gen;
    std::uniform_int_distribution<int> dis;

public:
    Summation(int number_thread) : number_thread(number_thread), total(0), gen(std::random_device{}()), dis(1, 1000) {}

    void randomSum() {
        for (int i = 0; i < 100; i++) {
            total += dis(gen);
        }
    }

    int Number_Thread() const {
        return number_thread;
    }

    int Total() const {
        return total;
    }
};

int main() {
    const int THREAD_COUNT = 10;
    std::vector<std::thread> threads;
    std::vector<Summation> summations;

    for (int i = 0; i < THREAD_COUNT; i++) {
        summations.emplace_back(i + 1);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        threads.emplace_back(&Summation::randomSum, &summations[i]);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    for (const auto& s : summations) {
        std::cout << "Thread # " << s.Number_Thread() << " : " << s.Total() << std::endl;
    }

    std::sort(summations.begin(), summations.end(), [](const Summation& a, const Summation& b) {
        return a.Total() > b.Total();
    });

    std::cout << "\nEl hilo con el Total más alto es el número # " << summations[0].Number_Thread() << " : " << summations[0].Total() << std::endl;

    return 0;
}