#include <iostream>
#include <vector>
#include <numeric>
#include <ctime>

using namespace std;


enum class Request {
    Earn,
    Compute,
    PayTax,
    Spend
};

struct Request_struct {
    Request resp_type;
    tm date_1;
    tm date_2;
    int value;
};

istream& operator>> (istream& is, tm& date) {
    date = {0};
    int year, month, day;
    is >> year;
    is.ignore(1);
    is >> month;
    is.ignore(1);
    is >> day;
    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day - 1;
    return is;
};

istream& operator>> (istream& is, Request_struct& req) {
    string cmd;
    is >> cmd;
    if (cmd == "Earn") {
        req.resp_type = Request::Earn;
        cin >> req.date_1 >> req.date_2 >> req.value;
    } else if (cmd == "ComputeIncome") {
        req.resp_type = Request::Compute;
        is >> req.date_1 >> req.date_2;
    } else if (cmd == "PayTax") {
        req.resp_type = Request::PayTax;
        is >> req.date_1 >> req.date_2 >> req.value;
    } else if (cmd == "Spend") {
        req.resp_type = Request::Spend;
        cin >> req.date_1 >> req.date_2 >> req.value;
    } else {
        throw invalid_argument("Wrong command");
    }
    return is;
};

class EarnDB {
public:
    EarnDB() {
        tm start = {0}, end = {0};
        start.tm_year = 2000 - 1900;
        end.tm_year = 2099 - 1900;
        end.tm_mon = 11;
        end.tm_mday = 30;
        int size = (mktime(&end) - mktime(&start)) / 24 / 3600;
        db_earnings.resize(size + 1, 0);
        db_spendings.resize(size + 1, 0);
    }
    void Earn ( const tm& date_1, const tm& date_2, int income) {
        auto begin_it = get_begin_earnings_iterator(date_1);
        auto end_it = get_end_earnings_iterator(date_2);
        
        double income_per_day = income;
        income_per_day /= (end_it - begin_it);
        for (auto it = begin_it; it != end_it; ++it) {
            (*it) += income_per_day;
        }
    }
    double ComputeIncome ( const tm& date_1, const tm& date_2) {
        return accumulate(
                          get_begin_earnings_iterator(date_1),
                          get_end_earnings_iterator(date_2),
                          0.0) -
            accumulate(
                          get_begin_spendings_iterator(date_1),
                          get_end_spendings_iterator(date_2),
                          0.0)
        ;
    }
    void PayTax ( const tm& date_1, const tm& date_2, int rate) {
        auto begin_it = get_begin_earnings_iterator(date_1);
        auto end_it = get_end_earnings_iterator(date_2);
        double total_rate = 1. - double(rate) * 0.01;
        for (auto it = begin_it; it != end_it; ++it) {
            (*it) *= total_rate;
        }
    }
    void Spend ( const tm& date_1, const tm& date_2, int spending) {
        auto begin_it = get_begin_spendings_iterator(date_1);
        auto end_it = get_end_spendings_iterator(date_2);
        
        double spending_per_day = spending;
        spending_per_day /= (end_it - begin_it);
        for (auto it = begin_it; it != end_it; ++it) {
            (*it) += spending_per_day;
        }
    }
    
    
private:
    vector<double> db_earnings;
    vector<double> db_spendings;
    vector<double>::iterator get_begin_earnings_iterator (tm date) {
        tm start = {0};
        start.tm_year = 2000 - 1900;
        int size = (mktime(&date) - mktime(&start)) / 24 / 3600;
        return db_earnings.begin() + size;
    }
    vector<double>::iterator get_end_earnings_iterator (tm date) {
        tm start = {0};
        start.tm_year = 2000 - 1900;
        int size = (mktime(&date) - mktime(&start)) / 24 / 3600;
        return db_earnings.begin() + size + 1;
    }
    vector<double>::iterator get_begin_spendings_iterator (tm date) {
        tm start = {0};
        start.tm_year = 2000 - 1900;
        int size = (mktime(&date) - mktime(&start)) / 24 / 3600;
        return db_spendings.begin() + size;
    }
    vector<double>::iterator get_end_spendings_iterator (tm date) {
        tm start = {0};
        start.tm_year = 2000 - 1900;
        int size = (mktime(&date) - mktime(&start)) / 24 / 3600;
        return db_spendings.begin() + size + 1;
    }
};


int main() {
    int N;
    cin >> N;
    EarnDB db;
    std::cout.precision(25);
    for (int i = 0; i < N; ++i) {
        Request_struct req;
        cin >> req;
        if (req.resp_type == Request::Earn) {
            db.Earn(req.date_1, req.date_2, req.value);
        } else if (req.resp_type == Request::Compute) {
            cout << db.ComputeIncome(req.date_1, req.date_2) << endl;
        } else if (req.resp_type == Request::PayTax) {
            db.PayTax(req.date_1, req.date_2, req.value);
        } else if (req.resp_type == Request::Spend) {
            db.Spend(req.date_1, req.date_2, req.value);
        } else {
            throw invalid_argument("Empty request");
        }
    }
    return 0;
}
