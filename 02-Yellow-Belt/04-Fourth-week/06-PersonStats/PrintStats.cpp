#include <vector>
#include <algorithm>

using namespace std;

#include <sstream>

void PrintStats(vector<Person> persons) {
    cout << "Median age = " << ComputeMedianAge(persons.begin(), persons.end()) << endl;
    auto fem_it = partition(persons.begin(), persons.end(), [](const Person& pers) {return pers.gender == Gender::FEMALE;});
    cout << "Median age for females = " << ComputeMedianAge(persons.begin(), fem_it) << endl;
    cout << "Median age for males = " << ComputeMedianAge(fem_it, persons.end()) << endl;
    auto fem_empl_it = partition(persons.begin(), fem_it, [](const Person& pers) {return pers.is_employed;});
    cout << "Median age for employed females = " << ComputeMedianAge(persons.begin(), fem_empl_it) << endl;
    cout << "Median age for unemployed females = " << ComputeMedianAge(fem_empl_it, fem_it) << endl;
    auto male_empl_it = partition(fem_it, persons.end(), [](const Person& pers) {return pers.is_employed;});
    cout << "Median age for employed males = " << ComputeMedianAge(fem_it, male_empl_it) << endl;
    cout << "Median age for unemployed males = " << ComputeMedianAge(male_empl_it, persons.end()) << endl;
};

