#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

class Person {
public:
    Person() {};
    void ChangeFirstName(int year, const string& first_name) {
        // добавить факт изменения имени на first_name в год year
        history_first_name[year] = first_name;
    }
    void ChangeLastName(int year, const string& last_name) {
        // добавить факт изменения фамилии на last_name в год year
        history_last_name[year] = last_name;
    }
    void PrintPerson () {
        set<int> tmp;
        for (const auto& [key, value] : history_first_name) {
            tmp.insert(key);
        }
        cout << "First names" << endl;
        for (int i : tmp) {
            cout << i << " " << history_first_name[i] << endl;
        }
        cout << "Last names" << endl;
        tmp.clear();
        for (const auto& [key, value] : history_last_name) {
            tmp.insert(key);
        }
        for (int i : tmp) {
            cout << i << " " << history_last_name[i] << endl;
        }
    }
    string GetFullName(int year) {
        // получить имя и фамилию по состоянию на конец года year
        string  actual_first_name = get_name(year, history_first_name),
                actual_last_name = get_name(year, history_last_name);
        
        if (actual_first_name.empty() && actual_last_name.empty()) {
            return "Incognito";
        }
        
        if (actual_last_name.empty()) {
            return actual_first_name + " with unknown last name";
        }
        
        if (actual_first_name.empty()) {
            return actual_last_name + " with unknown first name";
        }

        return actual_first_name + " " + actual_last_name;
    }
    string GetFullNameWithHistory(int year) {
        // получить все имена и фамилии по состоянию на конец года year
        vector<string>  vec_first_name = get_name_history(year, history_first_name),
                        vec_last_name = get_name_history(year, history_last_name);
        
        if (!vec_first_name.size() && !vec_last_name.size()) {
            return "Incognito";
        }
        
        if (!vec_last_name.size()) {
            return print_from_history(vec_first_name) + " with unknown last name";
        }
        
        if (!vec_first_name.size()) {
            return print_from_history(vec_last_name) + " with unknown first name";
        }

        return print_from_history(vec_first_name) + " " + print_from_history(vec_last_name);
    }
private:
    // приватные поля
    map<int, string> history_first_name;
    map<int, string> history_last_name;
    string get_name(const int year, const map<int, string>& name) {
        string result;
        for (const auto& [key, value] : name) {
            if (year < key) {
                break;
            }
            result = value;
        }
        return result;
    };
    vector<string> get_name_history(const int year, const map<int, string>& name) {
        vector<string> result;
        for (const auto& [key, value] : name) {
            if (year < key) {
                break;
            }
            if ((result.empty() || result.back() != value)) {
                result.push_back(value);
            }
        }
        return result;
    }
    string print_from_history (const vector<string>& source) {
        string result;
        if (source.size() == 0) {
            return "";
        }
        if (source.size() == 1) {
            return source[0];
        }
        result = source.back();
        result += " (";
        int first_iter = source.size() - 2;
        for (int i = first_iter; i >= 0; --i) {
            result += source[i];
            if (i != 0) {
                result += ", ";
            }
        }
        result += ")";
        return result;
    }
};

int main() {
  Person person;
  
    person.ChangeFirstName(1965, "Polina");
      person.ChangeLastName(1967, "Sergeeva");
      for (int year : {1900, 1965, 1990}) {
        cout << person.GetFullNameWithHistory(year) << endl;
      }
      
      person.ChangeFirstName(1970, "Appolinaria");
      for (int year : {1969, 1970}) {
        cout << person.GetFullNameWithHistory(year) << endl;
      }
      
      person.ChangeLastName(1968, "Volkova");
      for (int year : {1969, 1970}) {
        cout << person.GetFullNameWithHistory(year) << endl;
      }
      
      person.ChangeFirstName(1990, "Polina");
      person.ChangeLastName(1990, "Volkova-Sergeeva");
      cout << person.GetFullNameWithHistory(1990) << endl;
      
      person.ChangeFirstName(1966, "Pauline");
      cout << person.GetFullNameWithHistory(1966) << endl;
      
      person.ChangeLastName(1960, "Sergeeva");
      for (int year : {1960, 1967}) {
        cout << person.GetFullNameWithHistory(year) << endl;
      }
      
      person.ChangeLastName(1961, "Ivanova");
      cout << person.GetFullNameWithHistory(1967) << endl;
  
  return 0;
}
