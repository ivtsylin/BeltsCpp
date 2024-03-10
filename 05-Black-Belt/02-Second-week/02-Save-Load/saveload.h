#include <map>
#include <iostream>
#include <string>
#include <vector>

// Serialization

template <typename T>
void Serialize(T pod, std::ostream& out);

void Serialize(const std::string& str, std::ostream& out);

template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out);

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out);


// Deserialization

template <typename T>
void Deserialize(std::istream& in, T& pod);

void Deserialize(std::istream& in, std::string& str);

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data);

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data);


// Serialization

template <typename T>
void Serialize(T pod, std::ostream& out) {
    out.write(reinterpret_cast<const char*>(&pod), sizeof(pod));
}

void Serialize(const std::string& str, std::ostream& out) {
    Serialize<size_t>(str.size(), out);
    out.write(str.c_str(), str.size());
}

template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out) {
    Serialize<size_t>(data.size(), out);
    for (const auto& item : data) {
        Serialize(item, out);
    }
}

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out) {
    Serialize<size_t>(data.size(), out);
    for (const auto& [key, value] : data) {
        Serialize(key, out);
        Serialize(value, out);
    }
}

// Deserialization

template <typename T>
void Deserialize(std::istream& in, T& pod) {
    in.read(reinterpret_cast<char*>(&pod), sizeof(pod));
}

void Deserialize(std::istream& in, std::string& str) {
    size_t size;
    Deserialize(in, size);
    str.resize(size);
    if (size > 0) {
        in.read(str.data(), size);
    }
}

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data) {
    size_t size;
    Deserialize<size_t>(in, size);
    for (size_t i = 0; i < size; ++i) {
        T item;
        Deserialize(in, item);
        data.emplace_back(item);
    }
}

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data) {
    size_t size;
    Deserialize<size_t>(in, size);
    for (size_t i = 0; i < size; ++i) {
        T1 key;
        T2 value;
        Deserialize(in, key);
        Deserialize(in, value);
        data[std::move(key)] = std::move(value);
    }
}
