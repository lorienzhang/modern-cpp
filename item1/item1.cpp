#include <iostream>

using namespace std;

// void func(int a) {}
// void func(const int a) {}

void func(int a, int b) {
}

using F = int(int);

template<typename T>
void f(T param) {
    cout << param << endl;
}

template<typename T>
void f2(T *param) {
    cout << param << endl;
}

template<typename T>
void f2_2(const T *param) {
    cout << param << endl;
}

template<typename T>
void f3(T &param) {
    cout << param << endl;
}

/** const 引用可以接受任何类型，这个范型推导范围就很广 */
template<typename T>
void f3_2(const T &param) {
    cout << param << endl;
}

template<typename T>
void f4(T &&param) {
    cout << param << endl;
}

/**
template<typename T>
void f(T param) {
    cout << param << endl;
}
*/
void test_f() {
    int a = 10;
    f(a);       // T = int
    int *aptr = &a;
    f(aptr);    // T = int*
    int &aref = a;
    f(aref);    // 左值引用，T = int
    int &&arref = std::move(a);
    f(arref);   // 右值引用，T = int

    f(10);      // T = int
    const int ca = 20;
    f(ca);      // T = int，顶层const被忽略
    const int *captr = &ca;
    f(captr);   // T = const int*，底层const严格匹配
    const int &caref = ca;
    f(caref);   // T = int，引用忽略，顶层const被忽略
    int *const acptr = &a;
    f(acptr);   // T = int*，顶层const被忽略
    
    /** 数组 */
    int array[2] = {1, 2};
    f(array);           // T = int*，数组退化为指针
    f("hello world");   // T = const char*，底层const
    const char aaa[12] = "hello world";
    f(aaa);             // T = const char*，底层const
    int (*arrayPtr)[2] = &array;
    f(arrayPtr);        // T = int (*)[2]，数组指针

    /** 函数 */
    func;
    f(func);           // T = void (*)(int, int)
    void (*funcptr)(int, int) = func;
    f(funcptr);        // T = void (*)(int, int)
}

/*
template<typename T>
void f2(T *param) {
    cout << param << endl;
}
*/
void test_f2() {
    int a = 10;
    f2(a);       // 报错，无法将值引用转换为指针
    int *aptr = &a;
    f2(aptr);    // T = int
    int &aref = a;
    f2(aref);   // 报错，无法将值引用转换为指针
    int &&arref = std::move(a);
    f2(arref);  // 报错，无法将值引用转换为指针

    const int ca = 20;
    f2(ca);         // 报错，无法将值引用转换为指针
    const int *captr = &ca;
    f2(captr);      // T = const int，底层const严格匹配
    const int &caref = ca;
    f2(caref);      // 报错，无法将值引用转换为指针
    int *const acptr = &a;
    f2(acptr);      // T = int，顶层const忽略
    const int *const acptr2 = &a;
    f2(acptr2);     // T = const int，顶层const忽略，底层const严格匹配
    f2(10);         // 报错，无法将值引用转换为指针
    
    /** 数组 */  
    int array[2] = {1, 2};
    f2(array);           // T = int
    f2("hello world");   // T = const char
    const char aaa[12] = "hello world";
    f2(aaa);            // T = const char
    int (*arrayPtr)[2] = &array;
    f2(arrayPtr);       // T = int[2]

    /** 函数 */
    func;
    f2(func);           // T = void(int, int)
    void (*funcptr)(int, int) = func;
    f2(funcptr);        // T = void(int, int)
}

