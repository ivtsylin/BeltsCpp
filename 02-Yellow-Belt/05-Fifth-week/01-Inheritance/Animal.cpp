#include <iostream>
#include <string>

using namespace std;

class Animal {
public:
    Animal (const string& init = "") : Name(init) {};
    const string Name;
};


class Dog : public Animal {
public:
    Dog (const string& init = "") : Animal(init) {};
    void Bark () {
        cout << Name << " barks: woof!" << endl;
    }
};
