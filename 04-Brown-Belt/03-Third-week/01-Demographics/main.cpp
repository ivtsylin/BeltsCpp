#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) : first(begin), last(end) {}
    Iterator begin() const {
        return first;
    }
    Iterator end() const {
        return last;
    }
private:
    Iterator first, last;
};

//template <typename Collection>
//auto Head(Collection& v, size_t top) {
//    return IteratorRange{v.begin(), next(v.begin(), min(top, v.size()))};
//}

enum class Gender {
    Male,
    Female
};

struct Person {
    string name;
    int age, income;
    Gender gender;
};

class People {
public:
    People() : popular_name_f(""), popular_name_m("") {};
    void ReadPeople(istream& input) {
        int count;
        input >> count;

        data.resize(count);
        
        wealthy_index.resize(count);
        iota(wealthy_index.begin(), wealthy_index.end(), 0);
        
        for (Person& p : data) {
            char gender;
            input >> p.name >> p.age >> p.income >> gender;
            p.gender = gender == 'M' ? Gender::Male : Gender::Female;
        }
        
        ComputePopularName();
        
        vector<Person>& to_lambda = data;
        
        sort(data.begin(), data.end(), [](const Person& lhs, const Person& rhs) { return lhs.age < rhs.age;});
        sort(wealthy_index.begin(),
             wealthy_index.end(),
             [&to_lambda](const int &lhs_ind, const int &rhs_ind) {return to_lambda[lhs_ind].income > to_lambda[rhs_ind].income;});
    }
    string GetPopularNameMale() const {
        return popular_name_m;
    }
    string GetPopularNameFemale() const {
        return popular_name_f;
    }
    int GetWealth (int count) const {
        int result = 0;
        for (int i = 0; i < min(wealthy_index.size(), size_t(count)); ++i) {
            result += data[wealthy_index[i]].income;
        }
        return result;
    }
    int GetMaturity(int adult_age) const {
        auto adult_begin = lower_bound(data.begin(), data.end(), adult_age, [](const Person& lhs, int age) { return lhs.age < age;});
        return distance(adult_begin, data.end());
    }
private:
    void ComputePopularName() {
        auto part = partition(data.begin(), data.end(), [](const Person& p){return p.gender == Gender::Male;});
        sort(data.begin(), part, [](const Person& lhs, const Person& rhs){return lhs.name < rhs.name;});
        sort(part, data.end(), [](const Person& lhs, const Person& rhs){return lhs.name < rhs.name;});
        
        int max_count = 0;
        int cur_count = 0;
        string cur_value = "";
        
        for (auto iter = data.begin(); iter < part; ++iter) {
            if (iter->name == cur_value) {
                ++cur_count;
            } else {
                if (cur_count > max_count) {
                    max_count = cur_count;
                    popular_name_m = cur_value;
                }
                cur_count = 1;
                cur_value = iter->name;
            }
        }
        if (cur_count > max_count) {
            popular_name_m = cur_value;
        }
        
        max_count = 0;
        cur_count = 0;
        cur_value = "";
        
        for (auto iter = part; iter < data.end(); ++iter) {
            if (iter->name == cur_value) {
                ++cur_count;
            } else {
                if (cur_count > max_count) {
                    max_count = cur_count;
                    popular_name_f = cur_value;
                }
                cur_count = 1;
                cur_value = iter->name;
            }
        }
        if (cur_count > max_count) {
            popular_name_f = cur_value;
        }
    }
    
    vector<Person> data;
    vector<int> wealthy_index;
    string popular_name_m;
    string popular_name_f;
};

int main() {
    People people;
    people.ReadPeople(cin);
    
    for (string command; cin >> command; ) {
        if (command == "AGE") {
            int adult_age;
            cin >> adult_age;
            cout << "There are " << people.GetMaturity(adult_age) << " adult people for maturity age " << adult_age << '\n';
        } else if (command == "WEALTHY") {
            int count;
            cin >> count;
            cout << "Top-" << count << " people have total income " << people.GetWealth(count) << '\n';
        } else if (command == "POPULAR_NAME") {
            char gender;
            cin >> gender;
            string result = gender == 'M' ? people.GetPopularNameMale() : people.GetPopularNameFemale();
            if (result == "") {
                cout << "No people of gender " << gender << '\n';
            } else {
                cout << "Most popular name among people of gender " << gender << " is " << result << '\n';
            }
        }
    }
}
