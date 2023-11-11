#include "test_runner.h"
#include "profile.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <optional>

using namespace std;

template<typename T>
void PrintCoeff(ostream& out, int i, const T& coef, bool printed) {
    bool coeffPrinted = false;
    if (coef == 1 && i > 0) {
        out << (printed ? "+" : "");
    } else if (coef == -1 && i > 0) {
        out << "-";
    } else if (coef >= 0 && printed) {
        out << "+" << coef;
        coeffPrinted = true;
    } else {
        out << coef;
        coeffPrinted = true;
    }
    if (i > 0) {
        out << (coeffPrinted ? "*" : "") << "x";
    }
    if (i > 1) {
        out << "^" << i;
    }
}

template<class T>
class lazy_T {
private:
    vector<T>& local_coef;
    size_t degree_;
    T zero = T();
public:
    lazy_T(vector<T>& coef) : local_coef(coef), degree_(0) {}
    void set_degree(size_t degree) {
        degree_ = degree;
    }
    T& operator =(const T& rhs) {
        size_t cur_size = local_coef.size();
        if (degree_ >= cur_size && rhs != 0) {
            local_coef.resize(degree_ + 1);
            local_coef[degree_] = rhs;
            for (size_t i = cur_size; i < degree_; ++i) {
                local_coef[i] = 0;
            }
        } else if (degree_ < cur_size) {
            local_coef[degree_] = rhs;
            while (local_coef.size() > 1 && local_coef.back() == 0) {
                local_coef.pop_back();
            }
        }
        return local_coef[degree_];
    }
    const T& operator*() const {
        if (degree_ >= local_coef.size()) return zero;
        return local_coef[degree_];
     }

     const T& operator=(const T& value) const {
         if (degree_ >= local_coef.size()) return zero;
         return local_coef[degree_];
     }

     bool operator==(const T& value) const {
       if (degree_ >= local_coef.size()) return value == T();
       return value == local_coef[degree_];
     }

     bool operator==(const lazy_T& other) const {
       const T& value = other;
       return operator==(value);
     }
};

template<class T>
ostream& operator<<(ostream& os, const lazy_T<T>& ca) {
    const T& value = *ca;
    os << value;
    return os;
}

template<typename T>
class Polynomial {
private:

    void Shrink() {
        while (coeffs_.size() > 1 && coeffs_.back() == 0) {
            coeffs_.pop_back();
        }
    }
    
    vector<T> coeffs_ = {0};
    lazy_T<T> lazy_assignment = lazy_T(coeffs_);

public:
    Polynomial() = default;
    Polynomial(vector<T> coeffs) : coeffs_(std::move(coeffs)) {
        Shrink();
    }

    template<typename Iterator>
    Polynomial(Iterator first, Iterator last) : coeffs_(first, last) {
        Shrink();
    }

    bool operator ==(const Polynomial& other) const {
        return coeffs_ == other.coeffs_;
    }

    bool operator !=(const Polynomial& other) const {
        return !operator==(other);
    }

    int Degree() const {
        return coeffs_.size() - 1;
    }

    Polynomial& operator +=(const Polynomial& r) {
        if (r.coeffs_.size() > coeffs_.size()) {
            coeffs_.resize(r.coeffs_.size());
        }
        for (size_t i = 0; i != r.coeffs_.size(); ++i) {
            coeffs_[i] += r.coeffs_[i];
        }
        Shrink();
        return *this;
    }

    Polynomial& operator -=(const Polynomial& r) {
        if (r.coeffs_.size() > coeffs_.size()) {
            coeffs_.resize(r.coeffs_.size());
        }
        for (size_t i = 0; i != r.coeffs_.size(); ++i) {
            coeffs_[i] -= r.coeffs_[i];
        }
        Shrink();
        return *this;
    }

    T operator [](size_t degree) const {
        return degree < coeffs_.size() ? coeffs_[degree] : 0;
    }

    lazy_T<T>& operator [](size_t degree) {
        lazy_assignment.set_degree(degree);
        return lazy_assignment;
    }

    // Реализуйте неконстантную версию operator[]

    T operator ()(const T& x) const {
        T res = 0;
        for (auto it = coeffs_.rbegin(); it != coeffs_.rend(); ++it) {
            res *= x;
            res += *it;
        }
        return res;
    }

    using const_iterator = typename std::vector<T>::const_iterator;

    const_iterator begin() const {
        return coeffs_.cbegin();
    }

    const_iterator end() const {
        return coeffs_.cend();
    }
};

template <typename T>
std::ostream& operator <<(std::ostream& out, const Polynomial<T>& p) {
    bool printed = false;
    for (int i = p.Degree(); i >= 0; --i) {
        if (p[i] != 0) {
            PrintCoeff(out, i, p[i], printed);
            printed = true;
        }
    }
    return out;
}

template <typename T>
Polynomial<T> operator +(Polynomial<T> lhs, const Polynomial<T>& rhs) {
    lhs += rhs;
    return lhs;
}

template <typename T>
Polynomial<T> operator -(Polynomial<T> lhs, const Polynomial<T>& rhs) {
    lhs -= rhs;
    return lhs;
}

