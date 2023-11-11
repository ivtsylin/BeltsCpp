# Задание по программированию «Двойная диспетчеризация»

## Теория — статический и динамический тип указателя или ссылки на объект

Рассмотрим простой код:
```cpp
class Unit;
class Building;
class Tower;
class Fence;

struct GameObject {
  virtual ~GameObject() = default;

  virtual bool Collide(const GameObject& that) const = 0;
  virtual bool CollideWith(const Unit& that) const = 0;
  virtual bool CollideWith(const Building& that) const = 0;
  virtual bool CollideWith(const Tower& that) const = 0;
  virtual bool CollideWith(const Fence& that) const = 0;
};
```

Наша задача — реализовать функцию bool Collide(const GameObject& first, const GameObject& second), которая проверяет пересекаются ли два игровых объекта, то есть имеют ли они хотя бы одну общую точку.

Вам даны:
- файл game_object.h, содержащий интерфейс GameObject и объявление функции Collide
- файлы geo2d.h/cpp, содержащие библиотеку для работы с геометрическими фигурами на плоскости; библиотека состоит из классов Point, Rectangle, Circle и Segment и набора функций Collide, которые умеют определять пересечение для каждой пары фигур (занимательный факт — все функции реализованы в целых числах, без применения вычислений в типе double)
- файл collide.cpp, содержащий заготовку решения, а также юнит-тест, эмулирующий размещение нового объекта на игровой карте

Пришлите в тестирующую систему файл collide.cpp, который будет:
- подключать game_object.h
- содержать реализации классов Unit, Building, Tower и Fence (все они должны наследоваться от GameObject и реализовывать его интерфейс)
- содержать реализацию функции bool Collide(const GameObject& first, const GameObject& second), которая возвращает true, если объекты first и second имеют на плоскости хотя бы одну общую точку, и false в противном случае.

## Замечание
Один из способов реализовать функцию Collide — использовать dynamic_cast. Мы не освещали его в лекциях. Кроме того, этот способ приводит к появлению развесистого кода, подверженного возникновению ошибок. Мы хотим, чтобы в этой задаче вы хорошенько разобрались с понятиями статического и динамического типа, а также поработали с указателем this. Поэтому мы будем компилировать ваши решения с флагом -fno-rtti, который будет приводить к ошибкам компиляции в случае использования dynamic_cast.

