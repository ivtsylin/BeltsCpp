#include <string>
#include <vector>
using namespace std;

// Макросы проходят ровно дважды при подстановке
// Поэтому необходимо, чтобы на первом шаге была подставлена константа __LINE__
// А на втором шаге произошла конкатенация

// Таким образом, на нулевом шаге у нас будет запись вида
// UNIQ_ID
// #define CREATE_UNIQ_ID2(LINE) _best_name_for_using_tmp ## LINE
// #define CREATE_UNIQ_ID(LINE) CREATE_UNIQ_ID2(LINE)
// #define UNIQ_ID CREATE_UNIQ_ID(__LINE__)

// На первом проходе будет выполнена первая подстановка и первый парсинг
// CREATE_UNIQ_ID2(10)
// #define CREATE_UNIQ_ID2(LINE) _best_name_for_using_tmp ## LINE

// На втором произойдет доподстановка
// _best_name_for_using_tmp10


#define CREATE_UNIQ_ID2(LINE) _best_name_for_using_tmp ## LINE
#define CREATE_UNIQ_ID(LINE) CREATE_UNIQ_ID2(LINE)
#define UNIQ_ID CREATE_UNIQ_ID(__LINE__)

int main() {
  int UNIQ_ID = 0;
  string UNIQ_ID = "hello";
  vector<string> UNIQ_ID = {"hello", "world"};
  vector<int> UNIQ_ID = {1, 2, 3, 4};
}
