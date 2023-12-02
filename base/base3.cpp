#include <iostream>
using namespace std;

void func(int a[]) {
    
}

void func(int (*ptr)[5]) {

}

bool func(int a, int b);

using funcPtr = bool (*)(int, int);

int main()
{
    // int array[5] = {1, 2, 3, 4, 5};

    // func(&array);

    char *str = "hello world";
    // 编译不报错，运行发生段错误
    str[0] = 'i';

    return 0;
}