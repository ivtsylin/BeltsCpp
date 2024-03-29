# Задание по программированию ««Личный бюджет» под санитайзером»

Вам дано решение задачи «Личный бюджет, professional mobile version» из «Коричневого пояса». Оно содержит некоторое количество проблем, которые можно обнаружить с помощью санитайзеров и/или дебажных дефайнов: найдите их, исправьте и сдайте задачу.

Эта задача тренировочная, поэтому советуем использовать её как возможность лучше разобраться в санитайзерах и не сдавать готовое решение из предыдущего курса.

По техническим причинам тестирующая система плохо обнаруживает утечки памяти. Тем не менее, рекомендуем найти и исправить все утечки в данном решении.

# Условие исходной задачи #

Реализуйте систему ведения личного бюджета. Вам необходимо обрабатывать запросы следующих типов:
- **ComputeIncome *from to***: вычислить чистую прибыль за данный диапазон дат.
- **Earn *from to value***: учесть, что за указанный период (равномерно по дням) была заработана сумма ***value***.
- **Spend *from to value***: потратить указанную сумму равномерно за указанный диапазон дат. Чистая прибыль в запросах **ComputeIncome** вычисляется как разница заработанного (за вычетом налогов) и потраченного. При расчёте налога потраченные суммы не учитываются.
- **PayTax *from to percentage***: заплатить указанный налог в указанный диапазон. Гарантируется, что ***percentage***— целое число от 0 до 100. Это означает простое умножение всей прибыли в диапазоне на (1 − ***percentage*** / 100), независимо от того, отдавался ли уже налог за какой-то из указанных дней. Прибыль за эти дни, которая обнаружится позже, налогами из прошлого не облагается.

Примечания:
- Во всех диапазонах ***from to*** обе даты ***from*** и ***to*** включаются.

## Формат ввода ##

В первой строке вводится количество запросов **Q**, затем в описанном выше формате вводятся сами запросы, по одному на строке.

## Формат вывода ##

Для каждого запроса **ComputeIncome** в отдельной строке выведите вещественное число — чистую прибыль (прибыль за вычетом налогов) за указанный диапазон дат.

## Ограничения ##
- Количество запросов **Q** — натуральное число, не превышающее 100000.
- Все даты вводятся в формате YYYY-MM-DD. Даты корректны (с учётом високосных годов) и принадлежат годам с 2000 до 2100 включительно.
- ***value*** — положительные целые числа, не превышающие 1000000.
- 3,5 секунды на обработку всех запросов.

## Пример ##

Ввод 

```commandline
8
Earn 2000-01-02 2000-01-06 20
ComputeIncome 2000-01-01 2001-01-01
PayTax 2000-01-02 2000-01-03 13
ComputeIncome 2000-01-01 2001-01-01
Spend 2000-12-30 2001-01-02 14
ComputeIncome 2000-01-01 2001-01-01
PayTax 2000-12-30 2000-12-30 13
ComputeIncome 2000-01-01 2001-01-01
```

Вывод

```commandline
20
18.96
8.46
8.46
```
