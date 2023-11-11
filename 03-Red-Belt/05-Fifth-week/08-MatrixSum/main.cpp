#include "test_runner.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
#include <array>
using namespace std;

const int NUM_THREADS = 8;

template <class Iterator>
class Iterable_item {
public:
    Iterable_item(const Iterator& _first, const Iterator& _last) : first(_first), last(_last) {}
    const Iterator& begin() const {
        return first;
    }
    const Iterator& end() const {
        return last;
    }
    const size_t size() const {
        return last - first;
    }
private:
    Iterator first;
    Iterator last;
};

template <class Iterator>
class Paginator {
public:
    Paginator (const Iterator& _begin, const Iterator& _end, const size_t& _size) {
        if (_begin == _end) {
            return;
        }
        Iterator first = _begin;
        size_t amount_on_page = 0;
        for (Iterator iter = _begin; iter != _end; ++iter) {
            if (amount_on_page == 0) {
                if (iter != _begin) {
                    vec.push_back({first, iter});
                }
                first = iter;
            }
            ++amount_on_page;
            if (amount_on_page == _size) {
                amount_on_page = 0;
            }
        }
        vec.push_back({first, _end});
    }
    auto begin() const {
        return vec.begin();
    }
    auto end() const {
        return vec.end();
    }
    size_t size() const {
        return vec.size();
    }
private:
    vector<Iterable_item<Iterator>> vec;
};


template<class Iter>
int64_t CalculateMatrixSumSingleThreads(Iter begin, Iter end) {
    int64_t result = 0;
    for (auto cur = begin; cur != end; ++cur) {
        result += accumulate(cur->begin(), cur->end(), 0);
    }
    return result;
}


template <typename ContainerOfVectors>
int64_t SumSingleThread(const ContainerOfVectors& matrix)
{
    int64_t sum = 0;
    for (const auto& row : matrix) {
        for (const auto &item : row) {
            sum += item;
        }
    }
    return sum;
}


int64_t CalculateMatrixSum(const std::vector<std::vector<int>>& matrix)
{
    vector<future<int64_t>> futures;
    size_t row_num = (matrix.end() - matrix.begin() + NUM_THREADS) / NUM_THREADS;
    for (const auto page : Paginator(matrix.begin(), matrix.end(), row_num)) {
        futures.push_back(async([=] { return SumSingleThread(page); }));
    }

    int64_t result = 0;
    for (auto& f : futures) {
        result += f.get();
    }
    return result;
}

void TestCalculateMatrixSum() {
    {
        const vector<vector<int>> matrix = {
            {1}
        };
        ASSERT_EQUAL(CalculateMatrixSum(matrix), 1);
    }
    {
        const vector<vector<int>> matrix = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 16}
        };
        ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
    }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
}
