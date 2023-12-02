## 一、std::move实现原理

### 初探
```C++
template <typename T>
T &&myMove(T &&param) {
    return static_cast<T &&>(param);
}

int main() {
    int m = 10;
    // 报错
    int &&mm = myMove(m);

    return 0;
}
```
当传入`myMove(m)` m是左值，T会被推导成`int &`，同时发生引用折叠，这时myMove会被特化成：
```C++
template <> int &myMove<int &>(int &param)
```
引用折叠后，myMove()返回左值类型，`int &&mm = myMove(m);` 就会报类型不匹配错误

### 怎么改进？(类型萃取)
```C++
template <typename T>
typename std::remove_reference<T>::type &&myMove(T &&param) {   // C++ 11   
    // 类型萃取：去掉引用。
    // T不管推导成什么，把引用去掉，避免下面static_cast发生引用折叠
    using returnType = typename std::remove_reference<T>::type;
    return static_cast<returnType &&>(param);
}

int main() {
    int m = 10;

    int &&mmm = myMove(m);

    return 0;
}
```
`int &&mmm = myMove(m);`特化后的函数:
```C++
template <>
typename std::remove_reference<int &>::type &&myMove<int &>(int &param)
```

## 二、std::forward实现原理
使用std::forward可以实现完美转发，看下面例子：
```C++
struct A {};

void process(A &&rValue) {
    cout << "deal rvalue" << endl;
}

void process(const A &rValue) {
    cout << "deal lvalue" << endl;
}

/* 完美转发param */
template<typename T>
void logAndProcess(T &&param) {
    process(std::forward<T>(param));
}

int main() {

    A a;
    logAndProcess(a);               // deal lvalue
    logAndProcess(std::move(a));    // deal rvalue

    return 0;
}
```
### forward 源码：
```C++
template<typename _Tp>
_Tp&& forward(typename std::remove_reference<_Tp>::type& __t) { 
    return static_cast<_Tp&&>(__t); 
}
```