void TestCreation() {
    {
        Polynomial<int> default_constructed;
        ASSERT_EQUAL(default_constructed.Degree(), 0);
        ASSERT_EQUAL(default_constructed[0], 0);
    }
    {
        Polynomial<double> from_vector({1.0, 2.0, 3.0, 4.0});
        ASSERT_EQUAL(from_vector.Degree(), 3);
        ASSERT_EQUAL(from_vector[0], 1.0);
        ASSERT_EQUAL(from_vector[1], 2.0);
        ASSERT_EQUAL(from_vector[2], 3.0);
        ASSERT_EQUAL(from_vector[3], 4.0);
    }
    {
        const vector<int> coeffs = {4, 9, 7, 8, 12};
        Polynomial<int> from_iterators(begin(coeffs), end(coeffs));
        ASSERT_EQUAL(from_iterators.Degree(), coeffs.size() - 1);
        ASSERT(std::equal(cbegin(from_iterators), cend(from_iterators), begin(coeffs)));
    }
}

void TestEqualComparability() {
    Polynomial<int> p1({9, 3, 2, 8});
    Polynomial<int> p2({9, 3, 2, 8});
    Polynomial<int> p3({1, 2, 4, 8});

    ASSERT_EQUAL(p1, p2);
    ASSERT(p1 != p3);
}

void TestAddition() {
    Polynomial<int> p1({9, 3, 2, 8});
    Polynomial<int> p2({1, 2, 4});

    p1 += p2;
    ASSERT_EQUAL(p1, Polynomial<int>({10, 5, 6, 8}));

    auto p3 = p1 + p2;
    ASSERT_EQUAL(p3, Polynomial<int>({11, 7, 10, 8}));

    p3 += Polynomial<int>({-11, 1, -10, -8});
    ASSERT_EQUAL(p3.Degree(), 1);
    const Polynomial<int> expected{{0, 8}};
    ASSERT_EQUAL(p3, expected);
}

void TestSubtraction() {
    Polynomial<int> p1({9, 3, 2, 8});
    Polynomial<int> p2({1, 2, 4});

    p1 -= p2;
    ASSERT_EQUAL(p1, Polynomial<int>({8, 1, -2, 8}));

    auto p3 = p1 - p2;
    ASSERT_EQUAL(p3, Polynomial<int>({7, -1, -6, 8}));

    p3 -= Polynomial<int>({7, -5, -6, 8});
    ASSERT_EQUAL(p3.Degree(), 1);
    const Polynomial<int> expected{{0, 4}};
    ASSERT_EQUAL(p3, expected);
}

void TestEvaluation() {
    const Polynomial<int> default_constructed;
    ASSERT_EQUAL(default_constructed(0), 0);
    ASSERT_EQUAL(default_constructed(1), 0);
    ASSERT_EQUAL(default_constructed(-1), 0);
    ASSERT_EQUAL(default_constructed(198632), 0);

    const Polynomial<int64_t> cubic({1, 1, 1, 1});
    ASSERT_EQUAL(cubic(0), 1);
    ASSERT_EQUAL(cubic(1), 4);
    ASSERT_EQUAL(cubic(2), 15);
    ASSERT_EQUAL(cubic(21), 9724);
}

void TestConstAccess() {
    const Polynomial<int> poly(std::initializer_list<int> {1, 2, 3, 4, 5});

    ASSERT_EQUAL(poly[0], 1);
    ASSERT_EQUAL(poly[1], 2);
    ASSERT_EQUAL(poly[2], 3);
    ASSERT_EQUAL(poly[3], 4);
    ASSERT_EQUAL(poly[4], 5);
    ASSERT_EQUAL(poly[5], 0);
    ASSERT_EQUAL(poly[6], 0);
    ASSERT_EQUAL(poly[608], 0);
}

void TestNonconstAccess() {
    Polynomial<int> poly;

    poly[0] = 1;
    poly[3] = 12;
    poly[5] = 4;
    ASSERT_EQUAL(poly.Degree(), 5);

    ASSERT_EQUAL(poly[0], 1);
    ASSERT_EQUAL(poly[1], 0);
    ASSERT_EQUAL(poly[2], 0);
    ASSERT_EQUAL(poly[3], 12);
    ASSERT_EQUAL(poly[4], 0);
    ASSERT_EQUAL(poly[5], 4);
    ASSERT_EQUAL(poly[6], 0);
    ASSERT_EQUAL(poly[608], 0);

    ASSERT_EQUAL(poly.Degree(), 5);

    poly[608] = 0;
    ASSERT_EQUAL(poly.Degree(), 5);

    {
        LOG_DURATION("Iteration over const");
        for (size_t i = 10; i < 50000; ++i) {
            ASSERT_EQUAL(std::as_const(poly)[i], 0);
            ASSERT_EQUAL(poly.Degree(), 5);
        }
    }
    {
        LOG_DURATION("Iteration over non-const");
        for (size_t i = 10; i < 50000; ++i) {
            ASSERT_EQUAL(poly[i], 0);
            ASSERT_EQUAL(poly.Degree(), 5);
        }
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestCreation);
    RUN_TEST(tr, TestEqualComparability);
    RUN_TEST(tr, TestAddition);
    RUN_TEST(tr, TestSubtraction);
    RUN_TEST(tr, TestEvaluation);
    RUN_TEST(tr, TestConstAccess);
    RUN_TEST(tr, TestNonconstAccess);
    return 0;
}
