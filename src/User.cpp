#include "../include/User.h"
#include <iostream>
#include <algorithm>
using namespace std;

User::User(int id, const string& name, const string& email,
           const string& password, const string& role)
    : id(id), name(name), email(email), password(password), role(role) {}

Admin::Admin(int id, const string& name, const string& email,
             const string& password)
    : User(id, name, email, password, "admin") {}

void Admin::viewOrderStatus(int orderId) {
    cout << "Admin " << name << " viewing order #" << orderId << endl;
}

void Admin::viewAllOrders() {
    cout << "Admin " << name << " viewing all orders..." << endl;
    cout << "Total orders in system: " << orders.size() << endl;
}

void Admin::addProduct(const string& name, double price, int quantity) {
    cout << "Admin " << this->name << " adding product: " << name << " (Price: " << price << ", Qty: " << quantity << ")" << endl;
}

void Admin::updateProduct(int productId, double price, int quantity) {
    cout << "Admin " << name << " updating product #" << productId << " (Price: " << price << ", Qty: " << quantity << ")" << endl;
}

void Admin::deleteProduct(int productId) {
    cout << "Admin " << name << " deleting product #" << productId << endl;
}

Manager::Manager(int id, const string& name, const string& email,
                 const string& password)
    : User(id, name, email, password, "manager") {}

void Manager::viewOrderStatus(int orderId) {
    cout << "Manager " << name << " viewing order #" << orderId << endl;
}

void Manager::viewAllOrders() {
    cout << "Manager " << name << " viewing pending orders..." << endl;
}

void Manager::approveOrder(int orderId) {
    cout << "Manager " << name << " approving order #" << orderId << endl;
}

void Manager::updateStock(int productId, int quantity) {
    cout << "Manager " << name << " updating stock for product #" << productId << " to " << quantity << endl;
}

Customer::Customer(int id, const string& name, const string& email,
                   const string& password)
    : User(id, name, email, password, "customer") {}

void Customer::viewOrderStatus(int orderId) {
    cout << "Customer " << name << " viewing order #" << orderId << endl;
    
    auto it = find_if(orders.begin(), orders.end(),
        [orderId](const shared_ptr<Order>& order) {
            return order->getId() == orderId;
        });
    
    if (it != orders.end()) {
        cout << "  Status: " << (*it)->getStatus() << endl;
        cout << "  Total: " << (*it)->getTotalAmount() << " rubles" << endl;
    }
}

void Customer::createOrder() {
    cout << "Customer " << name << " creating new order..." << endl;
}

void Customer::addToOrder(shared_ptr<Order> order, int productId, int quantity) {
    cout << "Customer " << name << " adding product #" << productId << " (qty: " << quantity << ") to order #" << order->getId() << endl;
}

void Customer::removeFromOrder(shared_ptr<Order> order, int productId) {
    cout << "Customer " << name << " removing product #" << productId << " from order #" << order->getId() << endl;
}

void Customer::makePayment(shared_ptr<Order> order, const string& paymentMethod) {
    cout << "Customer " << name << " paying " << order->getTotalAmount() << " rubles for order #" << order->getId() << endl;
    cout << "Payment method: " << paymentMethod << endl;
}

void Customer::returnOrder(int orderId) {
    cout << "Customer " << name << " requesting return for order #" << orderId << endl;
}
