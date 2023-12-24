# copy & swap

## 示例：Buffer
Buffer类涉及内存的申请和释放，根据五法则我们需要编写拷贝构造，拷贝赋值，移动构造，移动赋值，析构函数。代码如下：
```c++
class Buffer
{
private:
    unsigned char *_buf;
    int _capacity;
    int _length;

public:
    explicit Buffer(int capacity) : _capacity(capacity), _length(0) {
        std::cout << "Buffer(int capacity)" << std::endl;
        _buf = (capacity == 0) ? nullptr : new unsigned char[capacity];
    }

    ~Buffer() {
        std::cout << "~Buffer()" << std::endl;
        delete _buf;
    }

    // 拷贝构造，属于构建一个新的对象
    Buffer(const Buffer &buffer) {
        std::cout << "Buffer(const Buffer &buffer)";

        this->_capacity = buffer._capacity;
        this->_length = buffer._length;
        this->_buf = new unsigned char[buffer._capacity];
        std::copy(buffer._buf, buffer._buf + buffer._capacity, this->_buf);

        std::cout << *this << std::endl;
    }

    // 移动构造
    Buffer(Buffer &&buffer) {
        std::cout << "Buffer(Buffer &&buffer)";

        this->_capacity = buffer._capacity;
        this->_length = buffer._length;
        this->_buf = buffer._buf;

        std::cout << *this << std::endl;

        // 清空右值资源
        buffer._buf = nullptr;
        buffer._capacity = 0;
        buffer._length = 0;
    }

    // 拷贝赋值运算符，对象已存在
    Buffer &operator=(const Buffer &buffer) {
        std::cout << "Buffer &operator=(const Buffer &buffer)";

        // 先判断：赋值运算符=的左右不一样
        if (this != &buffer) {
            // 先清空=左边的资源，否则内存泄漏
            delete[] this->_buf;
            // 重新开辟空间
            this->_buf = new unsigned char[buffer._capacity];
            this->_capacity = buffer._capacity;
            this->_length = buffer._length;
            std::copy(buffer._buf, buffer._buf + buffer._capacity, this->_buf);
        }

        std::cout << *this << std::endl;
        return *this;
    }

    // 移动赋值运算符
    Buffer &operator=(Buffer &&buffer) {
        std::cout << "Buffer &operator=(Buffer &&buffer)";
        // 先判断：赋值运算符=的左右不一样
        if (this != &buffer) {
            this->_capacity = buffer._capacity;
            this->_length = buffer._length;
            delete[] this->_buf;
            this->_buf = buffer._buf;

            buffer._buf = nullptr;
            buffer._capacity = 0;
            buffer._length = 0;
        }

        std::cout << *this << std::endl;
        return *this;
    }

    bool write(unsigned char value) {
        if (_length == _capacity) {
            return false;
        }
        _buf[_length++] = value;
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const Buffer& buffer) {
        os << "(" << buffer._length << "/" << buffer._capacity << ")[";
        for (int i = 0; i < buffer._capacity; ++i) {
            os << (int)buffer._buf[i];
            if (i < buffer._capacity - 1) {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }
};
```
main函数
```c++
int main() {
    Buffer buffer(10);
    std::cout << buffer << std::endl;
    buffer.write(1);
    buffer.write(2);
    std::cout << buffer << std::endl;
    // 拷贝构造
    Buffer buffer2 = buffer;
    
    Buffer buffer3(15);
    // 拷贝赋值运算符
    buffer3 = buffer;

    return 0;
}
```
Buffer类有三个数据成员：_buf，_capacity，_length。
* _buf: 存放数据的缓冲区
* _capacity: 缓冲区的容量
* _length: 缓冲区中已经使用的容量

Buffer类的代码有几个问题：
* 代码重复，拷贝构造，拷贝赋值运算符，移动构造，移动赋值运算符，四个函数代码相似度高
* 非异常免疫，拷贝赋值运算符和移动赋值运算符中如果发生异常，已经执行完的操作无法复原

## Copy & Swap
Copy & Swap是C++语言中解决对象拷贝构造和赋值运算符重载时，代码重复问题的一种解决方案。同时还是异常免疫的
