//
//  main.cpp
//  YellowBelt
//
//  Created by Ivan Tsylin on 09.02.2023.
//

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>
#include <tuple>
#include <utility>
//#include "YellowBeltProblems.hpp"

using namespace std;

/*
 Ссылка на элемент
 Реализуйте шаблонную функцию GetRefStrict, которая на вход принимает: map и значение ключа k. Если элемент по ключу k в коллекции отсутствует, то функция должна бросить исключение runtime_error, иначе вернуть ссылку на элемент в коллекции.
 */


template <class K, class V>
V& GetRefStrict(map<K, V> &m, const K& key) {
    if (m.count(key) == 0) {
        throw runtime_error("");
    }
    return m.at(key);
};
/*
int main() {
    map<int, string> m = {{0, "value"}};
    string& item = GetRefStrict(m, 0);
    item = "newvalue";
    cout << m[0] << endl; // выведет newvalue
  return 0;
}
*/
