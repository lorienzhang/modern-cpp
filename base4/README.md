# C++类对象布局

## 影响C++对象大小的3个因素
* 非静态数据成员（因为静态数据成员存在类中）
* 虚函数（类中如果有虚函数，每个对象就会带一个虚表指针，大小8个字节）
* 字节对齐

## 字节对齐
字节对齐：
1. 类中变量占n个字节，那么这个变量就会放在n的整数倍的位置上
2. 结构体中最后一个字段会向内部最大字段对齐

看例子：
```C++
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

int main() {
    struct A a;
    // 16
    cout << "sizeof(a): " << sizeof(a) << endl;
    struct B b;
    // 12
    cout << "sizeof(b): " << sizeof(b) << endl;
}
```
再看下继承场景下的字节对齐：
```C++
struct C 
{
    int a;
    short b; 
};

struct D : public C
{
    short c;
};

int main() {
    struct C c;
    // 8
    cout << "sizeof(c): " << sizeof(c) << endl;
    struct D d;
    // 12
    cout << "sizeof(d): " << sizeof(d) << endl;
}
```
问：D继承了C，D里面有a，b，c三个成员，这样看在D里面b，c是不需要内存对齐的，为什么sizeof(d)不是8，而是12？
答：如果d的size是8，那么将d强转成c的时候就会有问题<br>

**要点**:在类中定义字段的时候，同种类型的字段尽量放在一起，可节省对象大小

## 初探虚表
```C++
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
    Test a;
    Fun *ptr = nullptr;

    // 对象a的首地址
    auto mm = &a;
    cout << "mm: " << mm << endl;
    
    // t1是a的首地址，也是a中虚表指针_vptr的地址
    auto t1 = (int64_t *)(&a);
    cout << "t1: " << t1 << endl;   // t1和mm是同一个地址
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
```
输出：
```
mm: 0x7ffc8577aec0
t1: 0x7ffc8577aec0
t2: 0x557b29f2dd60
func1
func2
```
