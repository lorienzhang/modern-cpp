#include "iostream"
#include <algorithm>

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
    Buffer(const Buffer &buffer) : _capacity(buffer._capacity), 
                                    _length(buffer._length), 
                                    _buf(_capacity ?  new unsigned char[_capacity] : nullptr)  {
        std::cout << "Buffer(const Buffer &buffer)";
        std::cout << *this << std::endl;

        std::copy(buffer._buf, buffer._buf + buffer._capacity, this->_buf);
    }

    // 移动构造
    // 这里先通过Buffer(0)构造一个空的buffer，然后将buffer的内容交换到this中
    Buffer(Buffer &&buffer) : Buffer(0) {
        std::cout << "Buffer(Buffer &&buffer)";
        swap(buffer, *this);
    }

    // 拷贝赋值，对象已存在
    // 要实现swap，入参得是Buffer buffer
    //   1. 如果传入是左值，那么通过拷贝构造 构造buffer
    //   2. 如果传入是右值，那么通过移动构造 构造buffer，一般编译器优化，移动构造也不会调。
    Buffer &operator=(Buffer buffer) {
        std::cout << "Buffer &operator=(const Buffer &buffer)";

        swap(buffer, *this);

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