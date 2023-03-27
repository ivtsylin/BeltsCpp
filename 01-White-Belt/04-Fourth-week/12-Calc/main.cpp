#include <iostream>

using namespace std;

class Rational {
public:
    Rational() {
        // Реализуйте конструктор по умолчанию
        p = 0;
        q = 1;
    }

    Rational(int numerator, int denominator) {
        if (!denominator) {
            throw invalid_argument("");
        }
        p = numerator;
        q = denominator;
        normalize();
        // Реализуйте конструктор
    }

    int Numerator() const {
        // Реализуйте этот метод
        return p;
    }

    int Denominator() const {
        // Реализуйте этот метод
        return q;
    }
    
    bool operator== (const Rational& rhs) const {
        return (p == rhs.p && q == rhs.q);
    };
    Rational operator+ (const Rational& rhs) const {
        int p_res, q_res;
        p_res = p * rhs.q + q * rhs.p;
        q_res = q * rhs.q;
        return Rational(p_res, q_res);
    };
    Rational operator- (const Rational& rhs) const {
        int p_res, q_res;
        p_res = p * rhs.q - q * rhs.p;
        q_res = q * rhs.q;
        return Rational(p_res, q_res);
    };
    Rational operator* (const Rational& rhs) const {
        int p_res, q_res;
        p_res = p * rhs.p;
        q_res = q * rhs.q;
        return Rational(p_res, q_res);
    };
    Rational operator/ (const Rational& rhs) const {
        if (!rhs.p) {
            throw domain_error("");
        }
        int p_res, q_res;
        p_res = p * rhs.q;
        q_res = q * rhs.p;
        return Rational(p_res, q_res);
    };
    bool operator< (const Rational& rhs) const {
        return p * rhs.q < q * rhs.p;
    }
    friend std::ostream& operator<<(std::ostream& stream, const Rational& rhs);
    friend std::istream& operator>>(std::istream& stream, Rational& rhs);

private:
    int p;
    int q;
    // Добавьте поля
    int NOD (int numerator, int denominator) const {
        if (!numerator) {
            return denominator;
        }
        int res;
        if (numerator < denominator) {
            res = numerator;
            numerator = denominator;
            denominator = res;
        }
        res = numerator % denominator;
        while (res) {
            res = numerator;
            numerator = denominator;
            denominator = res % denominator;
            res = numerator % denominator;
        }
        return denominator;
    }
    void normalize () {
        int nod = NOD(abs(p), abs(q));
        p /= nod;
        q /= nod;
        if (q < 0) {
            p *= -1;
            q *= -1;
        }
    }
};

std::ostream& operator<<(std::ostream& stream, const Rational& rhs) {
    stream << rhs.p << "/" << rhs.q;
    return stream;
}

std::istream& operator>>(std::istream& stream, Rational& rhs) {
    stream >> rhs.p;
    stream.ignore(1);
    stream >> rhs.q;
    if (!rhs.q) {
        throw invalid_argument("");
    }
    rhs.normalize();
    return stream;
}

int main() {
    Rational a, b;
    char operation;
    try {
        cin >> a >> operation >> b;
    } catch (invalid_argument&) {
        cout << "Invalid argument" << endl;
        return 0;
    }

    try {
        if (operation == '+') {
            cout << a + b << endl;
        } else if (operation == '-') {
            cout << a - b << endl;
        } else if (operation == '*') {
            cout << a * b << endl;
        } else { // (operation == '/')
            cout << a / b << endl;
        }
    } catch (domain_error&) {
        cout << "Division by zero" << endl;
        return 0;
    }

    return 0;
}
