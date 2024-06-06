# Тестовое задание Биржа

## Системные ребования
- C++ 17
- Boost 1.40
- CMake 3.21

## Сборка проекта

```
git clone git@github.com:Gleor/ClientServerMatching.git
cd ClientServerMatching
mkdir build && cd build  
cmake ..  
cmake --build .
```
## Работа с проектом

Для начало работы нужно запустить сервер и клиент: `./Server` и `./Client`

После регистрации пользователя с помощью имени можно будет выбрать из опций меню:

1. Check Balance - ***проверить баланс***
2. Make trade application - ***создать торговую заявку***
3. Exit - ***выйти***

Если выбрана торговая заявка, то нужно указать:
- Сторону - Sell/Buy
- Количество валюты 
- Цену в рублях
