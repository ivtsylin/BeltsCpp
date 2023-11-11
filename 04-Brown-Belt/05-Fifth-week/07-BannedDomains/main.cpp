#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <map>

using namespace std;

struct domains{
    map<string, unique_ptr<domains>> data;
};

void PrintDomains (ostream& c_out, const domains &dom, string offset = "") {
    for (const auto& item : dom.data) {
        c_out << offset << item.first << endl;
        if (item.second.get()) { // Есть набор поддоменов
            PrintDomains(c_out, *item.second.get(), offset + "| ");
        }
    }
}

ostream& operator<< (ostream& c_out, const domains &dom) {
    PrintDomains (c_out, dom);
    return c_out;
}

istream& operator>> (istream& c_in, domains &dom) {
    string str_domain;
    cin >> str_domain;
    string str_subdomain;
    domains* it = &dom;
    if (str_domain.length()) {
        auto pos_end = str_domain.length();
        for (auto i = str_domain.length(); i > 0; --i) {
            if (str_domain[i - 1] == '.') {
                str_subdomain = str_domain.substr(i, pos_end - i);
                auto it_found = it->data.find(str_subdomain);
                if (it_found == it->data.end()) { // Новое значение ключа
                    // Добавляем
                    it->data[str_subdomain] = make_unique<domains>();
                    it = it->data[str_subdomain].get();
                    pos_end = i - 1;
                } else { // Ключ уже есть
                    // Могут быть варианты
                    // Вариант номер 1 -- есть поддомены
                    if (it_found->second.get()) {
                        // Пролистываем и идем дальше
                        it = it_found->second.get();
                        pos_end = i - 1;
                    } else { // Вариант номер 2 -- нет поддоменов (а ключ есть)
                        // Это означает, что новый домен является поддоменом запрещенного домена
                        // Дальше перебирать нет смысла
                        return c_in;
                    }
                }
            }
        }
        str_subdomain = str_domain.substr(0, pos_end);
        it->data[move(str_subdomain)] = nullptr;
    }
    
    return c_in;
}

void ReadBannedDomains(domains &dom) {
    int count = 0;
    cin >> count;
    for (int i = 0; i < count; ++i) {
        cin >> dom;
        //cout << dom;
    }
}

bool CheckBannedDomain(const string& test, const domains& dom) {
    string str_subdomain;
    const domains* it = &dom;
    if (test.length()) {
        auto pos_end = test.length();
        for (auto i = test.length(); i > 0; --i) {
            if (test[i - 1] == '.') {
                str_subdomain = test.substr(i, pos_end - i);
                auto it_found = it->data.find(str_subdomain);
                if (it_found == it->data.end()) { // Не нашли, значит нет исключения
                    return false;
                } else { // Нашли
                    // Надо принять решение, ищем ли дальше
                    // Мы ищем, только если это не терминальный узел дерева
                    if (it_found->second.get()) { // узел не терминальный
                        // Пролистываем и идем дальше
                        it = it_found->second.get();
                        pos_end = i - 1;
                    } else { // это терминальный узел, значит тестовый домен запрещен
                        return true;
                    }
                }
            }
        }
        str_subdomain = test.substr(0, pos_end); // остался последний тест, который не проходится внутри цикла
        auto it_found = it->data.find(str_subdomain);
        if (it_found == it->data.end()) { // Не нашли, значит нет исключения
            return false;
        } else {
            return true;
        }
    }
    
    return false;
}

void CheckBannedDomains(const domains &dom) {
    int count = 0;
    cin >> count;
    string test;
    for (int i = 0; i < count; ++i) {
        cin >> test;
        if (CheckBannedDomain(test, dom)) {
            cout << "Bad" << endl;
        } else {
            cout << "Good" << endl;
        }
    }
}

int main() {
    

    domains BannedDomains;
    ReadBannedDomains(BannedDomains);
    CheckBannedDomains(BannedDomains);
    
    return 0;
}
