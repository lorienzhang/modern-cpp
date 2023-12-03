#include "iostream"
#include <cstdint>

using namespace std;

// void test_static_cast() {
//     // 1
//     float f = 10.f;
//     int i = static_cast<int>(f);

//     // 2
//     A a;
//     void *vpa = static_cast<void *>(&a);
//     int *pi = static_cast<int *>(vpa);
// }


const std::string &shorterString(const std::string &s1, const std::string &s2) {
    return s1.size() <= s2.size() ? s1 : s2;
}

std::string &shorterString(std::string &s1, std::string &s2) {
    auto &r = shorterString(const_cast<const std::string &>(s1), 
                                                 const_cast<const std::string &>(s2));
    return const_cast<std::string &>(r);
}

int main() {
    // test_static_cast();

    // const char *pc = "hello";
    // char *p = const_cast<char *>(pc);
    // p[1] = 'm'; // 编译通过，运行时发生段错误

    int value = 10;
    int *ptr = &value;
    intptr_t int_value = reinterpret_cast<intptr_t>(ptr);
    cout << "Pointer Value: " << ptr << endl;
    cout << "intptr_t value: " << int_value << endl;

    int *ptr2 = reinterpret_cast<int *>(int_value);
    return 0;
}