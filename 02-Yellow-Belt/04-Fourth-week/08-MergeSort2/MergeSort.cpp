#include <algorithm>

using namespace std;

#include <sstream>

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end);


template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_begin + 1 >= range_end)
        return;
    
    vector<typename RandomIt::value_type> copy_vec(range_begin, range_end);
    
    auto step = copy_vec.size() / 3;
    
    MergeSort(copy_vec.begin(), copy_vec.begin() + step);
    MergeSort(copy_vec.begin() + step, copy_vec.begin() + 2 * step);
    MergeSort(copy_vec.begin() + 2 * step, copy_vec.end());
    
    vector<typename RandomIt::value_type> tmp;
    
    merge(copy_vec.begin(), copy_vec.begin() + step,
          copy_vec.begin() + step, copy_vec.begin() + 2 * step,
          back_inserter(tmp));
    
    merge(tmp.begin(), tmp.end(),
          copy_vec.begin() + 2 * step, copy_vec.end(),
          range_begin);
}
