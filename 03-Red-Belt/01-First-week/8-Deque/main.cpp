//#include "test_runner.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

// Реализуйте здесь шаблонный класс Table


template <class T>
class Deque {
public:
    Deque () : front_size(0), back_size(0) {};
    bool Empty() const {
        return front_size + back_size == 0;
    }
    size_t Size() const {
        return front_size + back_size;
    }
    T& operator[](size_t index) {
        if (index >= front_size) {
            return back [ index - front_size ];
        } else {
            return front [ front_size - 1 - index ];
        }
    }
    const T& operator[](size_t index) const {
        if (index >= front_size) {
            return back [ index - front_size ];
        } else {
            return front [ front_size - 1 - index ];
        }
    }
    T& At(size_t index) {
        if (index >= Size()) {
            throw out_of_range(to_string(index) + " greater then " + to_string(Size()));
        }
        if (index >= front_size) {
            return back [ index - front_size ];
        } else {
            return front [ front_size - 1 - index ];
        }
    }
    const T& At(size_t index) const {
        if (index >= Size()) {
            throw out_of_range(to_string(index) + " greater then " + to_string(Size()));
        }
        if (index >= front_size) {
            return back [ index - front_size ];
        } else {
            return front [ front_size - 1 - index ];
        }
    }
    T& Front() {
        if (front_size > 0) {
            return front [front_size - 1];
        }
        return back [0];
    }
    const T& Front() const {
        if (front_size > 0) {
            return front [front_size - 1];
        }
        return back [0];
    }
    T& Back() {
        if (back_size > 0) {
            return back [back_size - 1];
        }
        return front [ 0 ];
    }
    const T& Back() const {
        if (back_size > 0) {
            return back [back_size - 1];
        }
        return front [ 0 ];
    }
    void PushFront(const T& item) {
        front.push_back(item);
        ++front_size;
    }
    void PushBack(const T& item) {
        back.push_back(item);
        ++back_size;
    }
private:
    vector<T> front;
    vector<T> back;
    size_t front_size;
    size_t back_size;
};

/*
void TestTable() {
  Table<int> t(1, 1);
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  t[0][0] = 42;
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}
*/

int main() {
    Deque<int> deque;
    deque.PushBack(1);
    deque.PushBack(2);
    deque.PushFront(0);
    deque.PushFront(-1);
    
    for (int i = 0; i < deque.Size(); ++i) {
        cout << deque [ i ] << " ";
    }
    cout << endl;
    
    try {
        deque.At(5);
    } catch (...) {
        cout << "Error" << endl;
    }
    
    cout << deque.Front() << " " << deque.Back() << endl;
    
    //TestRunner tr;
    //RUN_TEST(tr, TestTable);
    return 0;
}
