#pragma once

#include <stdexcept>

#include <array>
#include <iostream>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0) {
        if (a_size > N)
            throw invalid_argument("");
        size_ = a_size;
    };

    T& operator[](size_t index) {
        return data[index];
    }
    const T& operator[](size_t index) const {
        return data[index];
    }

    auto begin() {
        return data.begin();
    }
    auto end() {
        return data.begin() + size_;
    }
    auto begin() const {
        return data.begin();
    }
    auto end() const {
        return data.begin() + size_;
    }

    size_t Size() const {
        return size_;
    }
    size_t Capacity() const {
        return N;
    }

    void PushBack(const T& value) {
        if (size_ == N)
            throw overflow_error("");
        data[size_++] = value;
    }
    T PopBack() {
        if (!size_)
            throw underflow_error("");
        return data[--size_];
            
    }
private:
    array<T, N> data;
    size_t size_;
};

