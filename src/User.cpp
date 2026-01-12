#include "../include/User.h"
#include <iostream>
using namespace std;

User::User(int id, const string& name, const string& email,
           const string& password, const string& role)
    : id(id), name(name), email(email), password(password), role(role) {}

Admin::Admin(int id, const string& name, const string& email,
             const string& password)
    : User(id, name, email, password, "admin") {}

void Admin::viewOrders() {
    cout << "Admin " << name << " viewing all orders..." << endl;
}

void Admin::manageProducts() {
    cout << "Admin managing products..." << endl;
}

void Admin::manageUsers() {
    cout << "Admin managing users..." << endl;
}

Manager::Manager(int id, const string& name, const string& email,
                 const string& password)
    : User(id, name, email, password, "manager") {}

void Manager::viewOrders() {
    cout << "Manager " << name << " viewing orders..." << endl;
}

void Manager::manageProducts() {
    cout << "Manager updating product quantities..." << endl;
}

Customer::Customer(int id, const string& name, const string& email,
                   const string& password)
    : User(id, name, email, password, "customer") {}

void Customer::viewOrders() {
    cout << "Customer " << name << " viewing own orders..." << endl;
}

void Customer::createOrder() {
    cout << "Customer " << name << " creating order..." << endl;
}

void Customer::returnOrder() {
    cout << "Customer " << name << " returning order..." << endl;
}
