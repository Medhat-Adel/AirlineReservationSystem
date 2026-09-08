#ifndef LOYALTY_SERVICE_H
#define LOYALTY_SERVICE_H

#include <memory>

#include "models/Passenger.h"

class LoyaltyService
{
private:
    static constexpr int pointsPerUnit = 100;

public:
    int calculatePoints(double amount) const;

    void awardPoints(
        const std::shared_ptr<Passenger>& passenger,
        double amount
    ) const;
};

#endif