#include "services/LoyaltyService.h"

#include <memory>
#include <stdexcept>

int LoyaltyService::calculatePoints(double amount) const
{
    if (amount <= 0)
    {
        throw std::invalid_argument(
            "Amount must be positive."
        );
    }

    return static_cast<int>(
        amount / pointsPerUnit
    );
}

void LoyaltyService::awardPoints(
    const std::shared_ptr<Passenger>& passenger,
    double amount
) const
{
    if (passenger == nullptr)
    {
        throw std::invalid_argument(
            "Passenger cannot be null."
        );
    }

    const int points = calculatePoints(amount);

    if (points > 0)
    {
        passenger->addLoyaltyPoints(points);
    }
}