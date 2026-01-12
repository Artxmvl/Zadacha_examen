#include <iostream>
#include <memory>
#include "../include/Menu.h"
#include "../include/User.h"
#include "../include/Order.h"
#include "../include/Payment.h"
using namespace std;

int main() {
    cout << "Welcome to Internet Shop Management System!" << endl;
    cout << "Author: Artem Talakhov" << endl << endl;
    
    Menu menu;
    menu.start();
    
    return 0;
}
