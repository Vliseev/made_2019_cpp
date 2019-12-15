#include <iostream>
#include <sstream>
#include "Threadpoll.h"

int main() {
    ThreadPool pool(4);
    std::vector<std::future<int>> fut_vec;

    for (int i = 0; i < 16; ++i) {
        auto f = [i]() {
            std::ostringstream iss;
            iss << "hello " << i << "\n";
            std::cout << iss.str();

            iss.str("");
            iss.clear();

            std::this_thread::sleep_for(std::chrono::seconds(1));
            iss << "world " << i << std::endl;
            std::cout << iss.str();
            return i * i;
        };

        auto fut = pool.Exec(std::move(f));
        fut_vec.emplace_back(std::move(fut));
    }

    for (auto&& f : fut_vec) {
        std::cout << f.get() << "\n";
    }

    return 0;
}