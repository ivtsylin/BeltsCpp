#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end);


template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_begin + 1 >= range_end)
        return;
    
    vector<typename RandomIt::value_type> copy_vec(make_move_iterator(range_begin), make_move_iterator(range_end));
    
    auto step = copy_vec.size() / 3;
    
    MergeSort(copy_vec.begin(), copy_vec.begin() + step);
    MergeSort(copy_vec.begin() + step, copy_vec.begin() + 2 * step);
    MergeSort(copy_vec.begin() + 2 * step, copy_vec.end());
    
    vector<typename RandomIt::value_type> tmp;
    
    merge(make_move_iterator(copy_vec.begin()), make_move_iterator(copy_vec.begin() + step),
          make_move_iterator(copy_vec.begin() + step), make_move_iterator(copy_vec.begin() + 2 * step),
          back_inserter(tmp));
    
    merge(make_move_iterator(tmp.begin()), make_move_iterator(tmp.end()),
          make_move_iterator(copy_vec.begin() + 2 * step), make_move_iterator(copy_vec.end()),
          range_begin);
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
