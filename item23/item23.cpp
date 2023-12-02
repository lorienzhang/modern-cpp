#include "iostream"
#include <type_traits>
#include <utility>

using namespace std;

template <typename T>
typename std::remove_reference<T>::type &&myMove(T &&param) {
    // 类型萃取：去掉引用。
    // T不管推导成什么，把引用去掉，避免下面static_cast发生引用折叠
    using returnType = typename std::remove_reference<T>::type;
    return static_cast<returnType &&>(param);
}

struct A {};

void process(A &&rValue) {
    cout << "deal rvalue" << endl;
}

void process(const A &rValue) {
    cout << "deal lvalue" << endl;
}

template<typename T>
void logAndProcess(T &&param) {
    process(std::forward<T>(param));
}

int main() {

    A a;
    logAndProcess(a);
    logAndProcess(std::move(a));

    return 0;
}