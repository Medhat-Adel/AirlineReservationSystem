#ifndef PAYMENT_SERVICE_H
#define PAYMENT_SERVICE_H

#include <memory>
#include <vector>

#include "models/Payment.h"

class PaymentService
{
private:
    std::vector<std::shared_ptr<Payment>> payments;

    int nextPaymentId;

public:
    PaymentService();

    std::shared_ptr<Payment> processPayment(
        int reservationId,
        double amount,
        PaymentMethod method,
        const std::string& transactionDate
    );

    bool refundPayment(int paymentId);

    std::shared_ptr<Payment> getPayment(
        int paymentId
    ) const;

    std::shared_ptr<Payment> getPaymentByReservationId(
        int reservationId
    ) const;

    const std::vector<std::shared_ptr<Payment>>&
    getAllPayments() const;

    
};

#endif