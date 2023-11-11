#include "test_runner.h"
#include "Person.h"

// Это пример функции, его не нужно отправлять вместе с функцией PrintStats
template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
  if (range_begin == range_end) {
    return 0;
  }
  vector<typename InputIt::value_type> range_copy(range_begin, range_end);
  auto middle = begin(range_copy) + range_copy.size() / 2;
  nth_element(
      begin(range_copy), middle, end(range_copy),
      [](const Person& lhs, const Person& rhs) {
        return lhs.age < rhs.age;
      }
  );
  return middle->age;
}

void PrintStats(const vector<Person>& persons) {
    auto tmp_persons = persons;
    cout << "Median age = " << ComputeMedianAge(tmp_persons.begin(), tmp_persons.end()) << '\n';
    
    auto fem_it = partition(tmp_persons.begin(), tmp_persons.end(), [](const Person &p) {return p.gender == Gender::FEMALE;});
    cout << "Median age for females = " << ComputeMedianAge(tmp_persons.begin(), fem_it) << '\n';
    cout << "Median age for males = " << ComputeMedianAge(fem_it, tmp_persons.end()) << '\n';
    
    auto fem_empl_it = partition(tmp_persons.begin(), fem_it, [](const Person &p) {return p.gender == Gender::FEMALE && p.is_employed;});
    cout << "Median age for employed females = " << ComputeMedianAge(tmp_persons.begin(), fem_empl_it) << '\n';
    cout << "Median age for unemployed females = " << ComputeMedianAge(fem_empl_it, fem_it) << '\n';
    
    auto m_empl_it = partition(fem_it, tmp_persons.end(), [](const Person &p) {return p.gender == Gender::MALE && p.is_employed;});
    cout << "Median age for employed males = " << ComputeMedianAge(fem_it, m_empl_it) << '\n';
    cout << "Median age for unemployed males = " << ComputeMedianAge(m_empl_it, tmp_persons.end()) << endl;
    
}

int main() {
  vector<Person> persons = {
      {31, Gender::MALE, false},
      {40, Gender::FEMALE, true},
      {24, Gender::MALE, true},
      {20, Gender::FEMALE, true},
      {80, Gender::FEMALE, false},
      {78, Gender::MALE, false},
      {10, Gender::FEMALE, false},
      {55, Gender::MALE, true},
  };
  PrintStats(persons);
  return 0;
}


