#include "iostream"

using namespace std;

class A 
{
public:
    A() {
        cout << "A::A()" << endl;
        _data1 = new int();
        _data2 = new int();
    }

    virtual void vAfunc1() { cout << "A::vAfunc1()" << endl; }

    virtual void vAfunc2() { cout << "A::vAfunc2()" << endl; }

    void func1() { cout << "A::func1()" << endl; }

    void func2() { cout << "A::func2()" << endl; }

    virtual ~A() {
        cout << "A::~A()" << endl;
        if (_data1) {
            delete _data1; 
            _data1 = nullptr;
        }
        if (_data2) {
            delete _data2; 
            _data2 = nullptr;
        }
    }

private:
    int *_data1 {nullptr};
    int *_data2 {nullptr};
};

class B : public A 
{
public:
    B() {
        cout << "B::B()" << endl;
        _data3 = new int();
    }

    virtual void vAfunc1() override { cout << "B::vAfunc1()" << endl; }

    virtual void vBfunc1() { cout << "B::vBfunc1()" << endl; }

    virtual void vBfunc2() { cout << "B::vBfunc2()" << endl; }

    void func2() { cout << "B::func2()" << endl; }

    virtual ~B() {
        cout << "B::~B()" << endl;
        if (_data3) {
            delete _data3; 
            _data3 = nullptr;
        }
    }
private:
    int *_data3 {nullptr};
};

void test(A *a) {
    // 使用的虚表里面的type_info字段判断的
    if (typeid(*a) == typeid(A)) {
        cout << "a is A type: " << typeid(*a).name() << endl;
    }
    if (typeid(*a) == typeid(B)) {
        cout << "a is B type: " << typeid(*a).name() << endl;
    }
}

int main() {
    // A a;
    // B b;
    // a.vAfunc1();    // A::vAfunc1()
    // a.func1();      // A::func1()
    
    // b.vAfunc1();    // B::vAfunc1()
    // // B里面没有func1，但B的指针可以隐式转成A的指针，所以调用A的func1
    // b.func1();      // A::func1()
    // b.func2();      // B::func2()

    // cout  << "==================" << endl;
    // A aa = b;
    // // 虚表变了，变成A的虚表
    // aa.vAfunc1();   // A::vAfunc1()
    // aa.func1();     // A::func1()
    // aa.func2();     // A::func2()

    // cout  << "==================" << endl;

    // A *aptr = &b;
    // // 虽然aptr是A类型，但使用的还是b的虚表
    // aptr->vAfunc1();    // B::vAfunc1()
    // aptr->func1();      // A::func1()
    // aptr->func2();      // A::func2()

    // A *aptr =new B();
    // delete aptr;

    // cout << "======================" << endl;

    // A *base = new B();
    // test(base);

    // B b;
    // A *a = &b;
    // A *mm = dynamic_cast<A *>(&b);  // 成功
    // B *nn = dynamic_cast<B *>(mm);  // 成功

    // A a2;
    // B *b2 = dynamic_cast<B *>(&a2); // 失败，b2是nullptr

    B b;
    B *pb = &b;
    A *a = static_cast<A *>(pb);
    pb =  static_cast<B *>(a);
    a->vAfunc1();

    return 0;
}