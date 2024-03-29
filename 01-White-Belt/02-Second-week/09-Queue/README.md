# Задание по программированию «Очередь»

Люди стоят в очереди, но никогда не уходят из её начала, зато могут приходить в конец и уходить оттуда. Более того, иногда некоторые люди могут прекращать и начинать беспокоиться из-за того, что очередь не продвигается.

Реализуйте обработку следующих операций над очередью:
- **WORRY *i***: пометить ***i***-го человека с начала очереди (в нумерации с 0) как беспокоящегося;
- **QUIET *i***: пометить ***i***-го человека как успокоившегося;
- **COME *k***: добавить ***k*** спокойных человек в конец очереди;
- **COME -*k***: убрать ***k*** человек из конца очереди;
- **WORRY_COUNT**: узнать количество беспокоящихся людей в очереди.

Изначально очередь пуста.

# Формат ввода #
Количество операций Q, затем описания операций.

Для каждой операции **WORRY *i*** и **QUIET *i*** гарантируется, что человек с номером ***i*** существует в очереди на момент операции.

Для каждой операции **COME -*k*** гарантируется, что ***k*** не больше текущего размера очереди.

# Формат вывода #
Для каждой операции **WORRY_COUNT** выведите одно целое число — количество беспокоящихся людей в очереди.

# Пример #

Ввод
```commandline
8
COME 5
WORRY 1
WORRY 4
COME -2
WORRY_COUNT
COME 3
WORRY 3
WORRY_COUNT
```

Вывод
```commandline
1
2
```
