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

---

## Описание задачи

### Цель проекта

Разработать полнофункциональную систему интернет-магазина на C++17 с использованием PostgreSQL, демонстрирующую:

- Принципы объектно-ориентированного программирования (ООП)
- Правильное управление памятью через умные указатели
- Работу со стандартной библиотекой (STL)
- Проектирование и работу с реляционной БД
- Написание хранимых процедур и триггеров
- Систему аудита и логирования

### Основные компоненты

**Классы:**
- `User` - базовый класс для пользователей
- `Admin` - администратор системы (полный доступ)
- `Manager` - менеджер (обработка заказов)
- `Customer` - покупатель (работа со своими заказами)
- `Order` - заказ с товарами и платежом
- `OrderItem` - элемент заказа
- `Product` - товар в каталоге
- `Payment` - платёж заказа
- `PaymentStrategy` - стратегия оплаты (паттерн Strategy)
- `CardPayment` - оплата банковской картой
- `WalletPayment` - электронный кошелёк
- `SBPPayment` - система быстрых платежей
- `DatabaseConnection<T>` - шаблонный класс для работы с БД

**Функциональность:**
- Управление товарами (добавление, удаление, изменение)
- Создание и управление заказами
- Три метода оплаты (паттерн Strategy)
- Возврат товаров с проверкой условий (не более 30 дней)
- История изменений статусов заказов
- Полное логирование всех операций
- Система управления доступом (роли и права)
- CSV отчёты по аудиту

---

## Архитектура проекта

### Иерархия классов

Вот полный текст README — копируй всё:

text
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

---

## Описание задачи

### Цель проекта

Разработать полнофункциональную систему интернет-магазина на C++17 с использованием PostgreSQL, демонстрирующую:

- Принципы объектно-ориентированного программирования (ООП)
- Правильное управление памятью через умные указатели
- Работу со стандартной библиотекой (STL)
- Проектирование и работу с реляционной БД
- Написание хранимых процедур и триггеров
- Систему аудита и логирования

### Основные компоненты

**Классы:**
- `User` - базовый класс для пользователей
- `Admin` - администратор системы (полный доступ)
- `Manager` - менеджер (обработка заказов)
- `Customer` - покупатель (работа со своими заказами)
- `Order` - заказ с товарами и платежом
- `OrderItem` - элемент заказа
- `Product` - товар в каталоге
- `Payment` - платёж заказа
- `PaymentStrategy` - стратегия оплаты (паттерн Strategy)
- `CardPayment` - оплата банковской картой
- `WalletPayment` - электронный кошелёк
- `SBPPayment` - система быстрых платежей
- `DatabaseConnection<T>` - шаблонный класс для работы с БД

**Функциональность:**
- Управление товарами (добавление, удаление, изменение)
- Создание и управление заказами
- Три метода оплаты (паттерн Strategy)
- Возврат товаров с проверкой условий (не более 30 дней)
- История изменений статусов заказов
- Полное логирование всех операций
- Система управления доступом (роли и права)
- CSV отчёты по аудиту

---

## Архитектура проекта

### Иерархия классов

User (базовый, чисто виртуальный)
├── Admin
├── Manager
└── Customer

PaymentStrategy (абстрактный)
├── CardPayment
├── WalletPayment
└── SBPPayment

Order (использует OrderItem и Payment)
├── vector<OrderItem>
└── unique_ptr<Payment>

text

### Отношения между классами

**Композиция:**
- Order владеет OrderItem (vector)
- Order владеет Payment (unique_ptr)
- Payment владеет PaymentStrategy (unique_ptr)

**Агрегация:**
- User содержит Order (shared_ptr)
- Admin работает с Product

### Применение паттернов ООП

#### 1. Наследование

