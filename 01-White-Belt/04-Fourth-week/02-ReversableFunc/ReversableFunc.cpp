#include <algorithm>
#include <vector>

using namespace std;

class FunctionPart {
public:
    FunctionPart(char new_operation, double new_value) {
        operation = new_operation;
        value = new_value;
    }
    double Apply(double source_value) const {
        if (operation == '+') {
            return source_value + value;
        }
        return source_value - value;
    }
    void Invert() {
        if (operation == '+') {
            operation = '-';
        } else {
            operation = '+';
        }
    }
private:
    char operation;
    double value;
};

class Function {
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
private:
    vector<FunctionPart> parts;
};
