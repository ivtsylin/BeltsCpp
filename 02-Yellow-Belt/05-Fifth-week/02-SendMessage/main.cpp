#include <string>

using namespace std;

class INotifier {
public:
    virtual void Notify(const string& message) = 0;
};

class SmsNotifier : public INotifier {
public:
    SmsNotifier(const string& PhoneNum) : PhoneNum_(PhoneNum) {};
    void Notify(const string& message) override {
        SendSms (PhoneNum_, message);
    }
private:
    string PhoneNum_;
};

class EmailNotifier : public INotifier {
public:
    EmailNotifier(const string& email) : email_(email) {};
    void Notify(const string& message) override {
        SendEmail (email_, message);
    };
private:
    string email_;
};

void Notify(INotifier& notifier, const string& message) {
  notifier.Notify(message);
}
