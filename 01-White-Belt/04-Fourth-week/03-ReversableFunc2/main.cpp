#include <algorithm>
#include <vector>
#include <string>

using namespace std;

class FunctionPart {
private:
    char operation;
    double value;
public:
    FunctionPart(char new_operation, double new_value) {
        operation = new_operation;
        value = new_value;
    }
    double Apply(double source_value) const {
        if (operation == '+') {
            return source_value + value;
        } else if (operation == '-') {
            return source_value - value;
        } else if (operation == '*') {
            return source_value * value;
        }
        // operation == '/'
        return source_value / value;
    }
    void Invert() {
        if (operation == '+') {
            operation = '-';
        } else if (operation == '-') {
            operation = '+';
        } else if (operation == '*') {
            operation = '/';
        } else { // operation == '/'
            operation = '*';
        }
    }
};

class Function {
private:
    vector<FunctionPart> parts;
public:
    void AddPart(char operation, double value) {
        parts.push_back({operation, value});
    }
    double Apply(double value) const {
        for (const auto& part : parts) {
            value = part.Apply(value);
        }
        return value;
    }
    void Invert() {
        for (auto& part : parts) {
            part.Invert();
        }
        reverse(parts.begin(), parts.end());
    }
};
