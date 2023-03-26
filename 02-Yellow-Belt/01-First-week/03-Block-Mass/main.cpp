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

using namespace std;

void task_1_3_2 () {
    int N, R;
    cin >> N >> R;
    uint64_t answer = 0;
    for (int i = 0; i < N; ++i) {
      uint64_t W, H, D;
      cin >> W >> H >> D;
      // Если не привести W к типу uint64_t перед умножением,
      // произведение будет иметь тип int и случится переполнение.
      // Альтернативное решение — хранить сами W, H, D в uint64_t
      answer += static_cast<uint64_t>(W) * H * D;
    }
    answer *= R;
    cout << answer << endl;
}


int main() {
    task_1_3_2();
}

