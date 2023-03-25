#include "database.h"
#include "date.h"

#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <tuple>

using namespace std;

/*
 database.h/cpp — эти файлы должны содержать объявление и определение класса Database;
 */

void Database::Add(const Date& date, const string& event) {
    auto [it, new_el] = DB.insert(make_pair(date, event));
    if (new_el) {
        DB_partition[date].push_back(it);
    }
}
    
void Database::Print(ostream& os) const {

    DB_iter it_db_item;
    for (auto it_part = DB_partition.begin(); it_part != DB_partition.end(); ++it_part) {
        for (auto it_list_iter = it_part->second.begin(); it_list_iter != it_part->second.end(); ++it_list_iter) {
            // Для удобства получаем итератор на пару (date, event)
            it_db_item = *it_list_iter;
            os << it_db_item->first << " " << it_db_item->second << endl;
        }
    }
}

pair<Date, string> Database::Last(const Date& date) const {
    
    const auto ub = DB_partition.upper_bound(date);
    
    if (ub == DB_partition.begin()) {
        throw invalid_argument("");
    }
    
    return *prev(ub)->second.back();
};

ostream& operator<<(ostream& os, pair<Date, string> item) {
    os << item.first << " " << item.second;
    return os;
};

ostream& operator<<(ostream& os, DB_iter item) {
    os << *item;
    return os;
};


