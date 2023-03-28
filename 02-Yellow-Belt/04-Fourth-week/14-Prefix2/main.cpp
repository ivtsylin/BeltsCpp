#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(
    RandomIt range_begin, RandomIt range_end,
    const string& prefix);

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith( RandomIt range_begin, RandomIt range_end, const string& prefix) {
    auto prefix_size = prefix.size();
    auto it_pair = equal_range(
                               range_begin,
                               range_end,
                               prefix,
                               [prefix_size](const typename RandomIt::value_type &a, const typename RandomIt::value_type &b) {
                                   return a.substr(0, prefix_size) < b.substr(0, prefix_size);
                               } );
    return it_pair;
}


int main() {
  const vector<string> sorted_strings = {"moscow", "motovilikha", "murmansk"};
  
  const auto mo_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");
  for (auto it = mo_result.first; it != mo_result.second; ++it) {
    cout << *it << " ";
  }
  cout << endl;
  
  const auto mt_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), "mt");
  cout << (mt_result.first - begin(sorted_strings)) << " " <<
      (mt_result.second - begin(sorted_strings)) << endl;
  
  const auto na_result =
      FindStartsWith(begin(sorted_strings), end(sorted_strings), "na");
  cout << (na_result.first - begin(sorted_strings)) << " " <<
      (na_result.second - begin(sorted_strings)) << endl;
  
  return 0;
}
