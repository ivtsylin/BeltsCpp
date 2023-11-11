#include <sstream>

#include "Common.h"

class ValueExpr : public Expression {
public:
    ValueExpr(int value_) : value(value_) {};
    // Вычисляет значение выражения
    int Evaluate() const override {
        return value;
    }

    // Форматирует выражение как строку
    // Каждый узел берётся в скобки, независимо от приоритета
    std::string ToString() const override {
        return std::to_string(value);
    }
private:
    int value;
};

class BinaryExpr : public Expression {
public:
    // Вычисляет значение выражения
    BinaryExpr(ExpressionPtr left_, ExpressionPtr right_, char op_) : left(move(left_)), right(move(right_)), op(op_) {};
    int Evaluate() const override {
        return op == '+' ? left->Evaluate() + right->Evaluate() : left->Evaluate() * right->Evaluate();
    }

    // Форматирует выражение как строку
    // Каждый узел берётся в скобки, независимо от приоритета
    std::string ToString() const override {
        return "(" + left->ToString() + ')' + op + '(' + right->ToString() + ')';
    }
private:
    ExpressionPtr left;
    ExpressionPtr right;
    char op;
};


// Функции для формирования выражения
ExpressionPtr Value(int value) {
    ExpressionPtr res = std::make_unique<ValueExpr>(value);
    return std::move(res);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<BinaryExpr>(std::move(left), std::move(right), '+');
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<BinaryExpr>(std::move(left), std::move(right), '*');
}
