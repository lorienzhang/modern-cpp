#include "iostream"
#include <vector>

using namespace std;


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
        // 当T被特化成test1时，subType是数据
        // 当T被特化成test2时，subType时int类型
        typename T::subType *ptr;
    }
};

template<typename T>
using myVector = std::vector<T>;

template <typename T>
struct myVector2 
{
    typedef std::vector<T> type;
};

template <>
struct myVector2<float> 
{
    enum class WinType
    {white, red, rose};

    WinType type;
};

int main() {


    MyClass<test2> myClass;
    myClass.func(); 

    myVector<int> myvec = {1, 2, 3};

    myVector2<int>::type myvec2 = {1, 2, 3};
    return 0;
}
