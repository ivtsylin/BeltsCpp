#include "node.h"

#include <stdexcept>

using namespace std;

/*
 node.h/cpp — эти файлы должны содержать объявления и определения класса Node, а также всех его потомков (см. выше), которые представляют собой узлы абстрактного синтаксического дерева, формируемого функцией ParseCondition;
 */

template<class T>
bool comp(Comparison cmp, const T&a, const T&b) {
    if (cmp == Comparison::Less) {
        return a < b;
    } else if (cmp == Comparison::LessOrEqual) {
        return a <= b;
    } else if (cmp == Comparison::Greater) {
        return a > b;
    } else if (cmp == Comparison::GreaterOrEqual) {
        return a >= b;
    } else if (cmp == Comparison::Equal) {
        return a == b;
    } else if (cmp == Comparison::NotEqual) {
        return a != b;
    }
    
    throw invalid_argument("Invalid comparison operator");
}

bool DateComparisonNode::Evaluate(const Date& date, const string& event) const {
    return comp(cmp_, date, date_);
}

bool EventComparisonNode::Evaluate(const Date& date, const string& event) const {
    return comp(cmp_, event, event_);
}

bool LogicalOperationNode::Evaluate(const Date& date, const string& event) const {
    if (op_ == LogicalOperation::Or) {
        return left_->Evaluate(date, event) || right_->Evaluate(date, event);
    } else if (op_ == LogicalOperation::And) {
        return left_->Evaluate(date, event) && right_->Evaluate(date, event);
    }
    return true;
}
