#include "iostream"
#include <algorithm>
#include <vector>

using std::cout;
using std::endl;

class Buffer
{
private:
    unsigned char *_buf;
    int _capacity;
    int _length;

public:
    explicit Buffer(int capacity) : _capacity(capacity), _length(0) {
        _buf = (capacity == 0) ? nullptr : new unsigned char[capacity]{};
        cout << "constructor: " << *this << endl;
    }

    ~Buffer() {
        std::cout << "~Buffer(): " << *this << std::endl;
        delete _buf;
    }

    friend void swap(Buffer &lhs, Buffer &rhs) {
        // 用标准库里面的swap兜底
        using std::swap;    // ADL: argument-dependent lookup

        // 优先找自己定义的，如果找不到，就用标准库的swap
        swap(lhs._buf, rhs._buf);
        swap(lhs._capacity, rhs._capacity);
        swap(lhs._length, rhs._length);
    }

    // 拷贝构造，属于构建一个新的对象
    Buffer(const Buffer &buffer) : _capacity(buffer._capacity), 
                                    _length(buffer._length), 
                                    _buf(buffer._capacity ?  new unsigned char[buffer._capacity]{} : nullptr)  {
        std::cout << "copy constructor" << *this << std::endl;

        std::copy(buffer._buf, buffer._buf + buffer._capacity, this->_buf);
    }

    // 移动构造
    // 这里先通过Buffer(0)构造一个空的buffer，然后将buffer的内容交换到this中
    Buffer(Buffer &&buffer) : Buffer(0) {
        std::cout << "move construnctor: " << *this << std::endl;;
        swap(buffer, *this);
    }

    // 拷贝赋值，对象已存在
    // 要实现swap，入参得是Buffer buffer
    //   1. 如果传入是左值，那么通过拷贝构造 构造buffer
    //   2. 如果传入是右值，那么通过移动构造 构造buffer，一般编译器优化，移动构造也不会调。
    Buffer &operator=(Buffer buffer) {
        std::cout << "copy assign: " << buffer << std::endl;;

        swap(buffer, *this);

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

    cout << "------------------------" << endl;

    auto buffers = std::vector<Buffer>();
    buffers.push_back(Buffer(10));
    buffers.push_back(Buffer(5));
    return 0;
}