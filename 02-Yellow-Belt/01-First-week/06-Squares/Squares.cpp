#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>
#include <tuple>
#include <utility>

using namespace std;

// Объявление функций
template <class T>
T Sqr (const T& arg);

template <class F, class S>
pair<F, S> Sqr (const pair<F, S> &arg);

template <class K, class V>
map<K, V> Sqr (const map<K, V> &arg);

template <class T>
vector<T> Sqr (const vector<T> &arg);


// Описание функций
template <class T>
T Sqr (const T& arg) {
    return arg * arg;
}

template <class F, class S>
pair<F, S> Sqr (const pair<F, S> &arg) {
    return make_pair(Sqr(arg.first), Sqr(arg.second));
}

template <class K, class V>
map<K, V> Sqr (const map<K, V> &arg) {
    map<K, V> result;
    for (const auto& item : arg) {
        result [item.first] = Sqr(item.second);
    }
    return result;
}

template <class T>
vector<T> Sqr (const vector<T> &arg) {
    vector<T> result;
    for (const auto& item : arg) {
        result.push_back(Sqr(item));
    }
    return result;
}

/*
int main() {
    vector<int> v = {1, 2, 3};
    cout << "vector:";
    for (int x : Sqr(v)) {
      cout << ' ' << x;
    }
    cout << endl;

    map<int, pair<int, int>> map_of_pairs = {
      {4, {2, 2}},
      {7, {4, 3}}
    };
    cout << "map of pairs:" << endl;
    for (const auto& x : Sqr(map_of_pairs)) {
      cout << x.first << ' ' << x.second.first << ' ' << x.second.second << endl;
    }
  return 0;
}
*/
