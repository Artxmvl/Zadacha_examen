#include "../include/Menu.h"
#include <iostream>
using namespace std;

Menu::Menu() : db("host=localhost user=postgres password=password dbname=shop") {
    if (!db.isConnected()) {
        cout << "Warning: Database connection failed!" << endl;
    }
}

void Menu::start() {
    int choice = 0;
    while (choice != 4) {
        showRoleSelection();
        cin >> choice;
        
        switch (choice) {
            case 1:
                loginAdmin();
                break;
            case 2:
                loginManager();
                break;
            case 3:
                loginCustomer();
                break;
            case 4:
                cout << "Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
}

void Menu::showRoleSelection() {
    cout << "\n=== Internet Shop Management System ===" << endl;
    cout << "1. Login as Admin" << endl;
    cout << "2. Login as Manager" << endl;
    cout << "3. Login as Customer" << endl;
    cout << "4. Exit" << endl;
    cout << "Choose role: ";
}

void Menu::loginAdmin() {
    cout << "\n=== Admin Menu ===" << endl;
    currentUser = make_shared<Admin>(1, "Ivan Ivanov", "admin@gmail.com", "art1");
    
    int choice = 0;
    while (choice != 10) {
        cout << "\n1. Add product" << endl;
        cout << "2. Update product" << endl;
        cout << "3. Delete product" << endl;
        cout << "4. View all orders" << endl;
        cout << "5. View order details" << endl;
        cout << "6. Change order status" << endl;
        cout << "7. View order history" << endl;
        cout << "8. View audit log" << endl;
        cout << "9. Generate CSV report" << endl;
        cout << "10. Logout" << endl;
        cout << "Choose: ";
        cin >> choice;
        
        auto admin = dynamic_cast<Admin*>(currentUser.get());
        
        if (choice == 1) {
            admin->addProduct("Laptop", 50000, 10);
        } else if (choice == 2) {
            admin->updateProduct(1, 49000, 9);
        } else if (choice == 3) {
            admin->deleteProduct(1);
        } else if (choice == 4) {
            admin->viewAllOrders();
        } else if (choice == 5) {
            admin->viewOrderStatus(1);
        } else if (choice == 10) {
            cout << "Logging out..." << endl;
        }
    }
}

void Menu::loginManager() {
    cout << "\n=== Manager Menu ===" << endl;
    currentUser = make_shared<Manager>(2, "Alexander Alexandrov", "manager@gmail.com", "art2");
    
    int choice = 0;
    while (choice != 8) {
        cout << "\n1. View pending orders" << endl;
        cout << "2. Approve order" << endl;
        cout << "3. Update stock" << endl;
        cout << "4. View order details" << endl;
        cout << "5. Change order status" << endl;
        cout << "6. View approved orders" << endl;
        cout << "7. View order history" << endl;
        cout << "8. Logout" << endl;
        cout << "Choose: ";
        cin >> choice;
        
        auto manager = dynamic_cast<Manager*>(currentUser.get());
        
        if (choice == 1) {
            manager->viewAllOrders();
        } else if (choice == 2) {
            manager->approveOrder(1);
        } else if (choice == 3) {
            manager->updateStock(1, 8);
        } else if (choice == 4) {
            manager->viewOrderStatus(1);
        } else if (choice == 8) {
            cout << "Logging out..." << endl;
        }
    }
}

void Menu::loginCustomer() {
    cout << "\n=== Customer Menu ===" << endl;
    currentUser = make_shared<Customer>(3, "Alexey Sidorov", "customer@example.com", "art3");
    
    int choice = 0;
    while (choice != 9) {
        cout << "\n1. Create order" << endl;
        cout << "2. Add item to order" << endl;
        cout << "3. Remove item from order" << endl;
        cout << "4. View my orders" << endl;
        cout << "5. View order status" << endl;
        cout << "6. Pay order" << endl;
        cout << "7. Return order" << endl;
        cout << "8. View order history" << endl;
        cout << "9. Logout" << endl;
        cout << "Choose: ";
        cin >> choice;
        
        auto customer = dynamic_cast<Customer*>(currentUser.get());
        
        if (choice == 1) {
            customer->createOrder();
        } else if (choice == 4) {
            customer->viewAllOrders();
        } else if (choice == 5) {
            customer->viewOrderStatus(1);
        } else if (choice == 6) {
            auto order = make_shared<Order>(1, 3, "pending", 50000);
            customer->makePayment(order, "Card");
        } else if (choice == 9) {
            cout << "Logging out..." << endl;
        }
    }
}
