#include <iostream>

using namespace std;

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

    A(int a, std::string b) {
        cout << "std::string" << endl;
    }

    A(std::initializer_list<int> list) {
        cout << "A(initializer_list<int>)" << endl;
    }
};

struct People {
    std::string name;
    int age;
};

void func1(A a) {
    cout << "func1(A)" << endl;
}

int main() {
    // A a = 10;
    // A b(10);
    // A c {10};
    // A d = {10};

    // A e(10, 20);
    // A f{10, 20};
    // func1(A(10, 20));
    // func1({10, 20});

    // People aaa {"lisi", 18};


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