#include <vector>
#include <set>

using namespace std;

template <class T>
void RemoveDuplicates(vector<T> &elements) {
    set<T> set_elements;
    for (const auto& element : elements) {
        set_elements.insert(element);
    }
    elements.resize(set_elements.size());
    uint32_t i = 0;
    for (const auto& element : set_elements) {
        elements [ i ] = element;
        ++i;
    }
}
