#include "../include/Order.h"
#include <iostream>
using namespace std;

OrderItem::OrderItem(int productId, int quantity, double price)
    : productId(productId), quantity(quantity), price(price) {}

Order::Order(int id, int userId, const string& status, double totalAmount)
    : id(id), userId(userId), status(status), totalAmount(totalAmount) {}

void Order::addItem(unique_ptr<OrderItem> item) {
    items.push_back(move(item));
}

void Order::updateStatus(const string& newStatus) {
    status = newStatus;
    cout << "Order " << id << " status updated to: " << status << endl;
}
