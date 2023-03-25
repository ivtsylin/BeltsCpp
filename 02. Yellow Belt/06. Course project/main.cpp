#include "database.h"
#include "date.h"
#include "condition_parser.h"
//#include "node.h"
//#include "test_runner.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

string ParseEvent(istream& is)
{
  string event;
  getline(is, event);

  auto first = event.find_first_not_of(" ");

  if (first == string::npos)
    return string();
  
  event.erase(0, first);
  return event;
}

int main() {
    //TestAll();

    Database db;

    for (string line; getline(cin, line); ) {
        istringstream is(line);

        string command;
        is >> command;
    if (command == "Add") {
        const auto date = ParseDate(is);
        const auto event = ParseEvent(is);
        db.Add(date, event);
    } else if (command == "Print") {
        db.Print(cout);
    } else if (command == "Del") {
        auto condition = ParseCondition(is);
        auto predicate = [condition](const Date& date, const string& event) {
            return condition->Evaluate(date, event);
        };
        //auto predicate = [condition](const Date& date, const string& event) {
        //    return date == Date(2017,7,8);
        //};
        int count = db.RemoveIf(predicate);
        cout << "Removed " << count << " entries" << endl;
    } else if (command == "Find") {
        auto condition = ParseCondition(is);
          
          
          
        auto predicate = [condition](const Date& date, const string& event) {
          return condition->Evaluate(date, event);
        };

        const auto entries = db.FindIf(predicate);
        for (const auto& entry : entries) {
          cout << entry << endl;
        }
        cout << "Found " << entries.size() << " entries" << endl;
    } else if (command == "Last") {
        try {
            cout << db.Last(ParseDate(is)) << endl;
        } catch (invalid_argument&) {
            cout << "No entries" << endl;
        }
    } else if (command.empty()) {
        continue;
    } else {
        throw logic_error("Unknown command: " + command);
    }
  }

  return 0;
}
