#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <set>

using namespace std;

class ReadingManager {
public:
    ReadingManager() : user_count(0) {
        for (int i = 0; i < book_fat; ++i) {
            accumulate_progress [ i ] = max_users;
        }
        for (int i = 0; i < max_users; ++i) {
            user_progress [ i ] = 0;
        }
    };
    void Read(int user_id, int page_count) {
        if (user_progress[user_id] == 0) {
            ++user_count;
        }
        for (int i = user_progress[user_id]; i < page_count; ++i) {
            accumulate_progress [ i ]--;
        }
        user_progress[user_id] = page_count;
    }
    
    double Cheer(int user_id) const {
        if (!user_progress[user_id]) {
            return 0;
        }
        if (user_count == 1) {
            return 1.;
        }
        return (accumulate_progress[user_progress[user_id] - 1] - max_users + user_count) * 1.0 / (user_count - 1);
    }

private:
    static const int book_fat = 1'000 + 1;
    static const int max_users = 100'000 + 1;
    int accumulate_progress[book_fat];
    int user_progress[max_users]; // Страница, на которой находится пользователь
    size_t user_count;
};


int main() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }

    return 0;
}
