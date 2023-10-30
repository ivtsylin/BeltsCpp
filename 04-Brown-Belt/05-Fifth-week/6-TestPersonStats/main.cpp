#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "test_runner.h"

using namespace std;

enum class Gender {
  FEMALE,
  MALE
};

struct Person {
  int age;
  Gender gender;
  bool is_employed;
};

bool operator==(const Person& lhs, const Person& rhs) {
  return lhs.age == rhs.age
      && lhs.gender == rhs.gender
      && lhs.is_employed == rhs.is_employed;
}

ostream& operator<<(ostream& stream, const Person& person) {
  return stream << "Person(age=" << person.age
      << ", gender=" << static_cast<int>(person.gender)
      << ", is_employed=" << boolalpha << person.is_employed << ")";
}

struct AgeStats {
  int total;
  int females;
  int males;
  int employed_females;
  int unemployed_females;
  int employed_males;
  int unemployed_males;
};

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
  if (range_begin == range_end) {
    return 0;
  }
  vector<typename iterator_traits<InputIt>::value_type> range_copy(
      range_begin,
      range_end
  );
  auto middle = begin(range_copy) + range_copy.size() / 2;
  nth_element(
      begin(range_copy), middle, end(range_copy),
      [](const Person& lhs, const Person& rhs) {
        return lhs.age < rhs.age;
      }
  );
  return middle->age;
}

vector<Person> ReadPersons(istream& in_stream = cin) {
  int person_count;
  in_stream >> person_count;
  vector<Person> persons;
  persons.reserve(person_count);
  for (int i = 0; i < person_count; ++i) {
    int age, gender, is_employed;
    in_stream >> age >> gender >> is_employed;
    Person person{
        age,
        static_cast<Gender>(gender),
        is_employed == 1
    };
    persons.push_back(person);
  }
  return persons;
}

AgeStats ComputeStats(vector<Person> persons) {
  //                 persons
  //                |       |
  //          females        males
  //         |       |      |     |
  //      empl.  unempl. empl.   unempl.

  auto females_end = partition(
      begin(persons), end(persons),
      [](const Person& p) {
        return p.gender == Gender::FEMALE;
      }
  );
  auto employed_females_end = partition(
      begin(persons), females_end,
      [](const Person& p) {
        return p.is_employed;
      }
  );
  auto employed_males_end = partition(
      females_end, end(persons),
      [](const Person& p) {
        return p.is_employed;
      }
  );

  return {
       ComputeMedianAge(begin(persons), end(persons)),
       ComputeMedianAge(begin(persons), females_end),
       ComputeMedianAge(females_end, end(persons)),
       ComputeMedianAge(begin(persons),  employed_females_end),
       ComputeMedianAge(employed_females_end, females_end),
       ComputeMedianAge(females_end, employed_males_end),
       ComputeMedianAge(employed_males_end, end(persons))
  };
}

void PrintStats(const AgeStats& stats,
                ostream& out_stream = cout) {
  out_stream << "Median age = "
             << stats.total              << endl
             << "Median age for females = "
             << stats.females            << endl
             << "Median age for males = "
             << stats.males              << endl
             << "Median age for employed females = "
             << stats.employed_females   << endl
             << "Median age for unemployed females = "
             << stats.unemployed_females << endl
             << "Median age for employed males = "
             << stats.employed_males     << endl
             << "Median age for unemployed males = "
             << stats.unemployed_males   << endl;
}

void TestEmpty() {
    vector<Person> persons = {};
    
    ASSERT_EQUAL(ComputeStats(persons).employed_females, 0);
    ASSERT_EQUAL(ComputeStats(persons).employed_males, 0);
    ASSERT_EQUAL(ComputeStats(persons).females, 0);
    ASSERT_EQUAL(ComputeStats(persons).males, 0);
    ASSERT_EQUAL(ComputeStats(persons).total, 0);
    ASSERT_EQUAL(ComputeStats(persons).unemployed_females, 0);
    ASSERT_EQUAL(ComputeStats(persons).unemployed_males, 0);

}

void TestStandard() {
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
    
    ASSERT_EQUAL(ComputeStats(persons).total, 40);
    ASSERT_EQUAL(ComputeStats(persons).females, 40);
    ASSERT_EQUAL(ComputeStats(persons).males, 55);
    ASSERT_EQUAL(ComputeStats(persons).employed_females, 40);
    ASSERT_EQUAL(ComputeStats(persons).unemployed_females, 80);
    ASSERT_EQUAL(ComputeStats(persons).employed_males, 55);
    ASSERT_EQUAL(ComputeStats(persons).unemployed_males, 78);

}

void TestEven() {
    vector<Person> persons = {
        {10, Gender::MALE, false},
        {20, Gender::FEMALE, true},
        {30, Gender::MALE, true},
    };
    
    ASSERT_EQUAL(ComputeStats(persons).total, 20);
}

void TestReadPersons() {
    stringstream s;
    s << 4 << endl;
    s << "10 0 0" << endl; // {10, Gender::FEMALE, false},
    s << "20 0 1" << endl; // {20, Gender::FEMALE, true},
    s << "30 1 0" << endl; // {30, Gender::MALE, false},
    s << "40 1 1" << endl; // {40, Gender::MALE, true},
    
    vector<Person> persons_lhs = ReadPersons(s);
    vector<Person> persons_rhs = {
        {10, Gender::FEMALE, false},
        {20, Gender::FEMALE, true},
        {30, Gender::MALE, false},
        {40, Gender::MALE, true},
    };
    
    ASSERT_EQUAL(persons_lhs, persons_rhs);
    
}

void TestPrintStats() {
    
    stringstream ss_lhs;
    PrintStats({10, 20, 30, 40, 50, 60, 70}, ss_lhs);
    
    stringstream ss_rhs;
    
    ss_rhs << "Median age = "
               << 10              << endl
               << "Median age for females = "
               << 20            << endl
               << "Median age for males = "
               << 30              << endl
               << "Median age for employed females = "
               << 40   << endl
               << "Median age for unemployed females = "
               << 50 << endl
               << "Median age for employed males = "
               << 60     << endl
               << "Median age for unemployed males = "
               << 70   << endl;
    
    ASSERT_EQUAL(ss_lhs.str(), ss_rhs.str());
    
}

int main() {
    
    TestRunner tr;
    RUN_TEST(tr, TestEmpty);
    RUN_TEST(tr, TestStandard);
    RUN_TEST(tr, TestEven);
    RUN_TEST(tr, TestReadPersons);
    RUN_TEST(tr, TestPrintStats);
    
    PrintStats(ComputeStats(ReadPersons()));
    return 0;
}
