# Задание по программированию «Интерпретатор языка Mython»

Итак, в предыдущей задаче мы создали лексический анализатор для языка mython. Теперь с его помощью нужно создать синтаксический анализатор, а затем собственно интерпретатор. Для создания синтаксического анализатора требуется определённая теоретическая подготовка, которая выходит за рамки «Чёрного пояса по C++», так что в этой задаче мы выдадим вам готовую реализацию нисходящего LL(1)-парсера языка Mython. (Если вы не поняли последние несколько слов, это нестрашно — вы сможете решить эту задачу без их понимания. Однако это говорит о том, что у вас нет тех самых теоретических знаний, о которых говорилось выше. Лучший источник, в котором их можно получить, — это Книга Дракона, канонический учебник по созданию компиляторов и трансляторов).

Этот парсер формирует абстрактное синтаксическое дерево, узлами которого являются объекты классов, унаследованных от абстрактного класса Statement:
```cpp
struct Statement {
  virtual ~Statement() = default;
  virtual ObjectHolder Execute(Runtime::Closure& closure) = 0;
};
```

Единственный метод интерфейса выполняет разобранную программу на языке mython.

Вам даны:
- файлы parse.h/cpp, содержащие синтаксический анализатор языка mython (файл parse.cpp использует lexer.h, который вы разработали в предыдущей задаче)
- файл statement.h, содержащий абстрактный интерфейс Statement, а также всех его потомков, которые используются в построении абстрактного синтаксического дерева
- файлы object_holder.h/cpp, содержащий специальный класс ObjectHolder, который используется интерпретатором mython'а для обращения к объектам mython-программы (числам, строкам, экземплярам классов, логическим константам и None); в нём же содержится определение типа Runtime::Closure, который используется для связывания имени переменной с конкретным значением
- файл object.h, который содержит классы, представляющие отдельные объекты mython-программы
- файл mython.cpp, содержащий функцию main
- наконец, несколько файлов с юнит-тестами, отражающими различные аспекты работы интерпретатора языка Mython

Ваша задача на основе предложенных файлов разработать интерпретатор, который считывает из stdin программу на языке Mython и выводит в stdout результат выполнения всех команд print.

На проверку пришлите архив, содержащий все необходимые файлы для сборки и запуска вашего интерпретатора.
