
# Система интернет-магазина на C++17

## Оглавление

1. [Описание задачи](#описание-задачи)  
2. [Архитектура проекта](#архитектура-проекта)  
3. [Структура базы данных](#структура-базы-данных)  
4. [Умные указатели и STL](#умные-указатели-и-stl)  
5. [Система ролей и прав доступа](#система-ролей-и-прав-доступа)  
6. [Аудит и логирование](#аудит-и-логирование)  
7. [Примеры SQL запросов](#примеры-sql-запросов)  
8. [Сборка и запуск](#сборка-и-запуск)  
9. [Структура проекта](#структура-проекта)  
10. [Использованные техники](#использованные-техники)  
11. [Контакты](#контакты)  

---

## Описание задачи

Разработана система интернет-магазина на C++17 с использованием PostgreSQL и клиентской библиотеки libpqxx.  
Проект демонстрирует архитектуру прикладной системы с разделением на слои, корректной работой с памятью и полноценной реляционной базой данных.  

Основные цели:  
- Показать применение принципов ООП (наследование, полиморфизм, инкапсуляция, композиция, агрегация).  
- Продемонстрировать управление ресурсами через умные указатели (RAII-подход).  
- Использовать STL‑контейнеры и алгоритмы для работы с коллекциями доменных объектов.  
- Спроектировать и реализовать схему БД PostgreSQL с индексами, триггерами, функциями и процедурами.  
- Реализовать аудит действий пользователей и историю изменения статусов заказов.  

Функциональность системы:  
- Управление каталогом товаров (создание, изменение, удаление, просмотр).  
- Создание заказов, добавление в них товаров и изменение статусов.  
- Разные способы оплаты (паттерн Strategy).  
- Проверка возможности возврата товара (ограничение по сроку).  
- Ведение аудита всех ключевых операций (insert/update/delete/payment/return).  

---

## Архитектура проекта

### Основные классы

**Пользователи и роли:**  
- `User` — абстрактный базовый класс для всех типов пользователей.  
- `Admin` — администратор системы (полный доступ к товарам, заказам и аудиту).  
- `Manager` — менеджер (обработка заказов и платежей, контроль остатков).  
- `Customer` — покупатель (создание заказов, оплата, возвраты, просмотр своих заказов).  

**Доменные сущности:**  
- `Product` — товар (наименование, цена, остаток, категория).  
- `Order` — заказ (товары, сумма, статус, пользователь).  
- `OrderItem` — позиция заказа (товар, количество, цена).  
- `Payment` — платёж по заказу.  

**Оплата (паттерн Strategy):**  
- `PaymentStrategy` — абстрактная стратегия оплаты.  
- `CardPayment` — оплата банковской картой.  
- `WalletPayment` — оплата электронным кошельком.  
- `SBPPayment` — оплата через систему быстрых платежей (СБП).  

**Работа с БД:**  
- `DatabaseConnection<T>` — шаблонный класс-обёртка над соединением с PostgreSQL, инкапсулирующий подключение, транзакции и выполнение запросов.  

### Взаимоотношения классов

- `User` агрегирует коллекцию заказов (`std::vector<std::shared_ptr<Order>>`).  
- `Order` композитно владеет элементами `OrderItem` и объектом `Payment`.  
- `Payment` композитно владеет реализацией `PaymentStrategy`.  

Используются:  
- Наследование (`Admin`, `Manager`, `Customer` от `User`).  
- Полиморфизм (виртуальные методы для меню и операций с заказами, стратегии оплаты).  
- Композиция (жизненный цикл `OrderItem` и `Payment` привязан к `Order`).  
- Агрегация (пользователь ссылается на свои заказы через `shared_ptr`).  

---

## Структура базы данных

### Таблица users

```sql
CREATE TABLE users (
    user_id       SERIAL PRIMARY KEY,
    name          VARCHAR(100) NOT NULL,
    email         VARCHAR(100) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    role          VARCHAR(20) NOT NULL,  -- admin, manager, customer
    created_at    TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```
Хранит данные о пользователях и их ролях.

Таблица products
```sql
CREATE TABLE products (
    product_id        SERIAL PRIMARY KEY,
    name              VARCHAR(100) NOT NULL,
    description       TEXT,
    price             NUMERIC(10, 2) CHECK (price > 0),
    quantity_in_stock INT DEFAULT 0 CHECK (quantity_in_stock >= 0),
    category          VARCHAR(50),
    created_at        TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```
Каталог товаров с ценами, категориями и остатками.

Таблица orders
```sql
CREATE TABLE orders (
    order_id     SERIAL PRIMARY KEY,
    user_id      INT NOT NULL REFERENCES users(user_id),
    order_date   DATE NOT NULL,
    status       VARCHAR(20) DEFAULT 'pending',  -- pending, completed, cancelled, returned
    total_amount NUMERIC(10, 2) CHECK (total_amount >= 0),
    created_at   TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```
Основная сущность заказов, связана с пользователями.

Таблица order_items
```sql
CREATE TABLE order_items (
    order_item_id SERIAL PRIMARY KEY,
    order_id      INT NOT NULL REFERENCES orders(order_id),
    product_id    INT NOT NULL REFERENCES products(product_id),
    quantity      INT NOT NULL CHECK (quantity > 0),
    unit_price    NUMERIC(10, 2) NOT NULL,
    created_at    TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```
Содержит товары, входящие в каждый заказ.

Таблица order_status_history
```sql
CREATE TABLE order_status_history (
    history_id SERIAL PRIMARY KEY,
    order_id   INT NOT NULL REFERENCES orders(order_id),
    old_status VARCHAR(20),
    new_status VARCHAR(20),
    changed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    changed_by INT REFERENCES users(user_id)
);
```
Хранит историю изменения статусов заказов (для аудита и трассировки).

Таблица payments
```sql
CREATE TABLE payments (
    payment_id    SERIAL PRIMARY KEY,
    order_id      INT NOT NULL REFERENCES orders(order_id),
    payment_method VARCHAR(50),  -- card, wallet, sbp
    amount        NUMERIC(10, 2) NOT NULL,
    status        VARCHAR(20) DEFAULT 'pending',
    created_at    TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```
Отражает факты оплаты заказов.

Таблица audit_log
```sql
CREATE TABLE audit_log (
    log_id     SERIAL PRIMARY KEY,
    user_id    INT,
    operation  VARCHAR(50),   -- INSERT, UPDATE, DELETE, PAYMENT, RETURN
    table_name VARCHAR(50),
    record_id  INT,
    details    TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```
Используется для ведения общего аудита по системе.

Индексы
```sql
CREATE INDEX idx_orders_user_id              ON orders(user_id);
CREATE INDEX idx_orders_status               ON orders(status);
CREATE INDEX idx_order_items_order_id        ON order_items(order_id);
CREATE INDEX idx_order_items_product_id      ON order_items(product_id);
CREATE INDEX idx_order_status_history_order  ON order_status_history(order_id);
CREATE INDEX idx_audit_log_user_id           ON audit_log(user_id);
CREATE INDEX idx_audit_log_created_at        ON audit_log(created_at);
CREATE INDEX idx_payments_order_id           ON payments(order_id);
```
Умные указатели и STL
unique_ptr
Используется для эксклюзивного владения ресурсами:

```cpp
class Order {
private:
    std::unique_ptr<Payment> payment;
public:
    void setPayment(std::unique_ptr<Payment> p) {
        payment = std::move(p);
    }
};
```
Жизненный цикл Payment полностью контролируется объектом Order.

shared_ptr
Применяется, когда заказ разделяется между несколькими владельцами (например, пользователем и внутренними коллекциями):

```cpp
class User {
private:
    std::vector<std::shared_ptr<Order>> orders;
public:
    void addOrder(std::shared_ptr<Order> order) {
        orders.push_back(order);
    }
};
```
STL и алгоритмы
Используются стандартные контейнеры и алгоритмы:

std::vector — хранение коллекций заказов и товаров.

std::map / std::set — кэширование и хранение уникальных идентификаторов.

std::find_if — поиск заказов по условию (статус, идентификатор).

std::copy_if — фильтрация заказов (например, только completed).

std::accumulate — подсчёт выручки, количества проданных товаров и т.п.

Система ролей и прав доступа
Администратор (Admin)
Возможности:

Полный CRUD по товарам (Product).

Просмотр всех заказов и изменение их статусов.

Просмотр и выгрузка журнала аудита в CSV.

Управление пользователями (создание/удаление, назначение ролей).

Типичные методы:

addProduct, updateProduct, removeProduct.

viewAllOrders, approveOrder, rejectOrder.

viewAuditLog, exportAuditToCSV.

Менеджер (Manager)
Возможности:

Просмотр и обработка заказов в статусе pending.

Изменение статуса заказов, проведение платежей.

Контроль остатков и выявление товаров с низким количеством.

Ограничения:

Нет прав на создание/удаление пользователей.

Нет полного доступа к аудиту и настройке системы.

Покупатель (Customer)
Возможности:

Создание собственных заказов и добавление товаров.

Оплата заказов через выбранную стратегию (Card, Wallet, SBP).

Просмотр своих заказов и отслеживание статуса.

Возврат товаров при соблюдении условий (например, не позднее 30 дней).

Ограничения:

Нет доступа к чужим заказам.

Нет управления товарами и пользователями.

Аудит и логирование
Система поддерживает централизованный аудит всех ключевых операций.

Триггеры в БД
Для таблиц orders, products, payments и др. создаются триггеры, которые при вставке/обновлении/удалении записывают данные в audit_log.

Примеры действий, попадающих в аудит:

Создание заказа и изменение его статуса.

Создание/обновление/удаление товара.

Проведение платежей и возвратов.

Типичные запросы к аудиту
Получение аудита по пользователю.

История изменений статусов конкретного заказа.

Активность по дате/периоду.

Примеры SQL запросов
Информация о заказах
```sql
-- Все заказы пользователя
SELECT *
FROM orders
WHERE user_id = :user_id
ORDER BY order_date DESC;

-- Детали конкретного заказа
SELECT
    o.order_id,
    o.order_date,
    o.status,
    p.name       AS product_name,
    oi.quantity,
    oi.unit_price,
    oi.quantity * oi.unit_price AS line_total
FROM orders o
JOIN order_items oi ON o.order_id = oi.order_id
JOIN products p     ON oi.product_id = p.product_id
WHERE o.order_id = :order_id;
```
Популярные товары
```sql
SELECT
    p.product_id,
    p.name,
    COUNT(oi.order_item_id)         AS purchase_count,
    SUM(oi.quantity)                AS total_sold,
    SUM(oi.quantity * oi.unit_price) AS total_revenue
FROM order_items oi
JOIN products p ON oi.product_id = p.product_id
GROUP BY p.product_id, p.name
ORDER BY total_revenue DESC
LIMIT 10;
Статистика по пользователю
sql
SELECT
    u.name,
    COUNT(o.order_id)      AS order_count,
    SUM(o.total_amount)    AS total_spent,
    AVG(o.total_amount)    AS avg_order_value
FROM orders o
JOIN users u ON o.user_id = u.user_id
WHERE u.user_id = :user_id
GROUP BY u.name;
```
Сборка и запуск
Требования
C++17‑совместимый компилятор (GCC, Clang или MSVC).

CMake 3.10+ для конфигурации и сборки.

PostgreSQL 12+ и библиотека libpqxx.

Общая схема
Установить PostgreSQL и libpqxx.

Создать пользователя и базу данных для интернет‑магазина.

Последовательно выполнить SQL‑скрипты:

создание таблиц,

функции и процедуры,

триггеры,

тестовые данные.

Собрать проект через CMake.

Запустить бинарный файл (OnlineShop / OnlineShop.exe).

Структура проекта
text
Zadacha_examen/
├── include/
│   ├── DatabaseConnection.h
│   ├── User.h
│   ├── Admin.h
│   ├── Manager.h
│   ├── Customer.h
│   ├── Order.h
│   ├── OrderItem.h
│   ├── Product.h
│   ├── Payment.h
│   ├── PaymentStrategy.h
│   ├── CardPayment.h
│   ├── WalletPayment.h
│   ├── SBPPayment.h
│   ├── Menu.h
│   ├── Logger.h
│   └── ReportGenerator.h
│
├── src/
│   ├── main.cpp
│   ├── DatabaseConnection.cpp
│   ├── User.cpp
│   ├── Admin.cpp
│   ├── Manager.cpp
│   ├── Customer.cpp
│   ├── Order.cpp
│   ├── OrderItem.cpp
│   ├── Product.cpp
│   ├── Payment.cpp
│   ├── PaymentStrategy.cpp
│   ├── CardPayment.cpp
│   ├── WalletPayment.cpp
│   ├── SBPPayment.cpp
│   ├── Menu.cpp
│   ├── Logger.cpp
│   └── ReportGenerator.cpp
│
├── sql/
│   ├── tables.sql        # структура таблиц
│   ├── functions.sql     # функции и утилиты
│   ├── procedures.sql    # хранимые процедуры
│   ├── triggers.sql      # триггеры аудита и статусов
│   └── sample_data.sql   # тестовые данные
│
├── CMakeLists.txt
├── README.md
└── OnlineShop.exe (или бинарник после сборки)
Использованные техники
ООП: наследование, полиморфизм, инкапсуляция, композиция, агрегация.

Управление памятью: std::unique_ptr, std::shared_ptr, RAII.

STL: std::vector, std::map, std::set и алгоритмы find_if, copy_if, accumulate.

Паттерны проектирования: Strategy, частично MVC‑подход, RAII.

Работа с БД: транзакции, хранимые процедуры, функции, триггеры, индексы.

Контакты
Автор: Артём Талахов
GitHub: https://github.com/Artxmvl
Email: talakhovartem2007@gmail.com
