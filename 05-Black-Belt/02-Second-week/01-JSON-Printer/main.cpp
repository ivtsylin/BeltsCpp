#include "test_runner.h"

#include <cassert>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <stack>
#include <string>

class Empty {};

template <typename Parent>
class Array;

template <typename Parent>
class Object;

template<class Parent>
class Value {
private:
    std::ostream& out_;
    Parent* parent_;
    bool printed_;
public:
    Value(std::ostream& out) : out_(out), parent_(nullptr), printed_(false) {};
    Value(std::ostream& out, Parent* parent) : out_(out), parent_(parent), printed_(false) {};
    
    Parent &Number(int64_t value) {
        out_ << value;
        printed_ = true;
        return *parent_;
    }
    Parent &Boolean(bool value) {
        out_ << std::boolalpha << value;
        printed_ = true;
        return *parent_;
    }
    Parent &Null() {
        out_ << "null";
        printed_ = true;
        return *parent_;
    }
    Parent &String(std::string_view value){
        out_ << '\"';
        for (auto ch : value) {
          if (ch == '\"' || ch == '\\')
            out_ << '\\';
          out_ << ch;
        }
        out_ << '\"';

        printed_ = true;
        return *parent_;
    }
    
    Array<Parent> BeginArray() {
        printed_ = true;
        return {out_, this->parent_};
    }
    Object<Parent> BeginObject() {
        printed_ = true;
        return {out_, this->parent_};
    }
    
    ~Value() {
        if (!printed_) Null();
    }
};

template <typename Parent, char BeginChar, char EndChar, char DelimeterChar>
class Context {
public:
    Context(std::ostream& out) : out_(out), parent_(nullptr), first_added_(false), opened_(false) {
        Begin();
    }
    Context(std::ostream& out, Parent *parent) : out_(out), parent_(parent), first_added_(false), opened_(false) {
        Begin();
    }
    ~Context() {
        if(opened_)
            End();
    }

protected:
    std::ostream& out_;
    Parent *parent_;
    bool first_added_;
    bool opened_;

    Value<Context> AddValue() {
        Delimeter();
        return {out_, this};
    }

    void Begin() {
        opened_ = true;
        out_ << BeginChar;
    }
    void Delimeter() {
        if (!first_added_) {
            first_added_ = true;
        } else {
            out_ << ',';
        }
    }

    void End() {
        opened_ = false;
        out_ << EndChar;
    }
};

template<typename Parent>
class Array : public Context<Parent, '[', ']', ','> {
public:
    Array(std::ostream& out) : Context<Parent, '[', ']', ','>(out) {}
    Array(std::ostream& out, Parent* parent) : Context<Parent, '[', ']', ','>(out, parent) {}

    Array<Parent>& Number(int64_t value) {
        this->AddValue().Number(value);
        return *this;
    }
    Array<Parent>& String(std::string_view value) {
        this->AddValue().String(value);
        return *this;
    }
    Array<Parent>& Boolean(bool value) {
        this->AddValue().Boolean(value);
        return *this;
    }
    Array<Parent>& Null() {
        this->AddValue().Null();
        return *this;
    }

    Array<Array<Parent>> BeginArray() {
        this->Delimeter();
        return {this->out_, this};
    }
    Object<Array<Parent>> BeginObject() {
        this->Delimeter();
        return {this->out_, this};
    }
    Parent& EndArray() {
        this->End();
        return *this->parent_;
    }
};

template<typename Parent>
class Object : public Context<Parent, '{', '}', ','> {
public:
    Object(std::ostream& out) : Context<Parent, '{', '}', ','>(out) {}
    Object(std::ostream& out, Parent* parent) : Context<Parent, '{', '}', ','>(out, parent) {}

    Value<Object<Parent>> Key(std::string_view value) {
        this->Delimeter();
        Value<Empty>{this->out_}.String(value);
        this->out_ << ':';
        return {this->out_, this};
    }

    Parent& EndObject() {
        this->End();
        return *this->parent_;
    }
};

void PrintJsonString(std::ostream& out, std::string_view str) {
    Value<Empty>(out).String(str);
}

using ArrayContext = Array<Empty>;  // Замените это объявление на определение типа ArrayContext
ArrayContext PrintJsonArray(std::ostream& out) {
    // реализуйте функцию
    return {out};
}

using ObjectContext = Object<Empty>;   // Замените это объявление на определение типа ObjectContext
ObjectContext PrintJsonObject(std::ostream& out) {
    // реализуйте функцию
    return {out};
}

void TestObject() {
    std::ostringstream output;

    {
        auto json = PrintJsonObject(output);
        json
            .Key("id1").Number(1234)
            .Key("id2").Boolean(false)
            .Key("").Null()
            .Key("\"").String("\\");
    }

    ASSERT_EQUAL(output.str(), R"({"id1":1234,"id2":false,"":null,"\"":"\\"})");
}

void TestArray() {
    std::ostringstream output;

    {
        auto json = PrintJsonArray(output);
        json
            .Number(5)
            .Number(6)
            .BeginArray()
                .Number(7)
            .EndArray()
            .Number(8)
            .String("bingo!");
    }

    ASSERT_EQUAL(output.str(), R"([5,6,[7],8,"bingo!"])");
}

void TestAutoClose() {
    std::ostringstream output;

    {
        auto json = PrintJsonArray(output);
        json.BeginArray().BeginObject();
    }

    ASSERT_EQUAL(output.str(), R"([[{}]])");
}


int main() {
    TestRunner tr;
    RUN_TEST(tr, TestArray);
    RUN_TEST(tr, TestObject);
    RUN_TEST(tr, TestAutoClose);

    return 0;
}
