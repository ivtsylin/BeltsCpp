#pragma once

#include <iostream>

using namespace std;

/*
 date.h/cpp — эти файлы должны содержать объявления и определения класса Date, функции ParseDate и оператора вывода в поток для класса Date;
 */

class Date {
private:
    int year;
    int month;
    int day;
public:
    Date (const int init_year = 1, const int init_month = 1, const int init_day = 1);
    int GetYear() const;
    int GetMonth() const;
    int GetDay() const;
    bool operator< (const Date &rhs) const;
    bool operator<= (const Date &rhs) const;
    bool operator> (const Date &rhs) const;
    bool operator>= (const Date &rhs) const;
    bool operator== (const Date &rhs) const;
    bool operator!= (const Date &rhs) const;
    friend ostream& operator<< (ostream& stream, const Date& date);
    friend istream& operator>> (istream& stream, Date& date);
};

ostream& operator<< (ostream& stream, const Date& date);

istream& operator>> (istream& stream, Date& date);

Date ParseDate(istream& is);

