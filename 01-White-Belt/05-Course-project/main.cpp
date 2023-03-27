#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

class Date {
private:
    int year;
    int month;
    int day;
public:
    Date (const int init_year = 1, const int init_month = 1, const int init_day = 1) {
        
        if (init_month < 1 || init_month > 12) {
            throw logic_error("Month value is invalid: " + to_string(init_month));
        }
        
        if (init_day < 1 || init_day > 31) {
            throw logic_error("Day value is invalid: " + to_string(init_day));
        }
        
        year = init_year;
        month = init_month;
        day = init_day;
    }
    int GetYear() const {
        return year;
    }
    int GetMonth() const {
        return month;
    }
    int GetDay() const {
        return day;
    }
    bool operator< (const Date &rhs) const {
        return year < rhs.year ||
              (year == rhs.year && (month < rhs.month ||
                                   (month == rhs.month && day < rhs.day)));
    }
    friend ostream& operator<< (ostream& stream, const Date& date);
    friend istream& operator>> (istream& stream, Date& date);
};

ostream& operator<< (ostream& stream, const Date& date) {
    stream  << setw(4) << setfill('0') << date.year << '-'
            << setw(2) << setfill('0') << date.month << '-'
            << setw(2) << setfill('0') << date.day;
    return stream;
}

istream& operator>> (istream& stream, Date& date) {
    string str;
    stream >> str;
    stringstream sstr(str);
    int year, month, day;
    bool ok = true;
    
    ok = ok && (sstr >> year);
    ok = ok && (sstr.peek() == '-');
    sstr.ignore(1);
    
    ok = ok && (sstr >> month);
    ok = ok && (sstr.peek() == '-');
    sstr.ignore(1);
    
    ok = ok && (sstr >> day);
    ok = ok && sstr.eof();
    
    if (!ok) {
        throw logic_error("Wrong date format: " + str);
    }
    
    date = Date(year, month, day);
    
    return stream;
}

class Database {
private:
    map<Date, set<string>> DB;
public:
    void AddEvent(const Date& date, const string& event) {
        DB[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const string& event) {
        if (DB.count(date)) {
            if (DB[date].count(event)) {
                DB[date].erase(event);
                return true;
            }
        }
        return false;
    }
    int DeleteDate(const Date& date) {
        int res = 0;
        if (DB.count(date)) {
            res = DB[date].size();
            DB.erase(date);
        }
        return res;
    }
    set<string> Find(const Date& date) const {
        set<string> res;
        if (DB.count(date)) {
            return DB.at(date);
        }
        return res;
    }
    
    void Print() const {
        for (const auto& [key, value] : DB) {
            for (const auto& value_item : value) {
                cout << key << " " << value_item << endl;
            }
        }
    }
    
};

int main() {
    Database db;
    Date date;
    string event;
    string line_from_cin;
    string command;
    
    while (getline(cin, line_from_cin)) {
        if (line_from_cin == "") {
            continue;
        }
        stringstream str(line_from_cin);
        str >> command;
        if ((command == "Add") || (command == "Del") || (command == "Find")) {
            try {
                str >> date;
            } catch (exception& ex) {
                cout << ex.what() << endl;
                continue;
            }
            if (command == "Add") {
                str >> event;
                db.AddEvent(date, event);
            } else if (command == "Del") {
                if (str.eof()) {
                    // Удаление всех событий
                    int cnt_del_obj;
                    cnt_del_obj = db.DeleteDate(date);
                    cout << "Deleted " << cnt_del_obj << " events" << endl;
                } else {
                    bool is_del_event;
                    str >> event;
                    is_del_event = db.DeleteEvent(date, event);
                    if (is_del_event) {
                        cout << "Deleted successfully" << endl;
                    } else {
                        cout << "Event not found" << endl;
                    }
                }
            } else if (command == "Find") {
                set<string> events;
                events = db.Find(date);
                for (const auto& s : events) {
                    cout << s << endl;
                }
            }
        } else if (command == "Print") {
            db.Print();
        } else {
            cout << "Unknown command: " << command << endl;
        }
    }
    return 0;
}
