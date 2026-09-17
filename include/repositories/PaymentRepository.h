#ifndef PAYMENT_REPOSITORY_H
#define PAYMENT_REPOSITORY_H

#include <memory>
#include <string>
#include <vector>

#include "models/Payment.h"

class PaymentRepository
{
private:
    std::string filePath;

public:
    explicit PaymentRepository(
        const std::string& filePath = "data/payments.json"
    );

    void save(
        const std::vector<std::shared_ptr<Payment>>& payments
    ) const;

    std::vector<std::shared_ptr<Payment>> load() const;
};

#endif