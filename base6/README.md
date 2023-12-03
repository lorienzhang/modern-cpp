# 类型转换

## 一、 static_cast
### 1. 基本内置类型转换
```C++
float f = 10.f;
int i = static_cast<int>(f);
```
### 2. 任意类型的指针转换都可以用void *做媒介中转
```C++
A a;
void *vpa = static_cast<void *>(&a);
int *pi = static_cast<int *>(vpa);
```
### 3. 存在继承关系的类且没有多态之间类型转换
1. 上行(子 -> 父), 可以
2. 下行(父 -> 子), 不可以

从内存布局角度分析，这点很好理解

### 4. 存在继承关系的类，指针/引用之间的转换：随便转，上下行都不限制，但下行不安全
下行不安全，但编译器不限制。如果想要安全的转换，请使用dynamic_cast

## 二、 dynamic_cast
* 动态类型转换，通过虚表中type_info动态判断。转换失败返回nullptr；
* 一般用于将基类指针安全的转换到其派生类的指针类型

## 三、 const_cast
示例：
```C++
const char *pc = "hello";
// 去掉const
char *p = const_cast<char *>(pc);
// 增加const
const char *p1 = const_cast<const char *>p;
```

**注意**: 虽然p去掉了const，但我们不能通过p修改其指向的内容，否则会引发段错误
```C++
const char *pc = "hello";
char *p = const_cast<char *>(pc);

p[1] = 'm'; // 编译能通过，但运行时发生段错误
```
**const_cast应用**：函数重载，减少重复代码 (C++ Primer)
```C++
const std::string &shorterString(const std::string &s1, const std::string &s2) {
    return s1.size() <= s2.size() ? s1 : s2;
}

std::string &shorterString(std::string &s1, std::string &s2) {
    // 增加const，调用重载方法
    auto &r = shorterString(const_cast<const std::string &>(s1), 
                            const_cast<const std::string &>(s2));
    // 去掉const
    return const_cast<std::string &>(r);
}
```
## 四、 reinterpret_cast
* 为运算对象的位模式提供底层上的重新解释
* reinterpret_cast\<type\>(expression) 其中type和expression至少有一个为指针/引用
* 指针类型之间的转换可以直接使用reinterpret_cast，无需中间媒介void*

```C++
int value = 10;
int *ptr = &value;
// 把指针地址转成一个intptr_t的数
intptr_t int_value = reinterpret_cast<intptr_t>(ptr);
cout << "Pointer Value: " << ptr << endl;
cout << "intptr_t value: " << int_value << endl;
```
打印结果：
```
Pointer Value: 0x7ffe7045e944
intptr_t value: 140730782050628
```
虽然指针按十六进制输出，intptr_t按十进制输出。但这两个值是相等的<br>

我们还可以将一个整数转成任意类型的指针：
```C++
int *ptr = reinterpret_cast<int *>(int_value);
```
