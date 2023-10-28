# Задание по программированию «Зоопарк»

Вы работаете с иерархией классов, описывающих животных:
```cpp
#include <string>

class Animal {
public:
  virtual std::string Voice() const {
    return "Not implemented yet";
  }
  virtual ~Animal() {
  }
};

class Tiger: public Animal {
  std::string Voice() const override {
    return "Rrrr";
  }
};

class Wolf: public Animal {
  std::string Voice() const override {
    return "Wooo";
  }
};

class Fox: public Animal {
  std::string Voice() const override {
    return "Tyaf";
  }
};
```

Вам нужно определить тип Zoo, представляющий собой набор различных животных, и написать две функции:
- Zoo CreateZoo(std::istream& in)
- void Process(const Zoo& zoo, std::ostream& out)

Функция CreateZoo должна читать слова из потока ввода. Если на вход поступают слова "Tiger", "Wolf" или "Fox", она должна помещать соответствующего зверя в зоопарк. В противном случае она должна прекратить чтение и сгенерировать исключение std::runtime_error.

Функция Process должна перебрать всех зверей в зоопарке в порядке создания и записать в поток вывода для каждого из них результат работы виртуальной функции Voice.

Один ваш друг написал решение этой задачи, но оно почему-то не работает. Исправьте его и сдайте в систему.

Замечания:
- Код классов из файла animals.h переписывать не надо, просто подключите заголовочный файл animals.h.
- Обратите внимание, что в нашей версии файла animals.h голоса зверей могут отличаться от того, что приведено в примере.
- Ваша программа должна создавать ровно столько объектов-зверей, сколько было передано во входном потоке в функцию CreateZoo.


