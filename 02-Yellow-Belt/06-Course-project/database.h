#pragma once
#pragma inline

#include <map>
#include <set>
#include <string>
#include <iostream>
#include <utility>
#include <tuple>
#include <vector>
#include <list>

#include "date.h"

using namespace std;

/*
 database.h/cpp — эти файлы должны содержать объявление и определение класса Database;
 */

//using DB_iter = set<pair<Date, string>>::const_iterator;
//using DB_cmp_type = bool (const Date&, const string&);

using DB_iter = set<pair<Date, string>>::const_iterator;

class Database {
public:
    set<pair<Date, string>> DB;
    map<Date, list<DB_iter>> DB_partition;
public:
    void Add(const Date& date, const string& event);
    void Print(ostream& os) const;
    template<class Func>
    int RemoveIf(Func bool_cond_func) {
        int dec_size = 0;
        DB_iter it_db_item;
        // Проходим по разбиению
        // Сначала итерируемся по дате
        for (auto it_part = DB_partition.begin(); it_part != DB_partition.end(); ) {
            // Внутри итерируемся по списку итераторов
            for (auto it_list_iter = it_part->second.begin(); it_list_iter != it_part->second.end(); ) {
                // Для удобства получаем итератор на пару (date, event)
                it_db_item = *it_list_iter;
                // Проверяем, надо ли удалить элемент
                if (bool_cond_func(it_db_item->first, it_db_item->second)) {
                    // Надо удалить
                    // Сначала удаляем из разбиения
                    it_list_iter = it_part->second.erase(it_list_iter);
                    // Теперь удаляем физически
                    DB.erase(it_db_item);
                    // Увеличиваем количество удаленных элементов
                    ++dec_size;
                } else {
                    ++it_list_iter;
                }
            }
            // Проверяем, остались ли элементы в разбиении за дату
            if (it_part->second.empty()) {
                // Удалились все элементы по дате
                it_part = DB_partition.erase(it_part);
            } else {
                ++it_part;
            }
        }
        return dec_size;
    }
    template<class Func>
    list<DB_iter> FindIf(Func bool_cond_func) const {
        list<DB_iter> result;
        DB_iter it_db_item;
        for (auto it_part = DB_partition.begin(); it_part != DB_partition.end(); ++it_part) {
            for (auto it_list_iter = it_part->second.begin(); it_list_iter != it_part->second.end(); ++it_list_iter) {
                // Для удобства получаем итератор на пару (date, event)
                it_db_item = *it_list_iter;
                // Проверяем, запомнить элемент
                if (bool_cond_func(it_db_item->first, it_db_item->second)) {
                    result.push_back(it_db_item);
                }
            }
        }
        return result;
    }
    pair<Date, string> Last(const Date& date_) const;
};

ostream& operator<<(ostream& os, pair<Date, string>);
ostream& operator<<(ostream& os, DB_iter item);
