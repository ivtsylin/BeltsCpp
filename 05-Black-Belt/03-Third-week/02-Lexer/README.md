# Задание по программированию «Лексический анализатор языка Mython»

В этой задаче мы с вами разработаем лексический анализатор для разбора программы на языке mython. Давайте рассмотрим пример программы, которая проверяет, являются ли два числа взаимнопростыми:
```py
class GCD:
  def calc(a, b):
    if a == 0 or b == 0:
      return a + b
    else:
      if a < b:
        return self.calc(b, a)
      else:
        return self.calc(b, a - b)

  def is_coprime(a, b):
    return self.calc(a, b) == 1

coprime = False

gcd = None
gcd = GCD()

x = 4
y = 13
coprime = gcd.is_coprime(x, y)
if coprime:
  print x, 'and', y, 'are coprime'
else:
  print x, 'and', y, 'are not coprime'
```

Глядя на неё, мы можем выделить следующие лексемы языка mython:
- ключевые слова: class, return, if, else, def, print, or, None, а также не представленные в программе логические операции and, not и логические константы True и False
- целые числа
- идентификаторы (GCD, calc, coprime и т.д.)
- строковые константы ('are coprime')
- операторы сравнения, состоящие из нескольких символов (==, >=, <=, !=)
- отдельные символы ('=', '.', ',', '(', '+', '<' и т.д.)
- и, наконец, специальные лексемы, которых явно не видно в программе: «перевод строки», «увеличение отступа», «уменьшение отступа», «конец файла»

Среди лексем выделяются те, которые имеют связанное с ними значение (например, с лексемой «целое число» связано конкретное число), и те, которые не имеют никакого связанного значения (например, «перевод строки» или ключевое слово «return»). При этом лексема всегда имеет один из перечисленных выше типов. Таким образом, для представления лексемы в программе на C++ удобно использовать тип std::variant:
```cpp
namespace TokenType {
  struct Number { // Лексема «целое число»
    int value;
  };

  struct Id {     // Лексема «идентификатор»
    std::string value;
  };

  struct Class{};    // Лексема «class»
  struct Return{};   // Лексема «return»
  struct Indent {};  // Лексема «увеличение отступа»
  struct Dedent {};  // Лексема «уменьшение отступа»
  struct Eof {};     // Лексема «конец файла»
  ...
}

using Token = std::variant<
  TokenType::Number,
  TokenType::Id,
  TokenType::Class,
  TokenType::Return,
  TokenType::Indent,
  TokenType::Dedent,
  ...
  TokenType::Eof
>;
```

Такое представление удобно тем, что мы имеем возможность обратиться к полю value только той лексемы, для которой это имеет смысл.

В этой задаче мы с вами разработаем класс Lexer, который принимает на вход поток ввода (std::istream) и выдаёт последовательность лексем программы на языке mython. Класс Lexer имеет следующий интерфейс:
```cpp
class Lexer {
public:
  explicit Lexer(std::istream& input);

  const Token& CurrentToken() const;
  Token NextToken();

  template <typename T>
  const T& Expect() const;

  template <typename T, typename U>
  void Expect(const U& value) const;

  template <typename T>
  const T& ExpectNext();

  template <typename T, typename U>
  void ExpectNext(const U& value);
};
```

Метод CurrentToken возвращает ссылку на последнюю лексему, считанную Lexer'ом.

Метод NextToken считывает и возвращает очередную лексему из входного потока. Если входной поток пуст, метод должен вернуть лексему TokenType::Eof. После выполнения метода NextToken метод CurrentToken должен возвращать ссылку на только что считанную лексему. Пример:
```cpp
void LexerExample() {
  istringstream input("class Point:");
  Lexer lexer(input);
  ASSERT(std::hold_alternative<TokenType::Class>(lexer.CurrentToken()));

  auto token = lexer.NextToken();
  ASSERT(std::hold_alternative<TokenType::Id>(token));
  ASSERT(std::hold_alternative<TokenType::Id>(lexer.CurrentToken()));
}
```

В процессе синтаксического разбора мы часто знаем, какая лексема должна быть следующей (например, мы знаем, что после имени класса идёт символ двоеточия). Если нам встречается какая-то другая лексема, значит, нам на вход подали некорретную программу, и мы должны сообщить об ошибке разбора. Это удобно делать с помощью методов Expect*:

Шаблонный метод Expect\<T\>() проверяет, что CurrentToken() имеет тип T. Если это не так, он бросает исключение LexerError.

Шаблонный метод Expect\<T\>(const U& value) не только проверяет, что CurrentToken() имеет тип T, но и что значение текущей лексемы равно value.

Наконец, методы ExpectNext сначала вызывают NextToken, а затем — соответствующий метод Expect.

Например, разбор объявления класса без наследования может выглядеть так:
```cpp
void ParseClassDefinition(Lexer& lexer) {
  lexer.Expect<TokenType::Class>();
  auto name = lexer.ExpectNext<TokenType::Id>().value;
  lexer.ExpectNext<TokenType::Char>(':');
  ...
}
```

Вам дан файл lexer.h, а также файл lexer_test.cpp с неполным набором юнит-тестов. Пришлите на проверку архив, состоящий из файлов lexer.h и lexer.cpp, с реализацией класса Lexer. В следующей задаче мы воспользуемся классом Lexer для создания интерпретатора языка mython. 
