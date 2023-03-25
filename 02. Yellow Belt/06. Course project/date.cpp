#include "date.h"

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <tuple>

using namespace std;

/*
 date.h/cpp — эти файлы должны содержать объявления и определения класса Date, функции ParseDate и оператора вывода в поток для класса Date;
 */

Date::Date (const int init_year, const int init_month, const int init_day) : year(init_year), month(init_month), day(init_day) {
        
    if (init_month < 1 || init_month > 12) {
        throw logic_error("Month value is invalid: " + to_string(init_month));
    }
        
    if (init_day < 1 || init_day > 31) {
        throw logic_error("Day value is invalid: " + to_string(init_day));
    }

}

int Date::GetYear() const {
    return year;
}

int Date::GetMonth() const {
    return month;
}

int Date::GetDay() const {
    return day;
}

bool Date::operator< (const Date &rhs) const {
    return tie(year, month, day) < tie(rhs.year, rhs.month, rhs.day);
}

bool Date::operator<= (const Date &rhs) const {
    return tie(year, month, day) <= tie(rhs.year, rhs.month, rhs.day);
}

bool Date::operator> (const Date &rhs) const {
    return tie(year, month, day) > tie(rhs.year, rhs.month, rhs.day);
}

bool Date::operator>= (const Date &rhs) const {
    return tie(year, month, day) >= tie(rhs.year, rhs.month, rhs.day);
}

bool Date::operator== (const Date &rhs) const {
    return tie(year, month, day) == tie(rhs.year, rhs.month, rhs.day);
}

bool Date::operator!= (const Date &rhs) const {
    return tie(year, month, day) != tie(rhs.year, rhs.month, rhs.day);
}

ostream& operator<< (ostream& stream, const Date& date) {
    stream  << setw(4) << setfill('0') << date.year << '-'
            << setw(2) << setfill('0') << date.month << '-'
            << setw(2) << setfill('0') << date.day;
    return stream;
}

istream& operator>> (istream& stream, Date& date) {
    string str;
    stream >> str;
    stringstream sstr(str);
    int year, month, day;
    bool ok = true;
    
    ok = ok && (sstr >> year);
    ok = ok && (sstr.peek() == '-');
    sstr.ignore(1);
    
    ok = ok && (sstr >> month);
    ok = ok && (sstr.peek() == '-');
    sstr.ignore(1);
    
    ok = ok && (sstr >> day);
    ok = ok && sstr.eof();
    
    if (!ok) {
        throw logic_error("Wrong date format: " + str);
    }
    
    date = Date(year, month, day);
    
    return stream;
}

Date ParseDate(istream& is) {
    Date result;
    is >> result;
    return result;
}
