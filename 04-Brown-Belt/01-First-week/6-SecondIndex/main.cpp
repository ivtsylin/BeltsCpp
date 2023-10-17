#include "test_runner.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <map>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;
};

//ostream& operator<<(ostream& os, Record rec) {
//    os << rec.id << " " << rec.title << " " << rec.user << " " << rec.timestamp << " " << rec.karma;
//    return os;
//}

// Реализуйте этот класс
class Database {
    map<string, Record> data;
    using DataIter = typename map<string, Record>::iterator;
    
    multimap<string, DataIter> ind_user;
    multimap<int, DataIter> ind_timestamp;
    multimap<int, DataIter> ind_karma;
    
    struct IndexTable {
        multimap<string, DataIter>::iterator it_user;
        multimap<int, DataIter>::iterator it_timestamp;
        multimap<int, DataIter>::iterator it_karma;
    };
    
    unordered_map<string, IndexTable> indeces;
    
public:
    bool Put(const Record& record) {
        auto insertation = data.emplace(record.id, record);
        if (insertation.second) {
            
            indeces.emplace(record.id,
                            IndexTable{
                                ind_user.emplace(record.user, insertation.first),
                                ind_timestamp.emplace(record.timestamp, insertation.first),
                                ind_karma.emplace(record.karma, insertation.first)});
            return true;
        }
        return false;
    }
    
    const Record* GetById(const string& id) const {
        auto iter = data.find(id);
        return iter != data.end() ? &(iter->second) : nullptr;
    }
    
    bool Erase(const string& id) {
        auto iter = data.find(id);
        if (iter != data.end()) {
            ind_user.erase(indeces[id].it_user);
            ind_timestamp.erase(indeces[id].it_timestamp);
            ind_karma.erase(indeces[id].it_karma);
            indeces.erase(id);
            data.erase(iter);
            return true;
        }
        return false;
    }

    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const {
        auto low_it = ind_timestamp.lower_bound(low);
        auto upp_it = ind_timestamp.upper_bound(high);
        for (auto it = low_it; it != upp_it; ++it)
            if(!callback(it->second->second))
                return;
    }

    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const {
        auto low_it = ind_karma.lower_bound(low);
        auto upp_it = ind_karma.upper_bound(high);
        for (auto it = low_it; it != upp_it; ++it)
            if(!callback(it->second->second))
                return;
    }

    template <typename Callback>
    void AllByUser(const string& user, Callback callback) const {
        auto low_it = ind_user.lower_bound(user);
        auto upp_it = ind_user.upper_bound(user);
        for (auto it = low_it; it != upp_it; ++it)
            if(!callback(it->second->second))
                return;
    }
};

void TestRangeBoundaries() {
  const int good_karma = 1000;
  const int bad_karma = -10;

  Database db;
  db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
  db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestSameUser() {
  Database db;
  db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
  db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

  int count = 0;
  db.AllByUser("master", [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement() {
  const string final_body = "Feeling sad";

  Database db;
    //cout << db.data << endl;
  db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
    //cout << db.data << endl;
  db.Erase("id");
    //cout << db.data << endl;
  db.Put({"id", final_body, "not-master", 1536107260, -10});
    //cout << db.data << endl;

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  return 0;
}
