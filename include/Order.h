#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include <memory>

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
    std::string status;
    double totalAmount;
    std::vector<std::unique_ptr<OrderItem>> items;
    
public:
    Order(int id, int userId, const std::string& status, double totalAmount);
    
    void addItem(std::unique_ptr<OrderItem> item);
    void updateStatus(const std::string& newStatus);
    
    int getId() const { return id; }
    int getUserId() const { return userId; }
    std::string getStatus() const { return status; }
    double getTotalAmount() const { return totalAmount; }
    const std::vector<std::unique_ptr<OrderItem>>& getItems() const { return items; }
};

#endif
