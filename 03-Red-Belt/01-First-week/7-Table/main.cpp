#include "test_runner.h"
#include <vector>

using namespace std;

// Реализуйте здесь шаблонный класс Table

template <class T>
class Table {
public:
    Table (size_t _nrow, size_t _ncol) {
        Resize (_nrow, _ncol);
    }
    vector<T> &operator[] (int n) {
        return table[n];
    }
    const vector<T> &operator[] (int n) const{
        return table[n];
    }
    void Resize (size_t _nrow, size_t _ncol) {
        table.resize(_nrow);
        for (auto& row : table) {
            row.resize(_ncol);
        }
        nrow = _nrow;
        ncol = _ncol;
    }
    pair<size_t, size_t> Size() const {
        return make_pair(nrow, ncol);
    }
private:
    vector<vector<T>> table;
    size_t nrow;
    size_t ncol;
};

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

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestTable);
  return 0;
}
