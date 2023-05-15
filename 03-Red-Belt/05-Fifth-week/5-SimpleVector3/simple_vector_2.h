#include <cstdint>
#include <iostream>

using namespace std;

// Реализуйте SimpleVector в этом файле
// и отправьте его на проверку

template <typename T>
class SimpleVector {
public:
    SimpleVector() : size(0), capacity(0), data(nullptr) {};
    explicit SimpleVector(size_t size_) : size(size_), capacity(size_), data(size_ ? new T [size_] : nullptr) {}
    ~SimpleVector() {
        delete[] data;
    }
    
    //SimpleVector(const SimpleVector& rhs) = delete;
    SimpleVector(const SimpleVector& rhs) {
        delete[] data;
        size = rhs.size;
        capacity = rhs.capacity;
        data = new T [capacity];
        copy(rhs.data, rhs.data + size, data);
    }
    SimpleVector(SimpleVector&& rhs) {
        delete[] data;
        
        size = rhs.size;
        capacity = rhs.capacity;
        data = rhs.data;
        
        rhs.data = nullptr;
        rhs.size = 0;
        rhs.capacity = 0;
    }
    
    //void operator=(const SimpleVector& rhs) = delete;
    void operator=(const SimpleVector& rhs) {
        delete[] data;
        size = rhs.size;
        capacity = rhs.capacity;
        data = new T [capacity];
        copy(rhs.data, rhs.data + size, data);
    }
    void operator=(SimpleVector&& rhs) {
        delete[] data;
        
        size = rhs.size;
        capacity = rhs.capacity;
        data = rhs.data;
        
        rhs.data = nullptr;
        rhs.size = 0;
        rhs.capacity = 0;
    }

    T& operator[](size_t index) {
        return data[index];
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
            move(data, data + size, tmp);
            delete[] data;
            data = tmp;
            capacity *= 2;
        }
        data[size] = value;
        ++size;
    }
    void PushBack(T&& value) {
        if (!capacity) {
            capacity = 1;
            data = new T [1];
        } else if (size == capacity) {
            T* tmp = new T [2 * capacity];
            move(data, data + size, tmp);
            delete [] data;
            data = tmp;
            capacity *= 2;
        }
        data[size] = move(value);
        ++size;
    }

private:
    // Добавьте сюда поля
    T *data;
    size_t size;
    size_t capacity;
};
