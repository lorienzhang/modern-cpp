## 使用 () 与 {} 创建对象
本小节实验需要关闭返回值优化: `set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-elide-constructors")`

```C++
struct A {
    A(int a) {
        cout << "A(int)" << endl;
    }

    A(const A& a) {
        cout << "A(const A&)" << endl;
    }

    A(int a, int b) {
        cout << "A(int, int)" << endl;
    }
};

int main() {
    /*
     * A(int) 
     * A(const A&)
     */
    A a = 10;
    return 0;
}
```
**结果**：一次构造，一次拷贝构造
```C++
int main() {
    /*
     * A(int) 
     */
    A b(10);
    return 0;
}
```
**结果**：一次构造
```C++
int main() {
    /*
     * A(int) 
     */
    A d = {10};
    /*
     * A(int) 
     */
    A e {10};
    return 0;
}
```
**结果**：一次构造<br>
A d = {10}; 相比于老的方式 A a = 10; 省了拷贝构造<br>

## A a{10};的优势
1. A a = 10; 会多一次拷贝。A a = {10}，没有拷贝
2. 作为函数入参或者函数返回值 {10} 方式也不需要拷贝。

看例子：
```C++
void func1(A a) {
    cout << "func1(A)" << endl;
}

int main() {
    /*
     * A(int, int)
     * A(const A&)
     */
    func1(A(10, 20));
    /*
     * A(int, int)
     */
    func1({10, 20});
}
```
* `func1(A(10, 20));` 调用 A(int, int) 构造，然后拷贝构造。
* `func1({10, 20}); ` 只调用 A(int, int) 构造，没有拷贝构造

## {}列表初始化大大简化了聚合类的初始化
```C++
#include <iostream>

using namespace std;

// People是聚合类
struct People {
    std::string name;
    int age;
};

int main() {
    People aaa {"lisi", 18};
    return 0;
}
```
聚合类前提条件：
1. 所有成员必须public
2. 不能定义任何构造函数
3. 没有基类，没有virtual函数

## 让类支持列表初始化 std::initializer_list<>
```C++
struct A {
    A(int a, int b) {
        cout << "A(int, int)" << endl;
    }

    A(std::initializer_list<int> list) {
        cout << "A(initializer_list<int>)" << endl;
    }

    A(int a, std::string b) {
        cout << "std::string" << endl;
    }
};

int main() {
    /**
     * 列表初始化优先级最高
     * 会覆盖构造函数：A(int a, int b)
     */
    A a = {1, 2};
    /**
     * 因为会优先匹配A(std::initializer_list<int> list)
     * 会报类型不匹配错误
     */
    A a1 = {1, 2.f};

    /**
     * 类型完全不匹配，会匹配到：A(int a, std::string b)
     */
    A a2 = {1, "hello"};
    return 0;
}
```
std::initializer_list<>要点：
1. 隐士类型收窄，int，float无法进行隐士类型转换，会报错
2. initializer_list优先级高，会覆盖构造函数
3. 空的{}不会调用initializer_list，({})，{{}}会调用initializer_list。不同的是：({}) size=0， {{}} size=1
