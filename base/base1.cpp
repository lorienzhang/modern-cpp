#include <iostream>
using namespace std;

int main()
{
    const int a = 10;
    int b = a;

    const int *const p = new int(10);
    int *p1 = p;
    int *const p2 = p;
    const int *p3 = p;

    int *p4 = &a;
    
    const int &r1 = a;
    int &r2 = a;
    int &r3 = r1;

    int c = r1;
}