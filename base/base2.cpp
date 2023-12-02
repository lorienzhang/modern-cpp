#include <iostream>
using namespace std;

int main()
{
    // int x = 10;
    // int *p1 = &(x++);
    // int *p2 = &(++x);

    const int y = 10;
    const int &&a = std::move(y);
}