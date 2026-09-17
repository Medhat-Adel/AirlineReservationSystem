#include "models/Passenger.h"

#include <stdexcept>

Passenger::Passenger(
    int id,
    const std::string& username,
    const std::string& passwordHash,
    const std::string& fullName,
    const std::string& email,
    const std::string& phone,
    const std::string& passportNumber,
    int loyaltyPoints,
    bool isActive
)
    : User(
        id,
        username,
        passwordHash,
        fullName,
        email,
        phone,
        Role::Passenger,
        isActive
    ),
      passportNumber(passportNumber),
      loyaltyPoints(loyaltyPoints),
      preferredSeat(""),
      mealPreference("")
{
    if (passportNumber.empty())
    {
        throw std::invalid_argument(
            "Passport number cannot be empty."
        );
    }

    if (loyaltyPoints < 0)
    {
        throw std::invalid_argument(
            "Loyalty points cannot be negative."
        );
    }
}

const std::string& Passenger::getPassportNumber() const
{
    return passportNumber;
}

int Passenger::getLoyaltyPoints() const
{
    return loyaltyPoints;
}

void Passenger::addLoyaltyPoints(int points)
{
    if (points <= 0)
    {
        throw std::invalid_argument(
            "Points must be positive."
        );
    }

    loyaltyPoints += points;
}

bool Passenger::redeemLoyaltyPoints(int points)
{
    if (points <= 0)
    {
        return false;
    }

    if (points > loyaltyPoints)
    {
        return false;
    }

    loyaltyPoints -= points;

    return true;
}

const std::string& Passenger::getPreferredSeat() const
{
    return preferredSeat;
}

const std::string& Passenger::getMealPreference() const
{
    return mealPreference;
}

void Passenger::setPreferredSeat(
    const std::string& preferredSeat
)
{
    if (preferredSeat.empty())
    {
        throw std::invalid_argument(
            "Preferred seat cannot be empty."
        );
    }

    this->preferredSeat = preferredSeat;
}

void Passenger::setMealPreference(
    const std::string& mealPreference
)
{
    if (mealPreference.empty())
    {
        throw std::invalid_argument(
            "Meal preference cannot be empty."
        );
    }

    this->mealPreference = mealPreference;
}

void Passenger::addTravelHistory(
    const std::shared_ptr<Flight>& flight
)
{
    if (flight == nullptr)
    {
        throw std::invalid_argument(
            "Flight cannot be null."
        );
    }

    travelHistory.push_back(flight);
}

const std::vector<std::shared_ptr<Flight>>&
Passenger::getTravelHistory() const
{
    return travelHistory;
}

void Passenger::displayMenu() const
{
    std::cout << "\n========================================\n";
    std::cout << "          PASSENGER MENU\n";
    std::cout << "========================================\n";
    std::cout << "1. Search Flights\n";
    std::cout << "2. Make Reservation\n";
    std::cout << "3. Manage Reservations\n";
    std::cout << "4. Online Check-In\n";
    std::cout << "5. View Boarding Pass\n";
    std::cout << "6. View Profile\n";
    std::cout << "7. Loyalty Program\n";
    std::cout << "8. Logout\n";
}