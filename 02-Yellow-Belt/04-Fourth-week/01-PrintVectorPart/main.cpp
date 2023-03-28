#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void PrintVectorPart(const vector<int>& numbers) {
    auto res = find_if(
                        numbers.begin(),
                        numbers.end(),
                        [](int a) {return a < 0;});
    while (res != numbers.begin()) {
        --res;
        cout << *res << " ";
    }
};

int main() {
    PrintVectorPart({6, 1, 8, -5, 4});
    cout << endl;
    PrintVectorPart({-6, 1, 8, -5, 4});  // ничего не выведется
    cout << endl;
    PrintVectorPart({6, 1, 8, 5, 4});
    cout << endl;
    return 0;
}
