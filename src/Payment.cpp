#include "../include/Payment.h"
#include <iostream>
using namespace std;

CardPayment::CardPayment(const string& cardNumber)
    : cardNumber(cardNumber) {}

bool CardPayment::pay(double amount) {
    cout << "Processing card payment..." << endl;
    cout << "Amount: " << amount << " rubles" << endl;
    cout << "Card: **** **** **** " << cardNumber.substr(cardNumber.length() - 4) << endl;
    cout << "Payment successful!" << endl;
    return true;
}

EWalletPayment::EWalletPayment(const string& walletId)
    : walletId(walletId) {}

bool EWalletPayment::pay(double amount) {
    cout << "Processing e-wallet payment..." << endl;
    cout << "Amount: " << amount << " rubles" << endl;
    cout << "Wallet ID: " << walletId << endl;
    cout << "Payment successful!" << endl;
    return true;
}

SBPPayment::SBPPayment(const string& phoneNumber)
    : phoneNumber(phoneNumber) {}

bool SBPPayment::pay(double amount) {
    cout << "Processing SBP payment..." << endl;
    cout << "Amount: " << amount << " rubles" << endl;
    cout << "Phone: " << phoneNumber << endl;
    cout << "Payment successful!" << endl;
    return true;
}
