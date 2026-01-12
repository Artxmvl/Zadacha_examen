#include <iostream>
#include <memory>
#include "../include/User.h"
#include "../include/Order.h"
#include "../include/Payment.h"
using namespace std;

int main() {
    cout << "=== Internet Shop Management System ===" << endl;
    cout << "Exam Work by Artem Talakhov" << endl << endl;
    
    // Create users
    auto admin = make_unique<Admin>(1, "Ivan Ivanov", "admin@gmail.com", "art1");
    auto manager = make_unique<Manager>(2, "Alexander Alexandrov", "manager@gmail.com", "art2");
    auto customer = make_unique<Customer>(3, "Alexey Sidorov", "customer@example.com", "art3");
    
    cout << "Users created:" << endl;
    cout << "- " << admin->getName() << " (Admin)" << endl;
    cout << "- " << manager->getName() << " (Manager)" << endl;
    cout << "- " << customer->getName() << " (Customer)" << endl << endl;
    
    // Admin actions
    admin->viewOrders();
    admin->manageProducts();
    admin->manageUsers();
    cout << endl;
    
    // Manager actions
    manager->viewOrders();
    manager->manageProducts();
    cout << endl;
    
    // Customer actions
    customer->viewOrders();
    customer->createOrder();
    
    // Create order
    Order order(1, 3, "pending", 50000.00);
    order.addItem(make_unique<OrderItem>(1, 1, 50000.00));
    order.updateStatus("completed");
    cout << endl;
    
    // Payment
    CardPayment cardPayment("1234567890123456");
    cardPayment.pay(50000.00);
    cout << endl;
    
    // Customer return
    customer->returnOrder();
    
    return 0;
}
