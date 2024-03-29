# Сортировка слиянием

Напишите шаблонную функцию MergeSort, принимающую два итератора шаблонного типа RandomIt и сортирующую заданный ими диапазон с помощью сортировки слиянием. Гарантируется, что:

итераторы типа RandomIt аналогичны по функциональности итераторам вектора и строки, то есть их можно сравнивать с помощью операторов <, <=, > и >=, а также вычитать и складывать с числами;
сортируемые объекты можно сравнивать с помощью оператора <.
```cpp
template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end);
```

# Часть 1. Реализация с разбиением на 2 части #

## Алгоритм ##

Классический алгоритм сортировки слиянием выглядит следующим образом:
- Если диапазон содержит меньше 2 элементов, выйти из функции.
- Создать вектор, содержащий все элементы текущего диапазона.
- Разбить вектор на две равные части. (В этой задаче гарантируется, что длина передаваемого диапазона является степенью двойки, так что вектор всегда можно разбить на две равные части.)
- Вызвать функцию MergeSort от каждой половины вектора.
- С помощью алгоритма merge слить отсортированные половины, записав полученный отсортированный диапазон вместо исходного.

Вы должны реализовать **именно этот алгоритм** и никакой другой: тестирующая система будет проверять, что вы выполняете с элементами именно эти действия.

## Подсказка ##
Чтобы создать вектор, содержащий все элементы текущего диапазона (п. 2 алгоритма), необходимо уметь по типу итератора узнавать тип элементов, на которые он указывает. Если итератор RandomIt принадлежит стандартному контейнеру (вектору, строке, множеству, словарю...), нижележащий тип можно получить с помощью выражения typename RandomIt::value_type. Таким образом, гарантируется, что создать вектор в п. 2 можно следующим образом:
```cpp
vector<typename RandomIt::value_type> elements(range_begin, range_end);
```

## Пример кода ##
```cpp
int main() {
  vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1};
  MergeSort(begin(v), end(v));
  for (int x : v) {
    cout << x << " ";
  }
  cout << endl;
  return 0;
}
```
Вывод
```commandline
0 1 4 4 4 6 6 7
```
