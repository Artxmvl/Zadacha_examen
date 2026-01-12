#include "../include/Payment.h"
#include <iostream>
using namespace std;

CardPayment::CardPayment(const string& cardNumber)
    : cardNumber(cardNumber) {}

bool CardPayment::pay(double amount) {
    cout << "Processing card payment of " << amount << " rubles..." << endl;
    cout << "Card ending in " << cardNumber.substr(cardNumber.length() - 4) << " charged." << endl;
    return true;
}

EWalletPayment::EWalletPayment(const string& walletId)
    : walletId(walletId) {}

bool EWalletPayment::pay(double amount) {
    cout << "Processing e-wallet payment of " << amount << " rubles..." << endl;
    cout << "Wallet " << walletId << " charged." << endl;
    return true;
}

SBPPayment::SBPPayment(const string& phoneNumber)
    : phoneNumber(phoneNumber) {}

bool SBPPayment::pay(double amount) {
    cout << "Processing SBP payment of " << amount << " rubles..." << endl;
    cout << "Phone " << phoneNumber << " charged." << endl;
    return true;
}
