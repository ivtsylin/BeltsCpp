# Задание по программированию «Вектор своими руками — 2»

Теперь напишите реализацию функций Insert, Emplace и Erase для вектора. Мы просим написать версии функций Insert и Erase только для работы с одним элементом. Сам элемент задаётся константным итератором, а функции возвращают просто итератор.

Типы iterator и const_iterator определены как указатель и константный указатель на элемент. В реализации функций допускается любое количество вызовов move-конструкторов и move-операторов присваивания для элементов. В предположении, что move-операции не генерируют исключений, функции должны обеспечивать строгую гарантию безопасности. 