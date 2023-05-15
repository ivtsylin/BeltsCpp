#include "test_runner.h"
#include <string>
#include <vector>

using namespace std;

class Translator {
public:
    void Add(string_view source, string_view target) {
        auto [iter_src, flag_src] = data.insert(string(source));
        auto [iter_tgt, flag_tgt] = data.insert(string(target));
        RusEng[string_view(*iter_src)] = string_view(*iter_tgt);
        EngRus[string_view(*iter_tgt)] = string_view(*iter_src);
    }
    string_view TranslateForward(string_view source) const {
        auto target = RusEng.find(source);
        if (target == RusEng.end()) {
            return {};
        }
        return target->second;
    }
    string_view TranslateBackward(string_view source) const {
        auto target = EngRus.find(source);
        if (target == EngRus.end()) {
            return {};
        }
        return target->second;
    }

private:
    map<string_view, string_view> RusEng;
    map<string_view, string_view> EngRus;
    set<string> data;
};

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}
