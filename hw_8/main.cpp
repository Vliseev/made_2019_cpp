#include <iostream>
#include <vector>
#include "vector.h"

struct S {
    int a_;
    int b_;
};

void Test1() {
    std::vector<S> a(10);

    std::string aa = "11321";

    Vector<std::string> v;
    std::cout << "empty:" << v.Empty() << std::endl;
    v.PushBack(aa);
    std::cout << "empty:" << v.Empty() << std::endl;
    std::cout << v.Size() << std::endl;
    v.PushBack(std::move(aa));
    std::cout << v.Size() << std::endl;
    v.PopBack();
    std::cout << v.Size() << std::endl;
    v.PushBack("1313");
    v.Clear();
}

int main() {
    Vector<std::string> v = {"12313", "asdad", "eeeee", "ased"};
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << std::endl;
    }

    std::cout << std::endl;

    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        std::cout << *it << std::endl;
    }

    //    v.Clear();

    return 0;
}