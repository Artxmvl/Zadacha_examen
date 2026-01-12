#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>

class Payment {
public:
    virtual ~Payment() = default;
    virtual bool pay(double amount) = 0;
    virtual std::string getPaymentMethod() const = 0;
};

class CardPayment : public Payment {
private:
    std::string cardNumber;
    
public:
    CardPayment(const std::string& cardNumber);
    bool pay(double amount) override;
    std::string getPaymentMethod() const override { return "Card"; }
};

class EWalletPayment : public Payment {
private:
    std::string walletId;
    
public:
    EWalletPayment(const std::string& walletId);
    bool pay(double amount) override;
    std::string getPaymentMethod() const override { return "E-Wallet"; }
};

class SBPPayment : public Payment {
private:
    std::string phoneNumber;
    
public:
    SBPPayment(const std::string& phoneNumber);
    bool pay(double amount) override;
    std::string getPaymentMethod() const override { return "SBP"; }
};

#endif
