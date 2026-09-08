#include "models/Payment.h"

#include <stdexcept>

Payment::Payment(
    int id,
    int reservationId,
    double amount,
    PaymentMethod method,
    const std::string& transactionDate,
    PaymentStatus status
)
    : id(id),
      reservationId(reservationId),
      amount(amount),
      method(method),
      status(status),
      transactionDate(transactionDate)
{
    if (id <= 0)
    {
        throw std::invalid_argument(
            "Payment ID must be positive."
        );
    }

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

    if (transactionDate.empty())
    {
        throw std::invalid_argument(
            "Transaction date cannot be empty."
        );
    }
}

int Payment::getId() const
{
    return id;
}

int Payment::getReservationId() const
{
    return reservationId;
}

double Payment::getAmount() const
{
    return amount;
}

PaymentMethod Payment::getMethod() const
{
    return method;
}

PaymentStatus Payment::getStatus() const
{
    return status;
}

const std::string& Payment::getTransactionDate() const
{
    return transactionDate;
}

void Payment::setStatus(PaymentStatus status)
{
    this->status = status;
}