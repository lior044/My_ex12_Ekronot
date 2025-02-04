#include "Threads.h"

mutex mtx;

void I_Love_Threads() {
    cout << "I Love Threads" << endl;
}

void call_I_Love_Threads() {
    thread t1(I_Love_Threads);
    t1.join();
}

void printVector(const std::vector<int>& primes) {
    for (const auto& num : primes) {
        cout << to_string(num) << endl;
    }
}

void getPrimes(int begin, int end, std::vector<int>& primes) {
    for (int num = begin; num <= end; num++) {
        if (num == 2 || num == 3) {
            primes.push_back(num);
            continue;
        }
        if (num % 2 == 0 || num % 3 == 0 || num < 2) {
            continue;
        }
        bool isPrime = true;
        for (int i = 5; i * i <= num; i += 6) {
            if (num % i == 0 || num % (i + 2) == 0) {
                isPrime = false;
                break;
            }
        }

        if (isPrime) primes.push_back(num);
    }
}

std::vector<int> callGetPrimes(int begin, int end) {
    vector<int> primes;
    // Start the clock
    clock_t _start = std::clock();

    // Code to measure
    thread t1(getPrimes, begin, end, ref(primes));
    t1.join();

    // Stop the clock
    clock_t _end = clock();

    // Compute duration in seconds
    double duration = double(_end - _start) / CLOCKS_PER_SEC;

    cout << "Time taken: " << duration << " seconds" << endl;;

    return primes;
}


void writePrimesToFile(int begin, int end, std::ofstream& file) {
    std::vector<int> primes;
    getPrimes(begin, end, ref(primes));

    for (const auto& num : primes) {
        mtx.lock();
        file << num << " " << endl;;
        mtx.unlock();
    }

    //printVector(primes);

}

void callWritePrimesMultipleThreads(int begin, int end, std::string filePath, int N) {
    // Start the clock
    clock_t _start = std::clock();

    int amount_of_nums = end - begin;
    int amount_of_nums_in_each_thread = amount_of_nums / N;
    int new_begin;
    int new_end;

    vector<thread> threads;

    ofstream myfile(filePath);

    if (!myfile.is_open()) {
        cerr << "Failed to open file!" << endl;
        return;
    }

    mtx.lock();
    for (int i = 0; i < N; i++) {
        new_begin = begin + (amount_of_nums_in_each_thread * i);
        new_end = (i == N - 1) ? end : new_begin + amount_of_nums_in_each_thread;
        threads.push_back(thread(writePrimesToFile, new_begin, new_end, ref(myfile)));
    }
    mtx.unlock();

    // Join all threads to ensure they complete before moving on
    for (auto& t : threads) {
        t.join();
    }

    myfile.close();

    // Stop the clock
    clock_t _end = clock();

    // Compute duration in seconds
    double duration = double(_end - _start) / CLOCKS_PER_SEC;

    cout << "Time taken: " << duration << " seconds" << endl;
}