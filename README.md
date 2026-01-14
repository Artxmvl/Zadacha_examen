Система интернет-магазина на C++
Оглавление
Описание задачи
Архитектура проекта
Структура базы данных
Умные указатели и STL
Система ролей и прав доступа
Аудит и логирование
Примеры SQL запросов
Сборка и запуск
Описание задачи
Цель проекта
Разработать полнофункциональную систему интернет-магазина на C++17 с использованием PostgreSQL, демонстрирующую:

Принципы объектно-ориентированного программирования (ООП)
Правильное управление памятью через умные указатели
Работу со стандартной библиотекой (STL)
Проектирование и работу с реляционной БД
Написание хранимых процедур и триггеров
Основные компоненты
Классы:

User - базовый класс для пользователей
Admin - администратор системы (полный доступ)
Manager - менеджер (обработка заказов)
Customer - покупатель (работа со своими заказами)
Order - заказ с товарами и платежом
OrderItem - элемент заказа
Product - товар в каталоге
Payment - платёж заказа
PaymentStrategy - стратегия оплаты
CardPayment - оплата картой
EWalletPayment - электронный кошелёк
SBPPayment - система быстрых платежей
DatabaseConnection<T> - шаблонный класс для работы с БД
Функциональность:

Управление товарами (добавление, удаление, изменение)
Создание и управление заказами
Три метода оплаты (паттерн Strategy)
Возврат товаров с проверкой условий (не более 30 дней)
История изменений статусов заказов
Полное логирование всех операций
Система управления доступом (роли и права)
CSV отчёты по аудиту
Архитектура проекта
Иерархия классов
User (базовый, чисто виртуальный)
├── Admin
├── Manager
└── Customer

PaymentStrategy (абстрактный)
├── CardPayment
├── EWalletPayment
└── SBPPayment

Order (использует OrderItem и Payment)
├── vector<OrderItem>
└── unique_ptr<Payment>
Отношения между классами
Композиция:

Order владеет OrderItem (vector)
Order владеет Payment (unique_ptr)
Payment владеет PaymentStrategy (unique_ptr)
Агрегация:

User содержит Order (shared_ptr)
Admin работает с Product
Применение паттернов ООП
1. Наследование
class User {  // базовый класс
public:
    virtual void displayMenu() = 0;  // чисто виртуальная функция
    virtual bool viewOrderStatus() = 0;
protected:
    int userId;
    string name;
};

class Admin : public User {
public:
    void displayMenu() override;
    void addProduct(shared_ptr<Product> product);
};
2. Полиморфизм
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
};
3. Композиция
class Order {
private:
    vector<OrderItem> items;           // композиция
    unique_ptr<Payment> payment;       // композиция
};

class Payment {
private:
    unique_ptr<PaymentStrategy> strategy;  // композиция
};
4. Агрегация
class User {
private:
    vector<shared_ptr<Order>> orders;  // агрегация
};
Структура базы данных
Таблица: users
CREATE TABLE users (
    user_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    role VARCHAR(20) NOT NULL  -- admin, manager, customer
);
Назначение: Хранит информацию о пользователях системы с разными ролями.

Таблица: products
CREATE TABLE products (
    product_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    price NUMERIC(10, 2) CHECK (price > 0),
    quantity_in_stock INT DEFAULT 0
);
Назначение: Каталог товаров с ценами и остатками.

Таблица: orders
CREATE TABLE orders (
    order_id SERIAL PRIMARY KEY,
    user_id INT NOT NULL REFERENCES users(user_id),
    order_date DATE NOT NULL,
    status VARCHAR(20) DEFAULT 'pending',  -- pending, completed, cancelled, returned
    total_amount NUMERIC(10, 2) CHECK (total_amount >= 0)
);
Назначение: Основная таблица заказов.

Таблица: order_items
CREATE TABLE order_items (
    order_item_id SERIAL PRIMARY KEY,
    order_id INT NOT NULL REFERENCES orders(order_id),
    product_id INT NOT NULL REFERENCES products(product_id),
    quantity INT NOT NULL,
    unit_price NUMERIC(10, 2) NOT NULL
);
Назначение: Товары, входящие в каждый заказ.

