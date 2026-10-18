# Отбор для поступления на кафедру 1С

## Разработчик
Кириленко Михаил Юрьевич

## Решаемая задача
Задача 1 (алгоритмы), 18.10.2026

## Сборка и запуск
Сборка
```shell
mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && make -j$(nproc)
```

Запуск
```shell
./main
```

Пример работы:
```
Enter halls count: 
3
Enter daily workers cost: 
5

Setting data for 0th hall
Enter profit and tunnels count: 
0 2
Enter tunnels: 
1 0.5
2 0.1
Setting data for 1th hall
Enter profit and tunnels count: 
100 0
Setting data for 2th hall
Enter profit and tunnels count: 
1 0
Want to look at 0th hall
BROKEN OK
Total money earned: 1
```