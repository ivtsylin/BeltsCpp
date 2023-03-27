#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct Student {
    string first_name;
    string last_name;
    int day;
    int month;
    int year;
};

int main() {
    vector<Student> students;
    int n;
    cin >> n;
    int day, month, year;
    string first_name, last_name;
    for (int i = 0; i < n; ++i) {
        cin >> first_name >> last_name >> day >> month >> year;
        students.push_back({first_name, last_name, day, month, year});
    }
    int m;
    cin >> m;
    string cmnd;
    int num;
    for (int i = 0; i < m; ++i) {
        cin >> cmnd >> num;
        if (cmnd == "name" && num > 0 && num <= n) {
            cout << students[num - 1].first_name << " " << students[num - 1].last_name << endl;
        } else if (cmnd == "date" && num > 0 && num <= n) {
            cout << students[num - 1].day << "." << students[num - 1].month << "." << students[num - 1].year << endl;
        } else {
            cout << "bad request" << endl;
        }
    }
    return 0;
}