/*
template<typename T>
void f2_2(const T *param) {
    cout << param << endl;
}
*/
void test_f2_2() {
    int a = 10;
    f2_2(a);       // 报错，无法将值引用转换为const T*
    int *aptr = &a;
    f2_2(aptr);    // T = int, param=const int *
    int &aref = a;
    f2_2(aref);   // 报错，无法将值引用转换为指针
    int &&arref = std::move(a);
    f2_2(arref);  // 报错，无法将值引用转换为指针

    const int ca = 20;
    f2_2(ca);         // 报错，无法将值引用转换为指针
    const int *captr = &ca;
    f2_2(captr);      // T = int，param=const int *
    const int &caref = ca;
    f2_2(caref);      // 报错，无法将值引用转换为指针
    int *const acptr = &a;
    f2_2(acptr);      // T = int，param=const int *，顶层const忽略
    const int *const acptr2 = &a;
    f2_2(acptr2);     // T = int，顶层const忽略
    f2(10);           // 报错，无法将值引用转换为指针
    
    /** 数组 */  
    int array[2] = {1, 2};
    f2_2(array);         // T = int
    f2_2("hello world");   // T = char, param=const char *
    const char aaa[12] = "hello world";
    f2_2(aaa);            // T = char, param=const char *
    int (*arrayPtr)[2] = &array;
    f2_2(arrayPtr);       // T = int[2]

    /** 函数 */
    void (*funcPtr)(int, int) = func;
    f2_2(func);           // 报错：函数指针没有底层const概念
    void (*funcptr)(int, int) = func;
    f2_2(funcptr);        // 报错：函数指针没有底层const概念
}

/*
template<typename T>
void f3(T &param) {
    cout << param << endl;
}
*/
void test_f3() {
    int a = 10;
    f3(a);       // T = int
    int *aptr = &a;
    f3(aptr);    // T = int*，param=int *&param
    int &aref = a;
    f3(aref);   // T = int
    int &&arref = std::move(a);
    f3(arref);  // T = int

    const int ca = 20;
    f3(ca);         // 注意：T = const int，引用，不属于对象拷贝，const不能忽略
    const int *captr = &ca;
    f3(captr);      // 注意：T = const int*，param=const int *&param
    const int &caref = ca;
    f3(caref);      // T = const int
    int *const acptr = &a;
    f3(acptr);      // T = int *const
    const int *const acptr2 = &a;
    f3(acptr2);     // T = const int* const，顶层和底层const都保留
    f3(10);         // 报错，无法将常量引用转换为引用
    
    /** 数组 */  
    int array[2] = {1, 2};
    f3(array);           // T = int[2]，引用不会退化，T还是int[2]
    f3("hello world");   // T = const char[12]
    const char aaa[12] = "hello world";
    f3(aaa);            // T = const char[12]
    int (*arrayPtr)[2] = &array;
    f3(arrayPtr);       // T = int (*)[2], param=int (* &param)[2]

    /** 函数 */
    func;
    f3(func);           // T = void(int, int)，引用不退化
    void (*funcptr)(int, int) = func;
    f3(funcptr);        // T = void (*)(int, int)
}

/*
template<typename T>
void f3_2(const T &param) {
    cout << param << endl;
}
*/
void test_f3_2() {
    int a = 10;
    f3_2(a);       // T = int

    f3_2(func);    // T = void(int, int)，函数引用底层const会被忽略
}

/*
template<typename T>
void f4(T &&param) {
    cout << param << endl;
}
*/
/** 引用折叠 */
void test_f4() {
    int a = 10;
    f4(a);          // T = int&, parma = int &param
    int *aptr = &a;
    f4(aptr);       // T = int*&, param=int *&param

    const int ca = 20;
    f4(ca);         // T = const int&, param=const int &param

    const int &caref = ca;
    f4(caref);      // T = const int&, param=const int &param
    const int &&carref = std::move(ca);
    f4(carref);     // T = const int&, param=const int &param，右值引用仍然是左值，折叠后还是左值引用

    f4(10);         // 注意：T = int, param=int &&param，这里应该没有发生折叠，缺啥补啥

    int array[2] = {0, 1};
    f4(array);      // T = int (&)[2], par·am=int(&param)[2]

    f4("hello world");   // T = const char (&)[12], param=const char(&param)[12]

    f4(func);        // T = void(int, int), param=void(&param)(int, int)
}

int main() {

    cout << "Hello World!" << endl;
    return 0;
}