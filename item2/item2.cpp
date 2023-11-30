#include <iostream>

using namespace std;

template<typename T>
void f(T param) {
}

template<typename T>
void g(std::initializer_list<T> param) {
}

int main() {
    auto x1 = 27;       // type: int
    auto x2(27);        // type: int
    auto x3{27};        // type: int
    auto x4 = {27};     // type: std::initializer_list<int>

    auto x6 = {1, 2, 3}; // type: std::initializer_list<int>

    f({1, 2, 3});
    g({1, 2, 3});
    
    return 0;
}