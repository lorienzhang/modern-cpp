#include "iostream"
#include <cstddef>
#include <cstdint>

using namespace std;

struct A 
{
    int a;      // 4
    short b;    // 2 + 2 （补齐2个字节，保证下面的c的位置在4的整数位置上）
    int c;      // 4
    short d;    // 2 + 2 （补齐2个字节，结构体最后字段向内部最大对象对齐）
};

struct B
{
    int a;      // 4
    int c;      // 4
    short b;    // 2
    short d;    // 2
};

struct C 
{
    int a;
    short b; 
};

struct D : public C
{
    short c;
};

class Test {
public:
    virtual void func1() {
        cout << "func1" << endl;
    }

    virtual void func2() {
        cout << "func2" << endl;
    }
};

using Fun = void();

int main() {
    // struct A a;
    // // 16
    // cout << "sizeof(a): " << sizeof(a) << endl;
    // struct B b;
    // // 12
    // cout << "sizeof(b): " << sizeof(b) << endl;

    // struct C c;
    // // 16
    // cout << "sizeof(c): " << sizeof(c) << endl;
    // struct D d;
    // // 12
    // cout << "sizeof(d): " << sizeof(d) << endl;

    Test a;
    Fun *ptr = nullptr;

    auto mm = &a;
    cout << "mm: " << mm << endl;
    
    // t1是a的首地址
    auto t1 = (int64_t *)(&a);
    cout << "t1: " << t1 << endl;
    // t2是虚表的地址
    auto t2 = (int64_t *)(*t1);
    cout << "t2: " << t2 << endl;

    // 虚表中第1项，转成函数类型
    auto func1 = (Fun *)*t2;
    // 虚表中第2项，转成函数类型
    auto func2 = (Fun *)*(t2 + 1);

    func1();
    func2();
}