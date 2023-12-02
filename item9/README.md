## typename关键字用来说明模板内部T标识符代表的是某种类型

当编译器遇到类型`T::xxx`这样的代码，它不清楚xxx是类型成员还是数据成员，知道T被特化后才知道。但为了处理模板代码，编译器必须要提前知道`T::xxx`的含义，所以如果是类型，我们要通过typename关键字告诉编译器。

```C++
struct test1 {
    static int subType;
};

struct test2 {
    typedef int subType;
};

template <typename T>
class MyClass {
public:
    void func () {
        // 当T被特化成Test1时，subType是数据
        // 当T被特化成Test2时，subType时int 类型
        typename T::subType *ptr;
    }
};

int main() {
    // 用test2特化T，编译通过
    MyClass<test2> myClass1;
    myClass1.func(); 

    // 用test1特化T， 编译报错
    MyClass<test1> myClass2;
    myClass2.func(); 

    return 0;
}
```

## 对于模板来说using比typedef更好用

```C++
template<typename T>
using myVector = std::vector<T>;

int main() {
    // 不会像typedef那样，用::带来有歧义，using没有歧义
    myVector<int> myvec = {1, 2, 3};

    return 0;
}

```

不会像typedef那样，用::带来有歧义，using没有歧义

```C++
template <typename T>
struct myVector2 
{
    typedef std::vector<T> type;
};

int main() {

    myVector2<int>::type myvec2 = {1, 2, 3};
    return 0;
}

```
typedef方式容易有歧义，比如：`myVector2<T>::type`，编译器不知道type时类型还是数据。比如：
```C++
template <typename T>
struct myVector2 
{
    typedef std::vector<T> type;
};

/** 特化一种类型 */
template <>
struct myVector2<float> 
{
    enum class WinType
    {white, red, rose};

    WinType type;
};
```
myVector2\<T\>::type就会有歧义