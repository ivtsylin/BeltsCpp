#include <algorithm>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end);


template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_begin + 1 >= range_end)
        return;
    
    vector<typename RandomIt::value_type> copy_vec(range_begin, range_end);
    
    MergeSort(copy_vec.begin(), copy_vec.begin() + copy_vec.size() / 2);
    MergeSort(copy_vec.begin() + copy_vec.size() / 2, copy_vec.end());
    
    merge(copy_vec.begin(), copy_vec.begin() + copy_vec.size() / 2,
          copy_vec.begin() + copy_vec.size() / 2, copy_vec.end(),
          range_begin);
}