Таблица: order_status_history
CREATE TABLE order_status_history (
    history_id SERIAL PRIMARY KEY,
    order_id INT NOT NULL REFERENCES orders(order_id),
    old_status VARCHAR(20),
    new_status VARCHAR(20),
    changed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
Назначение: История смены статусов заказа для аудита.

Таблица: audit_log
CREATE TABLE audit_log (
    log_id SERIAL PRIMARY KEY,
    user_id INT,
    operation VARCHAR(50),
    table_name VARCHAR(50),
    record_id INT,
    details TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
Назначение: Полный журнал всех операций в системе.

Индексы
CREATE INDEX idx_orders_user_id ON orders(user_id);
CREATE INDEX idx_order_items_order_id ON order_items(order_id);
CREATE INDEX idx_order_items_product_id ON order_items(product_id);
CREATE INDEX idx_order_status_history_order_id ON order_status_history(order_id);
CREATE INDEX idx_audit_log_user_id ON audit_log(user_id);
CREATE INDEX idx_audit_log_created_at ON audit_log(created_at);
CREATE INDEX idx_orders_status ON orders(status);
Умные указатели и STL
Использование unique_ptr
Правило: Используйте unique_ptr для эксклюзивного владения ресурсом.

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
vector<shared_ptr<Order>> orders;
orders.push_back(make_shared<Order>(101, 1, "2025-01-13"));

// Итерирование
for (const auto& order : orders) {
    cout << "Order: " << order->getOrderId() << endl;
}
find_if - поиск с условием
// Найти первый заказ со статусом "completed"
auto it = find_if(orders.begin(), orders.end(),
    [](const shared_ptr<Order>& order) {
        return order->getStatus() == "completed";
    });

if (it != orders.end()) {
    cout << "Найден заказ: " << (*it)->getOrderId() << endl;
}
copy_if - фильтрация элементов
// Копировать все завершённые заказы
vector<shared_ptr<Order>> completed_orders;
copy_if(orders.begin(), orders.end(), 
    back_inserter(completed_orders),
    [](const shared_ptr<Order>& o) {
        return o->getStatus() == "completed";
    });
accumulate - агрегация данных
// Подсчитать общую сумму всех заказов
double total = accumulate(orders.begin(), orders.end(), 0.0,
    [](double sum, const shared_ptr<Order>& order) {
        return sum + order->getOrderTotal();
    });

cout << "Итоговая сумма: " << total << " руб." << endl;
map - ассоциативный массив
map<int, shared_ptr<Product>> productCache;
productCache[1] = make_shared<Product>(1, "Ноутбук", 50000, 5);

if (productCache.find(1) != productCache.end()) {
    cout << productCache[1]->getName() << endl;
}
Система ролей и прав доступа
Три роли в системе
1. Администратор (Admin)
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
    
    // Одобрение заказов
    void approveOrder(int orderId);
};
Права:

✅ Просмотр всех заказов
✅ Управление товарами (CRUD)
✅ Просмотр аудита
✅ Одобрение заказов
✅ Создание и удаление пользователей
2. Менеджер (Manager)
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
};
Права:

✅ Просмотр ожидающих заказов
✅ Изменение статуса заказа
✅ Обработка платежей
✅ Отклонение заказов
❌ Управление товарами
❌ Просмотр аудита других пользователей
3. Покупатель (Customer)
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
❌ Просмотр чужих заказов
❌ Управление товарами
Проверка прав доступа
// Лямбда-функция для проверки прав
auto hasAccessToAllOrders = [](const string& role) -> bool {
    return role == "admin" || role == "manager";
};

// Использование в коде
if (hasAccessToAllOrders(user->getRole())) {
    // Показать все заказы
} else {
    // Показать только свои заказы
}

// Комплексная проверка прав
auto canPerformAction = [](const string& role, const string& action) -> bool {
    if (role == "admin") return true;  // admin может всё
    if (role == "manager" && action != "delete_user") return true;
    if (role == "customer" && (action == "create_order" || action == "view_own_orders")) return true;
    return false;
};
Аудит и логирование
Автоматическое логирование через триггеры
Все изменения в основных таблицах автоматически логируются:

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
Функции для работы с аудитом
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
Таблица audit_log
Поле	Тип	Описание
log_id	SERIAL	Уникальный ID записи
user_id	INT	ID пользователя, выполнившего операцию
operation	VARCHAR	Операция (INSERT, UPDATE, DELETE)
table_name	VARCHAR	Имя таблицы
record_id	INT	ID изменённой записи
details	TEXT	Дополнительные детали
created_at	TIMESTAMP	Время операции
Примеры логирования
-- Логирование создания заказа
INSERT INTO audit_log (user_id, operation, table_name, record_id, details)
VALUES (3, 'CREATE', 'orders', 101, 'Order created for customer');

-- Логирование изменения статуса
INSERT INTO audit_log (user_id, operation, table_name, record_id, details)
VALUES (2, 'UPDATE', 'orders', 101, 'Status changed from pending to completed');

