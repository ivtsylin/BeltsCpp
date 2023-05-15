#include <cstdint>
#include <iostream>
#include <map>
#include <queue>

using namespace std;


class HotelManager {
public:
    void Book(const int64_t &time, const string& hotel_name, int client_id, int room_count) {
        current_time_ = time;
        hotel_info [ hotel_name ].Book(time, client_id, room_count);
    }
    int ComputeClientCount(const string& hotel_name) {
        if (!hotel_info.count(hotel_name))
            return 0;
        return hotel_info.at(hotel_name).get_users_count(current_time_);
    }
    int ComputeRoomCount(const string& hotel_name) {
        if (!hotel_info.count(hotel_name))
            return 0;
        return hotel_info.at(hotel_name).get_rooms_count(current_time_);
    }

private:
    class hotel_stat {
        struct booking_req {
            int64_t time;
            int client_id;
            int room_count;
        };
    public:
        hotel_stat() : rooms_count(0) {};
        void Book (const int64_t &time, int client_id, int room_count) {
            booking_req_.push({time, client_id, room_count});
            rooms_count += room_count;
            ++user_counts [ client_id ];
            adjust(time);
        }
        int get_rooms_count(const int64_t &time) {
            adjust(time);
            return rooms_count;
        }
        int get_users_count(const int64_t &time) {
            adjust(time);
            return user_counts.size();
        }
    private:
        static const int TIME_WINDOW_SIZE = 86400;
        void adjust (const int64_t &cur_time__) {
            while (!booking_req_.empty()
                       && booking_req_.front().time <= cur_time__ - TIME_WINDOW_SIZE) {
                rooms_count -= booking_req_.front().room_count;
                --user_counts[booking_req_.front().client_id];
                if (!user_counts[booking_req_.front().client_id]) {
                    user_counts.erase(booking_req_.front().client_id);
                }
                booking_req_.pop();
            }
        }
        queue<booking_req> booking_req_;
        int rooms_count;
        map<int, int> user_counts;
    };
    int64_t current_time_ = 0;
    map<string, hotel_stat> hotel_info;
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    HotelManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        
        if (query_type == "BOOK") {
            int64_t time;
            cin >> time;
            string hotel_name;
            cin >> hotel_name;
            int client_id, room_count;
            cin >> client_id >> room_count;
            manager.Book(time, hotel_name, client_id, room_count);
        } else {
            string hotel_name;
            cin >> hotel_name;
            if (query_type == "CLIENTS") {
                cout << manager.ComputeClientCount(hotel_name) << "\n";
            } else if (query_type == "ROOMS") {
                cout << manager.ComputeRoomCount(hotel_name) << "\n";
            }
        }
    }
    return 0;
}
