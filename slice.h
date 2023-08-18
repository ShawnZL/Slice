//
// Created by Shawn Zhao on 2023/8/18.
//

#ifndef SLICE_SLICE_H
#define SLICE_SLICE_H
#include <cassert>
#include <cstddef>
#include <cstring>
#include <string>

class Slice {
public:
    // empty Slice
    Slice() : data_(""), size_(0) {}
    // Create a slice that refers to d[0,n-1]
    Slice(const char* d, size_t n) : data_(d), size_(n) {}

    // Create a slice that refers to the contents of "s"
    Slice(const std::string& s) : data_(s.data()), size_(s.size()) {}

    // Create a slice that refers to s[0,strlen(s)-1]
    Slice(const char* s) : data_(s), size_(strlen(s)) {}

    // Intentionally copyable.
    Slice(const Slice&) = default; // 拷贝构造函数
    Slice& operator=(const Slice&) = default; // 重载=

    const char* data() const { return data_; }

    size_t size() const { return size_; }

    bool empty() const { return size_ == 0; }

    char operator[](size_t n) const {
        assert(n < size());
        return data_[n];
    }

    void clear() {
        data_ = "";
        size_ = 0;
    }

    void remove_prefix(size_t n) {
        assert(n <= size());
        data_ += n; // 直接将头指针向后移动
        size_ -= n;
    }

    std::string ToString() const { return std::string(data_, size_); }

    // Three-way comparison.  Returns value:
    //   <  0 iff "*this" <  "b",
    //   == 0 iff "*this" == "b",
    //   >  0 iff "*this" >  "b"

    int compare(const Slice& b) const;

    // Return true iff "x" is a prefix of "*this"
    bool starts_with(const Slice& x) const {
        return ((size_ >= x.size_) && (memcmp(data_, x.data_, x.size_) == 0));
    }
private:
    const char* data_;
    size_t size_;
};

inline bool operator==(const Slice& x, const Slice& y) {
    return ((x.size() == y.size()) &&
            memcmp(x.data(), y.data(), x.size()) == 0);
}

inline bool operator!=(const Slice& x, const Slice& y) { return !(x == y); }

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

#endif //SLICE_SLICE_H
