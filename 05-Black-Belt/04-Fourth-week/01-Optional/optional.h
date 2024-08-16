#include <utility>

// Исключение этого типа должно генерироваться при обращении к "пустому" Optional в функции Value
struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) unsigned char data[sizeof(T)];
    bool defined = false;

public:
    Optional() = default;
    Optional(const T& elem) {
        new(data) T(elem);
        defined = true;
    }
    Optional(T&& elem) {
        new(data) T(std::move(elem));
        defined = true;
    }
    Optional(const Optional& other) {
        if (other.defined) {
            new(data) T(*reinterpret_cast<const T*>(other.data));
        }
        defined = other.defined;
    }
    Optional(Optional&& other) {
        if (other.defined) {
            new(data) T(std::move(*reinterpret_cast<T*>(other.data)));
        }
        defined = other.defined;
    }

    Optional& operator=(const T& elem) {
        if(!defined) {
            new(data) T(elem);
        } else {
            *reinterpret_cast<T*>(data) = elem;
        }
        defined = true;
        return *this;
    }
    Optional& operator=(T&& elem) {
        if(!defined) {
            new(data) T(std::move(elem));
        } else {
            *reinterpret_cast<T*>(data) = std::move(elem);
        }
        defined = true;
        return *this;
    }
    Optional& operator=(const Optional& other) {
        if(defined) {
            if (other.defined) {
                *reinterpret_cast<T*>(data) = *reinterpret_cast<const T*>(other.data);
                defined = true;
            } else {
                reinterpret_cast<T*>(data)->~T();
                defined = false;
            }
        } else {
            if (other.defined) {
                new(data) T(*reinterpret_cast<const T*>(other.data));
                defined = true;
            }
            // else defined = false;
        }
        return *this;
    }
    Optional& operator=(Optional&& other) {
        if(defined) {
            if (other.defined) {
                *reinterpret_cast<T*>(data) = std::move(*reinterpret_cast<T*>(other.data));
                defined = true;
                //other.defined = false;
            } else {
                reinterpret_cast<T*>(data)->~T();
                defined = false;
            }
        } else {
            if (other.defined) {
                new(data) T(std::move(*reinterpret_cast<T*>(other.data)));
                defined = true;
                //other.defined = false;
            }
            // else defined = false;
        }
        return *this;
    }

    bool HasValue() const {
        return defined;
    }

    // Эти операторы не должны делать никаких проверок на пустоту.
    // Проверки остаются на совести программиста.
    T& operator*() {
        return *reinterpret_cast<T*>(data);
    }
    const T& operator*() const {
        return *reinterpret_cast<const T*>(data);
    }
    T* operator->() {
        return reinterpret_cast<T*>(data);
    }
    const T* operator->() const {
        return reinterpret_cast<const T*>(data);
    }

    // Генерирует исключение BadOptionalAccess, если объекта нет
    T& Value() {
        if (!defined)
            throw (BadOptionalAccess());
        return *reinterpret_cast<T*>(data);
    }
    const T& Value() const {
        if (!defined)
            throw (BadOptionalAccess());
        return *reinterpret_cast<const T*>(data);
    }

    void Reset() {
        if(defined) reinterpret_cast<T*>(data)->~T();
        defined = false;
    }

    ~Optional() {
        if(defined) reinterpret_cast<T*>(data)->~T();
    }
};
