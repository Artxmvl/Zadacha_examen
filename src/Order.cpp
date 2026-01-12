#include "../include/Order.h"
#include "../include/Payment.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <ctime>
using namespace std;

OrderItem::OrderItem(int productId, int quantity, double price)
    : productId(productId), quantity(quantity), price(price) {}

Order::Order(int id, int userId, const string& status, double totalAmount)
    : id(id), userId(userId), status(status), totalAmount(totalAmount), payment(nullptr) {}

void Order::addItem(unique_ptr<OrderItem> item) {
    items.push_back(move(item));
    cout << "Item added to order #" << id << endl;
}

void Order::removeItem(int productId) {
    auto it = find_if(items.begin(), items.end(),
        [productId](const unique_ptr<OrderItem>& item) {
            return item->getProductId() == productId;
        });
    
    if (it != items.end()) {
        items.erase(it);
        cout << "Item removed from order #" << id << endl;
    }
}

void Order::updateStatus(const string& newStatus) {
    cout << "Order #" << id << " status: " << status << " -> " << newStatus << endl;
    status = newStatus;
}

void Order::setPayment(unique_ptr<Payment> pmt) {
    payment = move(pmt);
}

bool Order::canBeReturned() const {
    return status == "completed";
}
