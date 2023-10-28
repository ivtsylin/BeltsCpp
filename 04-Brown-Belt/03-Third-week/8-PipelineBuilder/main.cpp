#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


struct Email {
  string from;
  string to;
  string body;
};


class Worker {
public:
    virtual ~Worker() = default;
    virtual void Process(unique_ptr<Email> email) = 0;
    virtual void Run() {
        // только первому worker-у в пайплайне нужно это имплементировать
        throw logic_error("Unimplemented");
    }
    
protected:
    // реализации должны вызывать PassOn, чтобы передать объект дальше
    // по цепочке обработчиков
    void PassOn(unique_ptr<Email> email) const {
        if (next_worker.get())
            next_worker->Process(move(email));
    }
public:
    void SetNext(unique_ptr<Worker> next) {
        next_worker = move(next);
    }
private:
    unique_ptr<Worker> next_worker;
};


class Reader : public Worker {
public:
    Reader(istream& cin) : cin_(cin) {};
    void Run() override {
        string line;
        while(getline(cin_, line)) {
            Email item;
            item.from = move(line);
            getline(cin_, line);
            item.to = move(line);
            getline(cin_, line);
            item.body = move(line);
            auto ptr = make_unique<Email>(move(item));
            // Do Something
            PassOn(move(ptr));
        }
    }
    void Process(unique_ptr<Email> email) override {}
private:
    istream& cin_;
};


class Filter : public Worker {
public:
    using Function = function<bool(const Email&)>;
    Filter(Function f) : f_(f) {};
    void Process(unique_ptr<Email> email) override {
        if(f_(*email)) {
            PassOn(move(email));
        }
    }

private:
    Function f_;
};


class Copier : public Worker {
public:
    // реализуйте класс
    Copier(string new_to) : new_to_(move(new_to)) {}
    void Process(unique_ptr<Email> email) override {
        if (email->to != new_to_) {
            auto email2 = make_unique<Email>();
            email2->to = new_to_;
            email2->from = email->from;
            email2->body = email->body;
            PassOn(move(email));
            PassOn(move(email2));
        } else {
            PassOn(move(email));
        }
    }
    
private:
    string new_to_;
};


class Sender : public Worker {
public:
    Sender (ostream& cout) : cout_(cout) {};
    void Process(unique_ptr<Email> email) override {
        cout_ << email->from << '\n' << email->to << '\n' << email->body << '\n';
        PassOn(move(email));
    }
private:
    ostream& cout_;
};


// реализуйте класс
class PipelineBuilder {
public:
    // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream& in) : fptr_(make_unique<Reader>(in)), lptr_(fptr_.get()) {}

    // добавляет новый обработчик Filter
    PipelineBuilder& FilterBy(Filter::Function filter) {
        auto new_ptr = make_unique<Filter>(move(filter));
        InsertWorker(move(new_ptr));
        return *this;
    }

    // добавляет новый обработчик Copier
    PipelineBuilder& CopyTo(string recipient) {
        auto new_ptr = make_unique<Copier>(move(recipient));
        InsertWorker(move(new_ptr));
        return *this;
    }

    // добавляет новый обработчик Sender
    PipelineBuilder& Send(ostream& out) {
        auto new_ptr = make_unique<Sender>(out);
        InsertWorker(move(new_ptr));
        return *this;
    }

    // возвращает готовую цепочку обработчиков
    unique_ptr<Worker> Build() {
        return move(fptr_);
    }
private:
    void InsertWorker(unique_ptr<Worker> ptr) {
        auto new_lptr_ = ptr.get();
        lptr_->SetNext(move(ptr));
        lptr_ = new_lptr_;
    }
    unique_ptr<Worker> fptr_;
    Worker* lptr_;
};


void TestSanity() {
  string input = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "ralph@example.com\n"
    "erich@example.com\n"
    "I do not make mistakes of that kind\n"
  );
  istringstream inStream(input);
  ostringstream outStream;

  PipelineBuilder builder(inStream);
  builder.FilterBy([](const Email& email) {
    return email.from == "erich@example.com";
  });
  builder.CopyTo("richard@example.com");
  builder.Send(outStream);
  auto pipeline = builder.Build();

  pipeline->Run();

  string expectedOutput = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "erich@example.com\n"
    "richard@example.com\n"
    "Are you sure you pressed the right button?\n"
  );

  ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSanity);
  return 0;
}
