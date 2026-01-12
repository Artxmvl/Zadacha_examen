#ifndef USER_H
#define USER_H

#include <string>
#include <memory>

class User {
protected:
    int id;
    std::string name;
    std::string email;
    std::string password;
    std::string role;
    
public:
    User(int id, const std::string& name, const std::string& email, 
         const std::string& password, const std::string& role);
    virtual ~User() = default;
    
    virtual void viewOrders() = 0;
    virtual void manageProducts() {}
    virtual void manageUsers() {}
    
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getRole() const { return role; }
};

class Admin : public User {
public:
    Admin(int id, const std::string& name, const std::string& email,
          const std::string& password);
    
    void viewOrders() override;
    void manageProducts() override;
    void manageUsers() override;
};

class Manager : public User {
public:
    Manager(int id, const std::string& name, const std::string& email,
            const std::string& password);
    
    void viewOrders() override;
    void manageProducts() override;
};

class Customer : public User {
public:
    Customer(int id, const std::string& name, const std::string& email,
             const std::string& password);
    
    void viewOrders() override;
    void createOrder();
    void returnOrder();
};

#endif
