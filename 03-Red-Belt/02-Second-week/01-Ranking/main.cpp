#include "test_runner.h"
#include "profile.h"
#include "student.h"

#include <numeric>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool Compare(const Student &first, const Student &second) {
  return first.Less(second);
}

/*
int main() {
    LOG_DURATION("Total");
    {
        LOG_DURATION("Empty");
        int a = 0;
    }
    return 0;
}
*/
