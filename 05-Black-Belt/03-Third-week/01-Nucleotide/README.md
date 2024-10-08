# Задание по программированию «Нуклеотид: сжатие структуры данных»

## Постановка задачи ##
В одной из программ исследования генома человека используется следующая структура данных для хранения и передачи информации о найденных в геноме нуклеотидах:
```cpp
struct Nucleotide {
  char Symbol;
  size_t Position;
  int ChromosomeNum;
  int GeneNum;
  bool IsMarked;
  char ServiceInfo;
};
```

Где:
- Symbol - общеупотребимый буквенный код одного из четырёх нуклеотидов: A, T, G или С.
- Position - положение найденного нуклеотида в геноме. Может принимать значения от 0 до 3,3 млрд.
- ChromosomeNum - номер хромосомы, в которой найден нуклеотид. Может принимать значения от 1 до 46.
- GeneNum - номер гена, в котором найден нуклеотид. Может принимать значения от 1 до 25 тыс, а также специальное значение 0 - в случае если найденный нуклеотид лежит вне известных генов.
- IsMarked - флаг, является ли найденный нуклеотид помеченным ранее меткой.
- ServiceInfo - служебная информация о результатах поиска. Может быть любым символом.

Программа работает правильно, однако результаты поиска занимают слишком много памяти.

Вам требуется, пользуясь полученными на лекциями знаниями, написать структуру данных CompactNucleotide, которая позволяет хранить ту же информацию (без потерь) более компактно.

Для обеспечения совместимости необходимо написать реализации функций Compress() и Decompress().

Требования:
- Размер вашей структуры CompactNucleotide не должен превышать 8 байтов.
- В решении нельзя использовать директиву pragma pack.
