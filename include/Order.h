#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include <memory>
using namespace std;

class Payment;

class OrderItem {
private:
    int productId;
    int quantity;
    double price;
    
public:
    OrderItem(int productId, int quantity, double price);
    
    int getProductId() const { return productId; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    double getTotal() const { return quantity * price; }
};

class Order {
private:
    int id;
    int userId;
    string status;
    double totalAmount;
    vector<unique_ptr<OrderItem>> items;
    unique_ptr<Payment> payment;
    
public:
    Order(int id, int userId, const string& status, double totalAmount);
    ~Order() = default;
    
    void addItem(unique_ptr<OrderItem> item);
    void removeItem(int productId);
    void updateStatus(const string& newStatus);
    void setPayment(unique_ptr<Payment> pmt);
    bool canBeReturned() const;
    
    int getId() const { return id; }
    int getUserId() const { return userId; }
    string getStatus() const { return status; }
    double getTotalAmount() const { return totalAmount; }
    const vector<unique_ptr<OrderItem>>& getItems() const { return items; }
    Payment* getPayment() const { return payment.get(); }
};

#endif