-- Логирование возврата товара
INSERT INTO audit_log (user_id, operation, table_name, record_id, details)
VALUES (3, 'RETURN', 'order_items', 201, 'Product returned within 30 days');
Примеры SQL запросов
Получение информации
-- 1. Получить все заказы пользователя
SELECT * FROM orders WHERE user_id = 3;

-- 2. Получить детали заказа с товарами
SELECT o.order_id, o.order_date, p.name, oi.quantity, oi.unit_price
FROM orders o
JOIN order_items oi ON o.order_id = oi.order_id
JOIN products p ON oi.product_id = p.product_id
WHERE o.order_id = 101;

-- 3. Получить популярные товары
SELECT product_id, COUNT(*) as purchase_count
FROM order_items
GROUP BY product_id
ORDER BY purchase_count DESC
LIMIT 5;

-- 4. Получить статус заказа
SELECT getOrderStatus(101);

-- 5. Получить всё потраченное пользователем
SELECT getTotalSpentByUser(3);

-- 6. Получить количество заказов пользователя
SELECT getUserOrderCount(3);
Модификация данных
-- 1. Создать новый заказ
CALL createOrder(3, '2025-01-13');

-- 2. Добавить товар в заказ
CALL addItemToOrder(101, 1, 2);  -- заказ 101, товар 1, кол-во 2

-- 3. Обновить статус заказа
CALL updateOrderStatus(101, 'completed');

-- 4. Отменить заказ
CALL cancelOrder(101);

-- 5. Вернуть товар (если в пределах 30 дней)
CALL returnOrder(101);

-- 6. Удалить товар из заказа
CALL removeItemFromOrder(101, 1);
Сложные запросы
-- 1. Получить заказы с рассчитанной суммой
SELECT 
    o.order_id,
    o.order_date,
    u.name,
    SUM(oi.quantity * oi.unit_price) as total
FROM orders o
JOIN users u ON o.user_id = u.user_id
JOIN order_items oi ON o.order_id = oi.order_id
GROUP BY o.order_id, o.order_date, u.name
ORDER BY total DESC;

-- 2. Получить доход по менеджерам
SELECT 
    u.name,
    COUNT(o.order_id) as orders_count,
    SUM(o.total_amount) as total_revenue
FROM orders o
JOIN audit_log a ON o.order_id = a.record_id
JOIN users u ON a.user_id = u.user_id
WHERE u.role = 'manager' AND a.operation = 'UPDATE'
GROUP BY u.name
ORDER BY total_revenue DESC;

-- 3. Товары, требующие пополнения (менее 5 штук)
SELECT * FROM products WHERE quantity_in_stock < 5;

-- 4. История платежей по дате
SELECT * FROM audit_log
WHERE operation = 'PAYMENT'
ORDER BY created_at DESC
LIMIT 10;

-- 5. Процент возвратов по товарам
SELECT 
    p.name,
    COUNT(CASE WHEN o.status = 'returned' THEN 1 END) as returns,
    COUNT(o.order_id) as total,
    ROUND(100.0 * COUNT(CASE WHEN o.status = 'returned' THEN 1 END) / COUNT(o.order_id), 2) as return_percent
FROM orders o
JOIN order_items oi ON o.order_id = oi.order_id
JOIN products p ON oi.product_id = p.product_id
GROUP BY p.name
ORDER BY return_percent DESC;
Сборка и запуск




Файлы проекта
online-store/
├── src/
│   └── main.cpp                 # Главная программа
├── include/
│   ├── database_connection.h    # Шаблон для БД
│   ├── payment_strategy.h       # Стратегии оплаты
│   ├── payment.h                # Класс платежа
│   ├── order_item.h             # Элемент заказа
│   ├── order.h                  # Класс заказа
│   ├── product.h                # Класс товара
│   ├── user.h                   # Базовый класс
│   ├── admin.h                  # Администратор
│   ├── manager.h                # Менеджер
│   └── customer.h               # Покупатель
├── sql/
│   ├── tables.sql               # Таблицы (1-е выполнить)
│   ├── functions.sql            # Функции (2-е)
│   ├── procedures.sql           # Процедуры (3-е)
│   ├── triggers.sql             # Триггеры (4-е)
│   └── sample_data.sql          # Примеры (5-е)
├── CMakeLists.txt
└── README.md                    # Эта документация




Контакты и поддержка
Автор: Артём Талахов
GitHub: https://github.com/Artxmvl
Email: talakhovartem2007@gmail.com
