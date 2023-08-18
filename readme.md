# 源码阅读

📖来自levelDB中的slice，维护一个char数组，同时保存长度

```c++
private:
  const char* data_;
  size_t size_;
```

# 学习

## c_str data

`data()`和`c_str()`都是用来获取底层字符串的首地址的，但是在C++98中规定的是`data()`返回的字符串不保证有结尾`\0`，但是`c_str()`返回的字符串保证有结尾`\0

也就是C++98标准在设计字符串的时候是想抛弃C风格的字符串语法的，但是在实际应用中，往往需要将C++的`string`转换为C风格的字符串`const char*`。

这一点平常使用的时候没问题，但如果想直接通过C风格的接口修改`string`中的内容就麻烦了，因为`&str[0]`返回的字符串并不保证有结尾'\0'：

```c
std::string str = "hello";
// 在C++98中是未定义行为
auto p = &str[0]
```

好在C++11标准中意识到了这一点，规定`string`底层存储的字符串直接采用C风格的字符串语法，所以`data()`和`c_str()`就变成了同义词了：

```c
std::string str = "hello";
// 在C++11中是正确的，p指向一个空字符串
auto p = &str[0]
```

```c
std::string s = "asdad";
    auto p = &s[0];
    std::cout << s.data() << std::endl;
    std::cout << p << std::endl;
    std::cout << s.c_str() << std::endl;
```

结果一样

## const 函数

在函数声明和定义中，`const` 关键字的位置和用法有些微妙的变化。在你提供的函数声明中：

```cpp
const char* dacota() const;
```

这里的第一个 `const` 关键字表示函数的返回值是一个指向常量字符的指针，也就是说，函数返回的指针指向的字符不能被修改。

而第二个 `const` 关键字在函数声明的最后，表示这个成员函数是一个 `const` 成员函数。这意味着在这个成员函数中，你不能修改类的任何非 `mutable` 成员变量，并且在函数体内只能调用其他 `const` 成员函数。

所以，`const` 关键字的后面是用来修饰成员函数的，表示该函数在被调用时不会修改类的状态。

## memcmp

```c++
inline bool operator==(const Slice& x, const Slice& y) {
  return ((x.size() == y.size()) &&
          (memcmp(x.data(), y.data(), x.size()) == 0));
}
```

`memcmp` 是 C/C++ 标准库中的一个函数，用于比较两个内存区域的内容。函数原型如下：

```cpp
int memcmp(const void* ptr1, const void* ptr2, size_t num);
```

- `ptr1`：指向要比较的第一个内存块的指针。
- `ptr2`：指向要比较的第二个内存块的指针。
- `num`：要比较的字节数。

`memcmp` 函数会逐字节比较两个内存块的内容，直到遇到不同的字节或者比较的字节数达到指定的 `num`。如果两个内存块的内容相同，则返回 0。如果不同，返回一个小于或大于 0 的整数，表示第一个不同字节的差值。

这个函数通常用于比较字符串或者其他二进制数据，例如判断两个字符串是否相等。

## 写法

```c++
inline int Slice::compare(const Slice &b) const {
    const size_t min_len = (size_ < b.size_) ? size_ : b.size_;
//    const size_t min_len = min(size_, b.size_); 不写的原因是因为会引入algoritm，运行变慢
    int r = memcmp(data_, b.data_, min_len);
    if (r == 0 ) {
        if (size_ < b.size_) {
            r = -1;
        }
        else if (size_ > b.size_) {
            r = +1;
        }
    }
    return r;
}
```

`min_len` 变量的目的是为了找到两个字符串中较小的长度，以便在比较它们的内容时不会越界。

使用 `std::min(size_, b.size_)` 函数确实会引入 `<algorithm>` 头文件，这个头文件包含了很多标准算法的实现。尽管这并不会造成严重的性能问题，但对于一些特定场景的代码，为**了避免不必要的头文件引入，开发人员可能会选择直接使用三元运算符来计算较小的长度，就像你在代码中所示的那样。**

**这种做法主要是出于代码的清晰性和精简性考虑，**以及避免引入不必要的库。性能方面的影响可能非常微小，通常不会对大多数应用产生显著影响。