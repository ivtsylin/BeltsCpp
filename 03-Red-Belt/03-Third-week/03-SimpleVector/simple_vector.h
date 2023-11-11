#pragma once

#include <cstdlib>

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
    SimpleVector() : size(0), capacity(0), data(nullptr) {}
    explicit SimpleVector(size_t size_) : size(size_), capacity(size_), data(size_ ? new T [size_] : nullptr) {}
    ~SimpleVector() {
        delete [] data;
    }

    T& operator[](size_t index) {
        return data [ index ];
    }

    T* begin() {
        return data;
    }
    T* end() {
        return data + size;
    }

    size_t Size() const {
        return size;
    }
    size_t Capacity() const {
        return capacity;
    }
    void PushBack(const T& value) {
        if (!capacity) {
            capacity = 1;
            data = new T [1];
        } else if (size == capacity) {
            T* tmp = new T [2 * capacity];
            for (int i = 0; i < capacity; ++i)
                tmp [ i ] = data [ i ];
            delete [] data;
            data = tmp;
            capacity *= 2;
        }
        data [ size ] = value;
        ++size;
    }

private:
    T *data;
    size_t size;
    size_t capacity;
};
