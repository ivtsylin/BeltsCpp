#include <iostream>
#include <map>

using namespace std;

class Person {
public:
    void ChangeFirstName(int year, const string& first_name) {
        // добавить факт изменения имени на first_name в год year
        FirstName [ year ] = first_name;
    }
    void ChangeLastName(int year, const string& last_name) {
        // добавить факт изменения фамилии на last_name в год year
        LastName [ year ] = last_name;
    }
    string GetFullName(int year) {
        // получить имя и фамилию по состоянию на конец года year
        // с помощью двоичного поиска
        string answer;
        string FirstNameByYear, LastNameByYear;
        bool KnownFirstName, KnownLastName;
        
        KnownFirstName = GetName (year, FirstName, FirstNameByYear);
        KnownLastName = GetName (year, LastName, LastNameByYear);
        
        if (!KnownFirstName && !KnownLastName) {
            return "Incognito";
        }
        
        if (!KnownFirstName) {
            return LastNameByYear + " with unknown first name";
        }
        
        if (!KnownLastName) {
            return FirstNameByYear + " with unknown last name";
        }
        
        return FirstNameByYear + " " + LastNameByYear;
    }
private:
    map<int, string> FirstName;
    map<int, string> LastName;
    bool GetName(const int year, const map<int, string>& name, string& answer) const {
         
        if (name.size() == 0) {
            return false;
        }
        auto it = name.lower_bound(year);
        if (it == name.end()) {
            answer = prev(it) -> second;
            return true;
        }
        if (it -> first <= year){
            answer = it -> second;
            return true;
        }
        if (it == name.begin()) {
            return false;
        }
        answer = prev(it) -> second;
        return true;
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
