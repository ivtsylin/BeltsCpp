#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

class Figure {
public:
    virtual string Name() const = 0;
    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;
private:
};

class Triangle : public Figure {
public:
    Triangle (const double& a, const double& b, const double& c) : a_(a), b_(b), c_(c), p((a + b + c) / 2) {
    };
    string Name() const override {
        return "TRIANGLE";
    }
    double Perimeter() const override {
        return 2 * p;
    }
    double Area() const override {
        return sqrt(p * (p - a_) * (p - b_) * (p - c_));
    }
private:
    const double a_, b_, c_, p;
};

class Rect : public Figure {
public:
    Rect (const double& a, const double& b) : a_(a), b_(b) {
    };
    string Name() const override {
        return "RECT";
    }
    double Perimeter() const override {
        return 2 * (a_ + b_);
    }
    double Area() const override {
        return a_ * b_;
    }
private:
    const double a_, b_;
};

class Circle : public Figure {
public:
    Circle (const double& r) : r_(r) {
    };
    string Name() const override {
        return "CIRCLE";
    }
    double Perimeter() const override {
        return 2 * 3.14 * r_;
    }
    double Area() const override {
        return 3.14 * r_ * r_;
    }
private:
    const double r_;
};

shared_ptr<Figure> CreateFigure (istream& is) {
    string figure;
    is >> figure;
    if (figure == "TRIANGLE") {
        double a, b, c;
        is >> a >> b >> c;
        return make_shared<Triangle>(a, b, c);
    } else if (figure == "RECT") {
        double a, b;
        is >> a >> b;
        return make_shared<Rect>(a, b);
    }
    // figure == "CIRCLE"
    double r;
    is >> r;
    return make_shared<Circle>(r);
};

int main() {
  vector<shared_ptr<Figure>> figures;
  for (string line; getline(cin, line); ) {
    istringstream is(line);

    string command;
    is >> command;
    if (command == "ADD") {
      figures.push_back(CreateFigure(is));
    } else if (command == "PRINT") {
      for (const auto& current_figure : figures) {
        cout << fixed << setprecision(3)
             << current_figure->Name() << " "
             << current_figure->Perimeter() << " "
             << current_figure->Area() << endl;
      }
    }
  }
  return 0;
}
