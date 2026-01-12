#ifndef USER_H
#define USER_H

#include <string>
#include <memory>
#include <vector>
#include "Order.h"
using namespace std;

class User {
protected:
    int id;
    string name;
    string email;
    string password;
    string role;
    vector<shared_ptr<Order>> orders;
    
public:
    User(int id, const string& name, const string& email, 
         const string& password, const string& role);
    virtual ~User() = default;
    
    virtual void viewOrderStatus(int orderId) = 0;
    virtual void viewAllOrders() {}
    virtual void addProduct(const string& name, double price, int quantity) {}
    virtual void updateProduct(int productId, double price, int quantity) {}
    virtual void deleteProduct(int productId) {}
    
    int getId() const { return id; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getRole() const { return role; }
    const vector<shared_ptr<Order>>& getOrders() const { return orders; }
    
    void addOrder(shared_ptr<Order> order) {
        orders.push_back(order);
    }
};

class Admin : public User {
public:
    Admin(int id, const string& name, const string& email, const string& password);
    
    void viewOrderStatus(int orderId) override;
    void viewAllOrders() override;
    void addProduct(const string& name, double price, int quantity) override;
    void updateProduct(int productId, double price, int quantity) override;
    void deleteProduct(int productId) override;
};

class Manager : public User {
public:
    Manager(int id, const string& name, const string& email, const string& password);
    
    void viewOrderStatus(int orderId) override;
    void viewAllOrders() override;
    void approveOrder(int orderId);
    void updateStock(int productId, int quantity);
};

class Customer : public User {
public:
    Customer(int id, const string& name, const string& email, const string& password);
    
    void viewOrderStatus(int orderId) override;
    void createOrder();
    void addToOrder(shared_ptr<Order> order, int productId, int quantity);
    void removeFromOrder(shared_ptr<Order> order, int productId);
    void makePayment(shared_ptr<Order> order, const string& paymentMethod);
    void returnOrder(int orderId);
};

#endif
