# Задание по программированию «Телефонная книга»

В этой задаче мы разработаем класс PhoneBook, который выполняет подмножество задач адресной книги мобильного телефона. Наш класс будет решать три задачи:
- сериализация контактов в выходной поток (например, для сохранения её в памяти телефона или для отправки на сервер синхронизации контактов)
- десериализация контактов из входного потока
- поиск контактов, имя которых начинается на заданную строку — эта функция полезна, когда пользователь вводит какое-то имя, а адресная книга показывает ему все контакты, имя которых начинается с введённой строки

Контакт в нашей адресной книге может иметь следующие характеристики:
- Имя
- Дату рождения
- Номера телефона

При этом имя есть всегда (хотя может быть пустым), дня рождения может не быть, номеров телефона может быть ноль или больше. Таким образом, мы отрабатываем разные сценарии использования телефонной книги:
- записали день рождения человека, чтобы не забыть (имя и дата рождения есть, номеров телефона нет);
- храним несколько номеров телефона знакомого человека (имя есть, есть один или несколько номеров телефона, дата рождения не задана)
- собираем всю информацию о человеке в одном месте (есть имя, дата рождения, один или несколько номеров телефона)
- есть несколько номеров телефона с пустым именем и без даты рождения (например, пользователь тренирует память)

Таким образом, мы можем представить контакт в адресной книге в виде структуры:
```cpp
struct Date {
  int year, month, day;
};

struct Contact {
  std::string name;
  std::optional<Date> birthday;
  std::vector<std::string> phones;
};
```

Интерфейс класса PhoneBook будет выглядеть так:
```cpp
class PhoneBook {
public:
  explicit PhoneBook(std::vector<Contact> contacts);

  IteratorRange<???> FindByNamePrefix(std::string_view name_prefix) const;
  void SaveTo(std::ostream& output) const;
};

PhoneBook DeserializePhoneBook(std::istream& input);
```

Для сериализации/десериализации класс PhoneBook должен использовать protobuf. Proto-схему вы должны разработать сами с учётом следующих требований:
- proto-схема должна находиться в пространстве имён PhoneBookSerialize;
- в ней должно быть сообщение Contact, используемое для сериализации одного контакта;
- в ней должно быть сообщение ContactList, содержащее одно repeated-поле типа Contact.

Метод FindByNamePrefix должен возвращать диапазон всех контактов, имя которых начинается на строку name_prefix, отсортированный лексикографически. Если name_prefix — это пустая строка, метод FindByNamePrefix должен возвращать диапазон, содержащий все контакты адресной книги, в том числе те, у которых имя пустое. Метод FindByNamePrefix должен работать быстрее, чем заO(N), где N — количество контактов в адресной книге.

На проверку пришлите архив, содержащий файлы phone_book.h, phone_book.cpp и contact.proto, а также любые другие файлы, которые будут необходимы для успешной сборки вашего проекта.

## Как будет тестироваться ваш код ##

Ваш код будет тестироваться аналогично юнит-тестам, приведённым в файле main.cpp (см. ниже):
- мы проверим, что метод SaveTo сохраняет контакты в созданную вами proto-схему;
- мы проверим, что функция DeserializePhoneBook загружает адресную книгу из proto-сообщения ContactList;
- мы проверим, что метод FindByNamePrefix корректно работает для десериализованной адресной книги;
- мы проверим производительность метода FindByNamePrefix.

### Примечания ###
- Изучите юнит-тесты, чтобы понять, какие поля должны быть у protobuf-сообщений Contact и ContactList.
- Чтобы собрать свой проект, вы можете использовать CMake так же, как это показано в лекции. У вас могут возникнуть проблемы с использованием test_runner.h. Простым решением может быть добавить его в свой проект. Однако, если вы не хотите копировать файлы, вы можете просто добавить в CMakeLists.txt строки 

```console
find_path(TEST_RUNNER_PATH test_runner.h)
include_directories(${TEST_RUNNER_PATH})
```

Когда будете запускать cmake первый раз, добавьте -DTEST_RUNNER_PATH=<путь до папки с test_runner.h> в строку его запуска, например: cmake -DCMAKE_PREFIX_PATH=c:\dev\protobuf -DTEST_RUNNER_PATH=c:\users\ishfb\code\onlinecpp\grader\include -G "MinGW Makefiles" -S ..\correct .

  