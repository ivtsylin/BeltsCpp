#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person {
public:
    Person (const string& name, const string& role) : Role(role), Name(name) {};
    virtual void Walk(const string &destination) const = 0;
    string GetName() const {
        return Name;
    }
    string GetRole() const {
        return Role;
    }
private:
    const string Name;
    const string Role;
};

class Student : public Person {
public:
    Student(const string &name, const string &favouriteSong) : Person(name, "Student"), FavouriteSong(favouriteSong) {}
    void Learn() const {
        cout << "Student: " << GetName() << " learns" << endl;
    }
    void Walk(const string &destination) const override {
        cout << "Student: " << GetName() << " walks to: " << destination << endl;
        cout << "Student: " << GetName() << " sings a song: " << FavouriteSong << endl;
    }
    void SingSong() const {
        cout << "Student: " << GetName() << " sings a song: " << FavouriteSong << endl;
    }
private:
    const string FavouriteSong;
};

class Teacher : public Person {
public:
    Teacher(const string &name, const string &subject) : Person(name, "Teacher"), Subject(subject) {}
    void Teach() const {
        cout << "Teacher: " << GetName() << " teaches: " << Subject << endl;
    }
    void Walk(const string &destination) const override {
        cout << "Teacher: " << GetName() << " walks to: " << destination << endl;
    }
private:
    const string Subject;
};

class Policeman : public Person {
public:
    Policeman(const string &name) : Person(name, "Policeman") {};
    void Check (const Person &p) const {
        cout << "Policeman: " << GetName() << " checks " << p.GetRole() << ". " << p.GetRole() << "'s name is: " << p.GetName() << endl;
    }
    void Walk(const string &destination) const override {
        cout << "Policeman: " << GetName() << " walks to: " << destination << endl;
    }
private:
};

void VisitPlaces(const Person &person, const vector<string> &places) {
    for (const auto &p : places) {
        person.Walk(p);
    }
}

int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");
    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});
    return 0;
}
