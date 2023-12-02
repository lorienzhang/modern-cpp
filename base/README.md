# 类型推导基础知识

## 基础1：顶层const与底层const
1. 顶层const：指针或引用本身是常量，即指针或引用本身不可变
2. 底层const：指针或引用指向的对象是常量，即对象本身不可变

```C++
// 顶层const
const int a = 10;

// const p是顶层const
// const int的底层const
const int *const p = new int(10);
```

> 《C++ Primer》 ：当执行对象拷贝操作时，顶层const可以忽略，底层const必须严格一致

参考下面代码，理解这句话：
```C++
int main()
{
    const int a = 10;
    // 顶层const，可以忽略
    int b = a;

    const int *const p = new int(10);
    // 编译报错，类型不一致
    int *p1 = p;
    // 编译报错，类型不一致
    int *const p2 = p;
    // 编译通过
    const int *p3 = p;
    
    // 编译报错
    int *p4 = &a;

    // 编译通过，这个const是底层const
    const int &r1 = a;
    // 编译报错，r2不是底层const
    int &r2 = a;
    // 编译报错，r3不是底层const
    int &r3 = r1;

    // 编译通过
    int c = r1;
}
```

问：为什么`int *p1 = p;`编译报错？<br>
答：本来p既是顶层const也是底层const，即p指向的对象不可改变，但是p1是int *，又可以改变其指向的内容，那么p的底层const就没有意义，所以必然是编译不过。

问：为什么`int *p4 = &a;`编译报错？<br>
答：变量a是顶层const，即a是一个常量，那么对a取址，必然是一个底层const，而p4不是底层const，所以编译报错。

引用赋值类似，看注释即可。有几个关键点：
1. 常量引用如果在=左边，=右边可以接收任何东西
2. 非常量引用=常量，编译报错
3. 非常量=常量引用，编译通过 (顶层const可以忽略)。参考：`int c = r1;`

## 基础2：表达式类型和右值引用
```c++
int main()
{
    int x = 10;
    // 报错，x++是右值
    int *p1 = &(x++);
    // 通过，++x是左值
    int *p1 = &(++x);
}
```
问：为什么`x++`是右值，`++x`是左值？<br>
答：`x++`首先返回x原始值，这个值是个临时值。`++x`返回增加后的值，这个值有名字的，所以是个左值   

**要点**：
1. 纯右值也可以std::move，即`std::move(10)`
2. 类没有实现移动构造，std::move会执行拷贝构造
3. 右值引用仍是左值
4. 将右值绑定到右值引用，即：`Test &&t = std::move(test)`什么都不会发生，只是一次简单的引用起别名。
5. 字符串字面量是个左值，即: "hello"是个左值。"hello"的类型：**const char[6]**，主要有个'\0'

```C++
int main()
{
    // 这个str应该是底层const，但不写const编译也不报错
    char *str = "hello world";
    // const char *str = "hello world";
    
    // 运行会发生段错误，不能更改"hello world"
    str[0] = 'i';

    return 0;
}
```

## 基础3：数组指针与函数指针
### 数组类型
```C++
int array[5] = {1, 2, 3, 4, 5};
```
问：array的类型是什么？<br>
答：int [5]

### 数组名退化为指针
```C++
int array[5] = {1, 2, 3, 4, 5};
int *ptr = array;
```
ptr和array肯定两个不同的类型，这里：数组名退化为指针 

### 数组名取地址
```C++
int array[5] = {1, 2, 3, 4, 5};
int *ptr = array;

int a = 1;
// 数组名取地址
int (*ptr2)[5] = &array;    // int (*)[5]
int *ptr3[5] = {&a, &a, &a, &a, &a};
```
问：解释ptr2和ptr3？<br>
答：[]的优先级要大于*；ptr2是指针，指向int [5]的数组类型；ptr3是数组，数组中每个元素是int * <br>

* 数组的首地址和数组类型取地址是两个不同类型，一个是int *，一个是int (\*)[5]

### 数组名作为参数传递会发生退化
```C++
void func(int a[]) {
}
```
这里参数a的类型会退化成int *<br>
也就是`void func(int a[100])`和`void func(int a[5])`和`void func(int *a)`等价

数组指针传参：

```C++
// ptr的类型：int (*)[5]
void func(int (*ptr)[5]) {
}

int main()
{
    int array[5] = {1, 2, 3, 4, 5};
    // 如果array的size不是5，则会报参数类型不一致
    func(&array);
}
```
即：`void func(int a[100])`和`void func(int (*a)[100])`和`void func(int (*a)[5])`不等价。

### 函数数据类型
问：`bool func(int a, int b);` 这个func的类型是什么？<br>
答：bool(int, int)<br>

问：`bool (*func_ptr)(int a, int b);` 这个func_ptr的类型是什么？<br>
答：func_ptr是一个指针，指向函数类型：`bool (*)(int, int)`<br>

* 函数指针赋值：`func_ptr=&func`，注意：取地址符&可以省略，函数名可以直接退化成函数指针。
* 函数指针使用：`bool result = (*func_ptr)(1, 2)`，这里*也是可以省略，即：`bool result = func_ptr(1, 2)`
* 函数指针作为形参：`void func2(int c, bool (*func_ptr)(int, int))`


**使用别名**：`using funcPtr = bool (*)(int, int);` 增加代码可读性: 
* 函数指针作为形参：`void func2(int c, funcPtr func)`
* 函数指针作为返回值：`funcPtr func3(int c)`