```cpp
class User {  // базовый класс
public:
    virtual void displayMenu() = 0;  // чисто виртуальная функция
    virtual bool viewOrderStatus() = 0;
protected:
    int userId;
    string name;
    string email;
};

class Admin : public User {
public:
    void displayMenu() override;
    void addProduct(shared_ptr<Product> product);
    void updateProduct(int productId, double newPrice);
    void removeProduct(int productId);
    vector<shared_ptr<Order>> viewAllOrders();
};

class Manager : public User {
public:
    void displayMenu() override;
    vector<shared_ptr<Order>> viewPendingOrders();
    void updateOrderStatus(int orderId, const string& status);
    void processPayment(int orderId);
};

class Customer : public User {
public:
    void displayMenu() override;
    int createNewOrder(int customerId);
    void addToOrder(shared_ptr<Product> product, int quantity);
    void makePayment(int orderId, unique_ptr<PaymentStrategy> method);
    bool returnProduct(int orderId, int productId);
};
2. Полиморфизм
cpp
class PaymentStrategy {  // абстрактный
public:
    virtual bool processPayment(double amount) = 0;
    virtual ~PaymentStrategy() {}
};

class CardPayment : public PaymentStrategy {
public:
    bool processPayment(double amount) override;
private:
    string cardNumber;
    string expiryDate;
    string cvv;
};

class WalletPayment : public PaymentStrategy {
public:
    bool processPayment(double amount) override;
private:
    double balance;
    string walletId;
};

class SBPPayment : public PaymentStrategy {
public:
    bool processPayment(double amount) override;
private:
    string phoneNumber;
};
3. Композиция
cpp
class Order {
private:
    vector<OrderItem> items;           // композиция
    unique_ptr<Payment> payment;       // композиция
    int orderId;
    int userId;
    string orderDate;
    string status;
    double totalPrice;
};

class Payment {
private:
    unique_ptr<PaymentStrategy> strategy;  // композиция
    double amount;
    string paymentDate;
};
4. Агрегация
cpp
class User {
private:
    vector<shared_ptr<Order>> orders;  // агрегация
    int userId;
    string name;
    string email;
    string role;
};
Структура базы данных
Таблица: users
sql
CREATE TABLE users (
    user_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    role VARCHAR(20) NOT NULL,  -- admin, manager, customer
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
Назначение: Хранит информацию о пользователях системы с разными ролями.

Таблица: products
sql
CREATE TABLE products (
    product_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    description TEXT,
    price NUMERIC(10, 2) CHECK (price > 0),
    quantity_in_stock INT DEFAULT 0 CHECK (quantity_in_stock >= 0),
    category VARCHAR(50),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
Назначение: Каталог товаров с ценами и остатками на складе.

Таблица: orders
sql
CREATE TABLE orders (
    order_id SERIAL PRIMARY KEY,
    user_id INT NOT NULL REFERENCES users(user_id),
    order_date DATE NOT NULL,
    status VARCHAR(20) DEFAULT 'pending',  -- pending, completed, cancelled, returned
    total_amount NUMERIC(10, 2) CHECK (total_amount >= 0),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
Назначение: Основная таблица заказов.

Таблица: order_items
sql
CREATE TABLE order_items (
    order_item_id SERIAL PRIMARY KEY,
    order_id INT NOT NULL REFERENCES orders(order_id),
    product_id INT NOT NULL REFERENCES products(product_id),
    quantity INT NOT NULL CHECK (quantity > 0),
    unit_price NUMERIC(10, 2) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
Назначение: Товары, входящие в каждый заказ.

Таблица: order_status_history
sql
CREATE TABLE order_status_history (
    history_id SERIAL PRIMARY KEY,
    order_id INT NOT NULL REFERENCES orders(order_id),
    old_status VARCHAR(20),
    new_status VARCHAR(20),
    changed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    changed_by INT REFERENCES users(user_id)
);
Назначение: История смены статусов заказа для аудита.

Таблица: payments
sql
CREATE TABLE payments (
    payment_id SERIAL PRIMARY KEY,
    order_id INT NOT NULL REFERENCES orders(order_id),
    payment_method VARCHAR(50),  -- card, wallet, sbp
    amount NUMERIC(10, 2) NOT NULL,
    status VARCHAR(20) DEFAULT 'pending',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
Назначение: Информация о платежах по заказам.

Таблица: audit_log
sql
CREATE TABLE audit_log (
    log_id SERIAL PRIMARY KEY,
    user_id INT,
    operation VARCHAR(50),  -- INSERT, UPDATE, DELETE, PAYMENT, RETURN
    table_name VARCHAR(50),
    record_id INT,
    details TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
Назначение: Полный журнал всех операций в системе.

Индексы
sql
CREATE INDEX idx_orders_user_id ON orders(user_id);
CREATE INDEX idx_order_items_order_id ON order_items(order_id);
CREATE INDEX idx_order_items_product_id ON order_items(product_id);
CREATE INDEX idx_order_status_history_order_id ON order_status_history(order_id);
CREATE INDEX idx_audit_log_user_id ON audit_log(user_id);
CREATE INDEX idx_audit_log_created_at ON audit_log(created_at);
CREATE INDEX idx_orders_status ON orders(status);
CREATE INDEX idx_payments_order_id ON payments(order_id);
Умные указатели и STL
Использование unique_ptr
Правило: Используйте unique_ptr для эксклюзивного владения ресурсом.

cpp
// В классе Order
class Order {
private:
    unique_ptr<Payment> payment;
    
public:
    void setPayment(unique_ptr<Payment> p) {
        payment = move(p);  // передача владения
    }
    
    ~Order() {
        // автоматическое удаление payment
    }
};

// Использование
auto cardPayment = make_unique<CardPayment>("1234567890123456", "12/25", "123");
order->setPayment(move(cardPayment));
Использование shared_ptr
Правило: Используйте shared_ptr для совместного владения ресурсом.

cpp
// В классе User
class User {
private:
    vector<shared_ptr<Order>> orders;
    
public:
    void addOrder(shared_ptr<Order> order) {
        orders.push_back(order);
    }
};

// Использование
auto order = make_shared<Order>(order_id, user_id, order_date);
user->addOrder(order);
STL контейнеры
vector - динамический массив
cpp
vector<shared_ptr<Order>> orders;
orders.push_back(make_shared<Order>(101, 1, "2025-01-13"));

// Итерирование
for (const auto& order : orders) {
    cout << "Order: " << order->getOrderId() << endl;
}

// Размер
cout << "Total orders: " << orders.size() << endl;

// Очистка
orders.clear();
find_if - поиск с условием
cpp
// Найти первый заказ со статусом "completed"
auto it = find_if(orders.begin(), orders.end(),
    [](const shared_ptr<Order>& order) {
        return order->getStatus() == "completed";
    });

if (it != orders.end()) {
    cout << "Найден заказ: " << (*it)->getOrderId() << endl;
} else {
    cout << "Заказ не найден" << endl;
}

// Найти заказ по ID
auto orderIt = find_if(orders.begin(), orders.end(),
    [targetId = 101](const shared_ptr<Order>& o) {
        return o->getOrderId() == targetId;
    });
copy_if - фильтрация элементов
cpp
// Копировать все завершённые заказы
vector<shared_ptr<Order>> completed_orders;
copy_if(orders.begin(), orders.end(), 
    back_inserter(completed_orders),
    [](const shared_ptr<Order>& o) {
        return o->getStatus() == "completed";
    });

cout << "Completed orders: " << completed_orders.size() << endl;

// Копировать заказы на сумму > 5000
vector<shared_ptr<Order>> expensive_orders;
copy_if(orders.begin(), orders.end(),
    back_inserter(expensive_orders),
    [](const shared_ptr<Order>& o) {
        return o->getTotalAmount() > 5000.0;
    });
accumulate - агрегация данных
cpp
// Подсчитать общую сумму всех заказов
double total = accumulate(orders.begin(), orders.end(), 0.0,
    [](double sum, const shared_ptr<Order>& order) {
        return sum + order->getTotalAmount();
    });

cout << "Total revenue: " << total << " руб." << endl;

// Подсчитать количество товаров во всех заказах
int totalItems = accumulate(orders.begin(), orders.end(), 0,
    [](int count, const shared_ptr<Order>& order) {
        return count + order->getItemCount();
    });

cout << "Total items: " << totalItems << endl;
map - ассоциативный массив
cpp
map<int, shared_ptr<Product>> productCache;
productCache = make_shared<Product>(1, "Ноутбук", 50000, 5);[1]
productCache = make_shared<Product>(2, "Монитор", 15000, 10);[2]

// Проверка наличия ключа
if (productCache.find(1) != productCache.end()) {
    cout << productCache->getName() << endl;[1]
}

// Итерирование
for (const auto& [id, product] : productCache) {
    cout << id << ": " << product->getName() << endl;
}

// Размер
cout << "Cached products: " << productCache.size() << endl;
set - уникальные элементы
cpp
set<int> processedOrderIds;
processedOrderIds.insert(101);
processedOrderIds.insert(102);
processedOrderIds.insert(101);  // дубликат, не добавится

cout << "Processed: " << processedOrderIds.size() << endl;  // 2

// Проверка наличия
if (processedOrderIds.count(101)) {
    cout << "Order 101 already processed" << endl;
}
Система ролей и прав доступа
Три роли в системе
1. Администратор (Admin)
cpp
class Admin : public User {
public:
    // Управление товарами
    void addProduct(shared_ptr<Product> product);
    void removeProduct(int productId);
    void updateProduct(int productId, double newPrice, int newQuantity);
    
    // Просмотр всех заказов
    vector<shared_ptr<Order>> viewAllOrders();
    
    // Просмотр аудита
    void viewAuditLog();
    void exportAuditToCSV(const string& filename);
    
    // Одобрение заказов
    void approveOrder(int orderId);
    void rejectOrder(int orderId);
    
    // Управление пользователями
    void createUser(const string& name, const string& email, const string& role);
    void deleteUser(int userId);
};


2. Менеджер (Manager)
cpp
class Manager : public User {
public:
    // Просмотр ожидающих заказов
    vector<shared_ptr<Order>> viewPendingOrders();
    
    // Изменение статуса заказа
    void updateOrderStatus(int orderId, const string& newStatus);
    
    // Обработка платежа
    void processPayment(int orderId);
    
    // Отклонение заказа
    void rejectOrder(int orderId);
    
    // Управление товарами (только просмотр и количество)
    vector<shared_ptr<Product>> viewLowStockProducts();
    void notifyAboutLowStock(int productId);
};


3. Покупатель (Customer)
cpp
class Customer : public User {
public:
    // Создание заказа
    int createNewOrder(int customerId, const string& orderDate);
    
    // Добавление товара в заказ
    void addToOrder(shared_ptr<Product> product, int quantity);
    
    // Оплата заказа
    void makePayment(int orderId, unique_ptr<PaymentStrategy> paymentMethod);
    
    // Возврат товара
    bool returnProduct(int orderId, int productId);
    
    // Просмотр своих заказов
    vector<shared_ptr<Order>> viewMyOrders();
    
    // Отслеживание заказа
    void trackOrder(int orderId);
};
Права:

✅ Создание своих заказов

✅ Просмотр своих заказов

✅ Оплата заказов

✅ Возврат товаров (если не более 30 дней)

✅ Отслеживание статуса

❌ Просмотр чужих заказов

❌ Управление товарами

Проверка прав доступа
cpp
// Лямбда-функция для проверки прав
auto hasAccessToAllOrders = [](const string& role) -> bool {
    return role == "admin" || role == "manager";
};

// Использование в коде
if (hasAccessToAllOrders(user->getRole())) {
    // Показать все заказы
    vector<shared_ptr<Order>> allOrders = admin->viewAllOrders();
} else {
    // Показать только свои заказы
    vector<shared_ptr<Order>> myOrders = customer->viewMyOrders();
}

// Комплексная проверка прав
auto canPerformAction = [](const string& role, const string& action) -> bool {
    if (role == "admin") return true;  // admin может всё
    if (role == "manager" && action != "delete_user") return true;
    if (role == "customer" && (action == "create_order" || action == "view_own_orders")) {
        return true;
    }
    return false;
};

// Проверка перед выполнением операции
if (canPerformAction(user->getRole(), "update_product")) {
    admin->updateProduct(productId, newPrice);
} else {
    cout << "Access denied!" << endl;
}
Аудит и логирование
Автоматическое логирование через триггеры
Все изменения в основных таблицах автоматически логируются:

sql
-- Триггер для логирования операций создания заказа
CREATE TRIGGER log_order_create
AFTER INSERT ON orders
FOR EACH ROW
EXECUTE FUNCTION log_order_operation();

-- Триггер для логирования изменения статуса
CREATE TRIGGER log_order_status_change
AFTER UPDATE OF status ON orders
FOR EACH ROW
EXECUTE FUNCTION log_status_change();

-- Триггер для логирования удаления товара
CREATE TRIGGER log_product_delete
BEFORE DELETE ON products
FOR EACH ROW
EXECUTE FUNCTION log_product_operation();

-- Триггер для логирования изменения товара
CREATE TRIGGER log_product_update
AFTER UPDATE ON products
FOR EACH ROW
EXECUTE FUNCTION log_product_update_operation();
Функции для работы с аудитом
sql
-- Получить весь аудит конкретного пользователя
SELECT * FROM getAuditLogByUser(3);

-- Сгенерировать отчёт аудита заказов
SELECT * FROM generateOrderAuditReport();

-- Просмотреть историю статусов заказа
SELECT * FROM getOrderStatusHistory(101);

-- Проверить, можно ли вернуть товар (не более 30 дней)
SELECT canReturnOrder(101);

-- Получить дни с момента заказа
SELECT getDaysSinceOrder(101);

-- Получить всю активность по дате
SELECT * FROM getAuditByDateRange('2025-01-01', '2025-01-31');
Таблица audit_log
Поле	Тип	Описание
log_id	SERIAL	Уникальный ID записи
user_id	INT	ID пользователя, выполнившего операцию
operation	VARCHAR	Операция (INSERT, UPDATE, DELETE, PAYMENT, RETURN)
table_name	VARCHAR	Имя таблицы (orders, products, users, payments)
record_id	INT	ID изменённой записи
details	TEXT	Дополнительные детали (JSON или текст)
created_at	TIMESTAMP	Время операции
Примеры логирования
sql
-- Логирование создания заказа
INSERT INTO audit_log (user_id, operation, table_name, record_id, details)
VALUES (3, 'CREATE', 'orders', 101, '{"status":"pending","total":5000.00}');

-- Логирование изменения статуса
INSERT INTO audit_log (user_id, operation, table_name, record_id, details)
VALUES (2, 'UPDATE', 'orders', 101, '{"old_status":"pending","new_status":"completed"}');

-- Логирование возврата товара
INSERT INTO audit_log (user_id, operation, table_name, record_id, details)
VALUES (3, 'RETURN', 'order_items', 201, '{"product_id":5,"quantity":1,"reason":"defective"}');

-- Логирование платежа
INSERT INTO audit_log (user_id, operation, table_name, record_id, details)
VALUES (3, 'PAYMENT', 'payments', 1, '{"order_id":101,"method":"card","amount":5000.00,"status":"success"}');

-- Логирование добавления товара
INSERT INTO audit_log (user_id, operation, table_name, record_id, details)
VALUES (1, 'CREATE', 'products', 10, '{"name":"Клавиатура","price":3000.00,"stock":20}');
Примеры SQL запросов
Получение информации
sql
-- 1. Получить все заказы пользователя
SELECT * FROM orders WHERE user_id = 3 ORDER BY order_date DESC;

-- 2. Получить детали заказа с товарами и ценами
SELECT 
    o.order_id,
    o.order_date,
    o.status,
    p.name as product_name,
    oi.quantity,
    oi.unit_price,
    (oi.quantity * oi.unit_price) as line_total
FROM orders o
JOIN order_items oi ON o.order_id = oi.order_id
JOIN products p ON oi.product_id = p.product_id
WHERE o.order_id = 101
ORDER BY p.name;

-- 3. Получить популярные товары
SELECT 
    p.product_id,
    p.name,
    COUNT(oi.order_item_id) as purchase_count,
    SUM(oi.quantity) as total_sold,
    SUM(oi.quantity * oi.unit_price) as total_revenue
FROM order_items oi
JOIN products p ON oi.product_id = p.product_id
GROUP BY p.product_id, p.name
ORDER BY purchase_count DESC
LIMIT 10;

-- 4. Получить статус заказа с деталями
SELECT 
    o.order_id,
    u.name as customer_name,
    o.order_date,
    o.status,
    o.total_amount
FROM orders o
JOIN users u ON o.user_id = u.user_id
WHERE o.order_id = 101;

-- 5. Получить всё потраченное пользователем
SELECT 
    u.name,
    COUNT(o.order_id) as order_count,
    SUM(o.total_amount) as total_spent,
    AVG(o.total_amount) as avg_order_value
FROM orders o
JOIN users u ON o.user_id = u.user_id
WHERE u.user_id = 3
GROUP BY u.name;

-- 6. Получить количество заказов пользователя по статусам
SELECT 
    status,
    COUNT(*) as count
FROM orders
WHERE user_id = 3
GROUP BY status;
Модификация данных
sql
-- 1. Создать новый заказ (функция или хранимая процедура)
INSERT INTO orders (user_id, order_date, status, total_amount)
VALUES (3, CURRENT_DATE, 'pending', 0.00);

-- 2. Добавить товар в заказ
INSERT INTO order_items (order_id, product_id, quantity, unit_price)
SELECT 101, 1, 2, price FROM products WHERE product_id = 1;

-- 3. Обновить статус заказа
UPDATE orders SET status = 'completed' WHERE order_id = 101;

-- 4. Обновить историю статусов
INSERT INTO order_status_history (order_id, old_status, new_status, changed_by)
VALUES (101, 'pending', 'completed', 2);

-- 5. Обновить количество товара на складе после заказа
UPDATE products 
SET quantity_in_stock = quantity_in_stock - 2
WHERE product_id = 1;

-- 6. Записать платёж
INSERT INTO payments (order_id, payment_method, amount, status)
VALUES (101, 'card', 5000.00, 'success');

-- 7. Отменить заказ
UPDATE orders SET status = 'cancelled' WHERE order_id = 101;

-- 8. Удалить товар из заказа
DELETE FROM order_items WHERE order_id = 101 AND product_id = 1;
Сложные запросы
sql
-- 1. Заказы с рассчитанной суммой и количеством товаров
SELECT 
    o.order_id,
    o.order_date,
    u.name as customer,
    COUNT(DISTINCT oi.product_id) as unique_products,
    SUM(oi.quantity) as total_items,
    SUM(oi.quantity * oi.unit_price) as total_price,
    o.status
FROM orders o
JOIN users u ON o.user_id = u.user_id
LEFT JOIN order_items oi ON o.order_id = oi.order_id
GROUP BY o.order_id, o.order_date, u.name, o.status
ORDER BY o.order_date DESC;

-- 2. Доход по менеджерам (на основе обновлений заказов)
SELECT 
    u.name as manager,
    COUNT(DISTINCT a.record_id) as orders_processed,
    SUM(CASE WHEN o.status = 'completed' THEN o.total_amount ELSE 0 END) as completed_revenue,
    COUNT(DISTINCT CASE WHEN a.operation = 'UPDATE' THEN a.record_id END) as updates_made,
    MAX(a.created_at) as last_activity
FROM audit_log a
JOIN users u ON a.user_id = u.user_id
JOIN orders o ON a.record_id = o.order_id
WHERE u.role = 'manager' AND a.table_name = 'orders'
GROUP BY u.name
ORDER BY completed_revenue DESC;

-- 3. Товары, требующие пополнения (менее 5 штук)
SELECT 
    product_id,
    name,
    quantity_in_stock,
    price,
    (price * (10 - quantity_in_stock)) as restock_cost
FROM products 
WHERE quantity_in_stock < 5
ORDER BY quantity_in_stock ASC;

-- 4. История платежей по дате с деталями
SELECT 
    p.payment_id,
    o.order_id,
    u.name as customer,
    p.payment_method,
    p.amount,
    p.status,
    p.created_at
FROM payments p
JOIN orders o ON p.order_id = o.order_id
JOIN users u ON o.user_id = u.user_id
ORDER BY p.created_at DESC
LIMIT 20;

-- 5. Процент возвратов по товарам
SELECT 
    p.product_id,
    p.name,
    COUNT(DISTINCT o.order_id) as total_orders,
    COUNT(DISTINCT CASE WHEN o.status = 'returned' THEN o.order_id END) as returned_orders,
    ROUND(100.0 * COUNT(DISTINCT CASE WHEN o.status = 'returned' THEN o.order_id END) / 
        COUNT(DISTINCT o.order_id), 2) as return_percent
FROM orders o
JOIN order_items oi ON o.order_id = oi.order_id
JOIN products p ON oi.product_id = p.product_id
GROUP BY p.product_id, p.name
HAVING COUNT(DISTINCT o.order_id) > 0
ORDER BY return_percent DESC;




Контакты и поддержка
Автор: Артём Талахов
GitHub: https://github.com/Artxmvl
Email: talakhovartem2007@gmail.com
