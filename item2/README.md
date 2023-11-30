## 理解auto类型推导

```C++
int main() {
    auto x1 = 27;       // type: int
    auto x2(27);        // type: int
    auto x3{27};        // type: int
    auto x4 = {27};     // type: std::initializer_list<int>

    auto x6 = {1, 2, 3}; // type: std::initializer_list<int>

    return 0;
}
```
注意：x3推导出的是int，x4推导出的都是std::initializer_list<int>。

## {} 的模板类型推导
```C++
template<typename T>
void f(T param) {
}

template<typename T>
void g(std::initializer_list<T> param) {
}

int main() {
    // 报错：T无法直接推导出initializer_list<>
    f({1, 2, 3});

    // 正确：T推导出int
    g({1, 2, 3});
    
    return 0;
}
```