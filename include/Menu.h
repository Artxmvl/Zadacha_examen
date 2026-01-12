#ifndef MENU_H
#define MENU_H

#include <string>
#include <memory>
#include "User.h"
#include "DatabaseConnection.h"
using namespace std;

class Menu {
private:
    shared_ptr<User> currentUser;
    DatabaseConnection<string> db;
    
public:
    Menu();
    void start();
    void showRoleSelection();
    void loginAdmin();
    void loginManager();
    void loginCustomer();
};

#endif
