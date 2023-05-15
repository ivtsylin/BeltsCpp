#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
#include <stack>
using namespace std;

template <class T>
class ObjectPool {
public:
    T* Allocate() {
        if (free.empty()) {
            T* NewPointer = new T;
            allocated.insert(NewPointer);
            return NewPointer;
        }
        T* ReusingPointer = free.front();
        free.pop();
        allocated.insert(ReusingPointer);
        return ReusingPointer;
    }
    T* TryAllocate() {
        if (free.empty()) {
            return nullptr;
        }
        T* ReusingPointer = free.front();
        free.pop();
        allocated.insert(ReusingPointer);
        return ReusingPointer;
    }

    void Deallocate(T* object) {
        if (!allocated.count(object)) {
            throw invalid_argument("Allocation impossible");
        }
        free.push(object);
        allocated.erase(object);
    }

    ~ObjectPool() {
        for (auto& p : allocated) {
            delete p;
        }
        while (!free.empty()) {
            delete free.front();
            free.pop();
        }
    }

private:
    // Добавьте сюда поля
    set<T*> allocated;
    queue<T*> free;
};

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}
