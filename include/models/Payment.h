#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>

enum class PaymentMethod
{
    Cash,
    Card,
    BankTransfer
};

enum class PaymentStatus
{
    Pending,
    Completed,
    Failed,
    Refunded
};

class Payment
{
private:
    int id;
    int reservationId;

    double amount;

    PaymentMethod method;
    PaymentStatus status;

    std::string transactionDate;

public:
    Payment(
        int id,
        int reservationId,
        double amount,
        PaymentMethod method,
        const std::string& transactionDate,
        PaymentStatus status = PaymentStatus::Pending
    );

    int getId() const;
    int getReservationId() const;

    double getAmount() const;

    PaymentMethod getMethod() const;
    PaymentStatus getStatus() const;

    const std::string& getTransactionDate() const;

    void setStatus(PaymentStatus status);
};

#endif