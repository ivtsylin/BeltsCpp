#include <string>
#include <iostream>

using namespace std;

class TimeServer {
public:
  string GetCurrentTime();
private:
  string LastFetchedTime = "00:00:00";
};

/*
string AskTimeServer() {
    //throw std::system_error(EFAULT, std::generic_category());
    throw runtime_error("");
    return "01:01:01";
}
 */

string TimeServer::GetCurrentTime() {
    try {
        LastFetchedTime = AskTimeServer();
    } catch (const system_error& ex) {
        // Do nothing
    }
    return LastFetchedTime;
}

int main() {
    // Меняя реализацию функции AskTimeServer, убедитесь, что это код работает корректно
    TimeServer ts;
    try {
        cout << ts.GetCurrentTime() << endl;
    } catch (exception& e) {
        cout << "Exception got: " << e.what() << endl;
    }
    return 0;
}
