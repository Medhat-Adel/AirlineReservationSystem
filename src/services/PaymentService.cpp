#include "services/PaymentService.h"

#include <stdexcept>

PaymentService::PaymentService()
    : nextPaymentId(1)
{
}

std::shared_ptr<Payment> PaymentService::processPayment(
    int reservationId,
    double amount,
    PaymentMethod method,
    const std::string& transactionDate
)
{
    if (reservationId <= 0)
    {
        throw std::invalid_argument(
            "Reservation ID must be positive."
        );
    }

    if (amount <= 0)
    {
        throw std::invalid_argument(
            "Payment amount must be positive."
        );
    }

    auto payment = std::make_shared<Payment>(
        nextPaymentId++,
        reservationId,
        amount,
        method,
        transactionDate,
        PaymentStatus::Pending
    );

    // Simulate successful payment
    payment->setStatus(
        PaymentStatus::Completed
    );

    payments.push_back(payment);

    return payment;
}

bool PaymentService::refundPayment(int paymentId)
{
    auto payment = getPayment(paymentId);

    if (payment == nullptr)
    {
        return false;
    }

    if (payment->getStatus() !=
        PaymentStatus::Completed)
    {
        return false;
    }

    payment->setStatus(
        PaymentStatus::Refunded
    );

    return true;
}

std::shared_ptr<Payment> PaymentService::getPayment(
    int paymentId
) const
{
    for (const auto& payment : payments)
    {
        if (payment->getId() == paymentId)
        {
            return payment;
        }
    }

    return nullptr;
}

std::shared_ptr<Payment>
PaymentService::getPaymentByReservationId(
    int reservationId
) const
{
    for (const auto& payment : payments)
    {
        if (payment->getReservationId() == reservationId)
        {
            return payment;
        }
    }

    return nullptr;
}

const std::vector<std::shared_ptr<Payment>>&
PaymentService::getAllPayments() const
{
    return payments;
}