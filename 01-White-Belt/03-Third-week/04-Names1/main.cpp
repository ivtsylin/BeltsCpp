#include <iostream>
#include <map>
#include <string>
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
private:
    // приватные поля
    map<int, string> history_first_name;
    map<int, string> history_last_name;
    string get_name(const int year, map<int, string>& name) {
        string result;
        if (!name.size()) {
            return "";
        }
        for (const auto& [key, value] : name) {
            if (year < key) {
                break;
            }
            result = value;
        }
        return result;
    };
};

int main() {
    Person person;
      
      person.ChangeFirstName(1965, "Polina");
      person.ChangeLastName(1967, "Sergeeva");
      for (int year : {1900, 1965, 1990}) {
        cout << person.GetFullName(year) << endl;
      }
      
      person.ChangeFirstName(1970, "Appolinaria");
      for (int year : {1969, 1970}) {
        cout << person.GetFullName(year) << endl;
      }
      
      person.ChangeLastName(1968, "Volkova");
      for (int year : {1969, 1970}) {
        cout << person.GetFullName(year) << endl;
      }
    return 0;
}
