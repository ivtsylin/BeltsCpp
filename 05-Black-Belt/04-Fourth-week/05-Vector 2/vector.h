#include <cstdint>
#include <iostream>
#include <utility>
#include <memory>

using namespace std;

template <typename T>
struct RawMemory {
    T* buf = nullptr;
    size_t cp = 0;
    
    static T* Allocate(size_t n) {
        return static_cast<T*>(operator new(n * sizeof(T)));
    }
    static void Deallocate(T* buf) {
        operator delete(buf);
    }
    
    RawMemory() = default;
    
    RawMemory(size_t n) {
        buf = Allocate(n);
        cp = n;
    }
    
    RawMemory(const RawMemory&) = delete;
    
    RawMemory(RawMemory&& other) noexcept {
        Swap(other);
    }
    
    ~RawMemory() {
        Deallocate(buf);
    }
    
    const T* operator+ (size_t i) const {
        return buf + i;
    }
    T* operator+ (size_t i) {
        return buf + i;
    }
    const T& operator[] (size_t i) const {
        return buf[i];
    }
    T& operator[] (size_t i) {
        return buf[i];
    }
    RawMemory& operator= (const RawMemory&) = delete;
    RawMemory& operator= (RawMemory&& other) {
        Swap(other);
        return *this;
    }
    
    void Swap(RawMemory& other) noexcept {
        std::swap(buf, other.buf);
        std::swap(cp, other.cp);
    }
};

template <typename T>
class Vector {
private:
    
    size_t sz = 0;
    RawMemory<T> data;
    
public:
    
    // Это простые и понятные операции
    size_t Size() const noexcept {
        return sz;
    }
    size_t Capacity() const noexcept {
        return data.cp;
    }
    const T& operator[](size_t i) const {
        return data[i];
    }
    T& operator[](size_t i) {
        return data[i];
    }
    
    // Требуется для начала реализовать эти функции:
    Vector() = default;
    
    explicit Vector(size_t n): data(n) {
        std::uninitialized_value_construct_n(data.buf, n);
        sz = n;
    }
    Vector(const Vector& other) : data(other.sz) {
        std::uninitialized_copy_n(other.data.buf, other.sz, data.buf);
        sz = other.sz;
    }
    ~Vector() {
        std::destroy_n(data.buf, sz);
    }
    // Строгая гарантия безопасности
    void Reserve(size_t n) {
        if (n > data.cp) {
            RawMemory<T> data2(n);
            std::uninitialized_move_n(data.buf, sz, data2.buf);
            std::destroy_n(data.buf, sz);
            data.Swap(data2);
        }
    }
    
    
    void Swap(Vector& other) noexcept {
        data.Swap(other.data);
        std::swap(sz, other.sz);
    }
    
    // Затем вот эти
    Vector(Vector&& other) noexcept {
        Swap(other);
    }
    // Базовая гарантия
    Vector& operator=(const Vector& other) {
        if (other.sz > data.cp) {
            Vector tmp(other);
            Swap(tmp);
        } else {
            std::copy(other.data.buf, other.data.buf + std::min(sz, other.sz), data.buf);
            if (sz < other.sz) {
                std::uninitialized_copy_n(
                    other.data.buf + sz,
                    other.sz - sz,
                    data.buf + sz
                );
            } else if (sz > other.sz) {
                std::destroy_n(data.buf + other.sz, sz - other.sz);
            }
            sz = other.sz;
        }
        return *this;
    }
    Vector& operator=(Vector&& other) noexcept {
        Swap(other);
        return *this;
    }
    // Строгая гарантия
    void Resize(size_t n) {
        Reserve(n);
        if (sz < n) {
            std::uninitialized_value_construct_n(data + sz, n - sz);
        } else if (sz > n) {
            std::destroy_n(data + n, sz - n);
        }
        sz = n;
    }
    void PushBack(const T& elem) {
        if (sz == data.cp) {
            Reserve(sz == 0 ? 1 : sz * 2);
        }
        new (data + sz) T(elem);
        ++sz;
    }
    // Строгая гарантия
    void PushBack(T&& elem) {
        if (sz == data.cp) {
            Reserve(sz == 0 ? 1 : sz * 2);
        }
        new (data + sz) T(std::move(elem));
        ++sz;
    }
    void PopBack() {
        std::destroy_at(data + sz - 1);
        --sz;
    }
    template <typename ... Args>
    T& EmplaceBack(Args&& ... args) {
        if (sz == data.cp) {
            Reserve(sz == 0 ? 1 : sz * 2);
        }
        auto elem = new (data + sz) T(std::forward<Args>(args) ...);
        ++sz;
        return *elem;
    }
    
    // В данной части задачи реализуйте дополнительно эти функции:
    using iterator = T*;
    using const_iterator = const T*;

    iterator begin() noexcept {
        return data.buf;
    }
    iterator end() noexcept {
        return data.buf + sz;
    }

    const_iterator begin() const noexcept {
        return data.buf;
    }
    const_iterator end() const noexcept {
        return data.buf + sz;
    }

    // Тут должна быть такая же реализация, как и для константных версий begin/end
    const_iterator cbegin() const noexcept {
        return data.buf;
    }
    const_iterator cend() const noexcept {
        return data.buf + sz;
    }
    
    template<typename ...Args>
    iterator Emplace(const_iterator pos, Args && ...args) {
        iterator result = nullptr;
        size_t shift = pos - begin();
        if (sz == Capacity()) {
            RawMemory<T> new_data(sz == 0 ? 1 : sz * 2);
            result = new (new_data + shift) T(std::forward<Args>(args)...);
            if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                std::uninitialized_move_n(begin(), shift, new_data.buf);
                std::uninitialized_move_n(begin() + shift, sz - shift, new_data.buf + shift + 1);
            }
            else {
                try {
                    std::uninitialized_copy_n(begin(), shift, new_data.GetAddress());
                    std::uninitialized_copy_n(begin() + shift, sz - shift, new_data.GetAddress() + shift + 1);
                }
                catch (...) {
                    std::destroy_n(new_data.GetAddress() + shift, 1);
                    throw;
                }
            }
            std::destroy_n(begin(), sz);
            data.Swap(new_data);
        }
        else {
            if (sz != 0) {
                new (data + sz) T(std::move(*(end() - 1)));
                try {
                    std::move_backward(begin() + shift, end(), end() + 1);
                }
                catch (...) {
                    std::destroy_n(end(), 1);
                    throw;
                }
                std::destroy_at(begin() + shift);
            }
            result = new (data + shift) T(std::forward<Args>(args)...);
        }
        ++sz;
        return result;
    }

    iterator Erase(const_iterator pos) {
        size_t shift = pos - begin();
        std::move(begin() + shift + 1, end(), begin() + shift);
        PopBack();
        return begin() + shift;
    }

    iterator Insert(const_iterator pos, const T& value) {
        return Emplace(pos, value);
    }

    iterator Insert(const_iterator pos, T&& value) {
        return Emplace(pos, std::move(value));
    }
};
