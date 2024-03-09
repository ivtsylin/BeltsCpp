# Задание по программированию «JSON Printer»

Вам нужно написать набор классов и функций, записывающих в заданный поток JSON-значение, причем всегда допустимое стандартом JSON.
- Функция PrintJsonString должна принимать ссылку на выходной поток (std::ostream), строку, которую нужно напечатать (std::string_view), и выводить экранированную строку в поток.
- Функция PrintJsonArray должна принимать ссылку на выходной поток и возвращать экземпляр класса, через который можно заполнить массив.
- Функция PrintJsonObject должна принимать ссылку на выходной поток и возвращать экземпляр класса, через который можно заполнить объект.

Методы вспомогательных классов:
- Number(int64_t) - выводит число
- String(std::string_view) - выводит экранированную строку
- Boolean(bool) - выводит булевое значение (true или false)
- Null() - выводит null.
- Key(std::string_view) - выводит ключ (только внутри объекта)
- BeginArray()/EndArray() - начинает/заканчивает вывод массива
- BeginObject()/EndObject() - начинает/заканчивает вывод объекта

При попытке записать невалидное JSON-значение через непрерывную цепочку вызовов должна возникать ошибка компиляции.

Примеры:
```cpp
PrintJsonString(std::cout, "Hello, \"world\"");
// "Hello, \"world\""

PrintJsonArray(std::cout)
  .Null()
  .String("Hello")
  .Number(123)
  .Boolean(false)
  .EndArray();  // явное завершение массива
// [null,"Hello",123,false]

PrintJsonArray(std::cout)
  .Null()
  .String("Hello")
  .Number(123)
  .Boolean(false);
// [null,"Hello",123,false]

PrintJsonArray(std::cout)
  .String("Hello")
  .BeginArray()
    .String("World");
// ["Hello",["World"]]

PrintJsonObject(std::cout)
  .Key("foo")
  .BeginArray()
     .String("Hello")
  .EndArray()
  .Key("foo")  // повторяющиеся ключи допускаются
  .BeginObject()
    .Key("foo");  // закрытие объекта в таком состоянии допишет null в качестве значения
// {"foo":["Hello"],"foo":{"foo":null}}

PrintJsonObject(std::cout)
  .String("foo");  // ошибка компиляции

PrintJsonObject(std::cout)
  .Key("foo")
  .Key("bar");  // ошибка компиляции
  
PrintJsonObject(std::cout)
  .EndArray();  // ошибка компиляции

PrintJsonArray(std::cout)
  .Key("foo")
  .BeginArray()
  .EndArray()
  .EndArray();  // ошибка компиляции
  
PrintJsonArray(std::cout)
  .EndArray()
  .BeginArray();  // ошибка компиляции  (JSON допускает только одно верхнеуровневое значение)
  
PrintJsonObject(std::cout)
  .EndObject()
  .BeginObject();  // ошибка компиляции  (JSON допускает только одно верхнеуровневое значение)
```

Код не обязан выводить корректный JSON, если создаваемые им временные объекты присваиваются переменным. Например, следующий код не обязан выводить корректный JSON:
```cpp
auto json = PrintJsonObject(std::cout).Key("x");
```

### Отступления от стандарта
- Для упрощения задачи мы немного упростили требования к JSON:
- Number должен принимать только int64_t, поддерживать вывод чисел с плавающей точкой не нужно
- Упрощенное экранирование строк: достаточно экранировать только " (как \") и \ (как \\)
