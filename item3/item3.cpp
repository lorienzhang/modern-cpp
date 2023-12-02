#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int func(int a, int b) {
    return 0;
}

void test_1() {
    int a = 10;
    decltype(a) bb1;         // type: int

    const int ca = 20;

    const int &caref = ca;
    decltype(caref) bb2;    // type: const int &
    const int &&carref = std::move(ca);
    decltype(carref) bb3;   // type: const int &&

    int *const acptr = &a;
    decltype(acptr) bb4;    // type: int *const
    const int *const cacptr = &a;
    decltype(cacptr) bb5;   // type: const int *const

    int array[2] = {0, 1};
    decltype(array) bb6;    // type: int[2]

    decltype(func) bb7;     // type: int bb7(int, int)
    decltype(&func) bb8;     // type: int (*)(int, int)
}

void test_2() {
    int a = 10;
    int *aptr = &a;
    decltype(aptr) b1;  // type：int *
    decltype(*aptr) b2; // *aptr是表达式，type：int &，引用类型
    decltype(a) b3;     // type: int
    decltype((a)) b4;   // 加个括号，a被当成表达式，type：int &

    const int ca = 20;
    decltype((ca)) b5;  // type: const int &

    const int *const cacptr = &a;
    decltype(cacptr) b6;    // type: const int *const
    decltype((cacptr)) b7;  // type: const int *const&

    decltype(10) b8;        // 10是表达式，右值。type：int
    decltype("hello") b9;   // "hello"是表达式，左值。type：const char (&)[6]

    decltype(a + 10) b10;   // a+10 是表达式，右值。type：int
}


char* func(int x) {
    return 0;
}

int &func2(int &x) {
    return x;
}

void test_3() {
    // 不会去执行func函数
    decltype(func(3)) b1;    // func(3)是表达式，函数返回右值，b1的type：char *

    int x = 10;
    decltype(func2(x)) b2;  // func2(x)是表达式，函数返回左值，b2的type：int &
}

void test_4() {
    std::vector<int> vec1 = {1, 2, 3};
    decltype(vec1[1]) aa = vec1[1]; // aa 的type：int &

    std::vector<bool> vec2 = {true, false, true};
    decltype(vec2[2]) bb = vec2[1]; // bb 的type：std::vector<bool>::reference
}

template <typename Container, typename Index>
auto getIndex(Container &c, Index i) ->decltype(c[i])
{
    return c[i];
}

int main() {

    return 0;
}