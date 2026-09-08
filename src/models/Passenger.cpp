#include "models/Passenger.h"
#include <iostream>
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
      loyaltyPoints(loyaltyPoints)
{
}

void Passenger::displayMenu() const
{
    std::cout << "\n";
    std::cout << "===== Passenger Menu =====\n";
    std::cout << "1. Search Flights\n";
    std::cout << "2. Make Reservation\n";
    std::cout << "3. Manage Reservations\n";
    std::cout << "4. Online Check-In\n";
    std::cout << "5. View Boarding Pass\n";
    std::cout << "6. View Profile\n";
    std::cout << "7. Loyalty Program\n";
    std::cout << "0. Logout\n";
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
    if (points < 0)
    {
        throw std::invalid_argument("Loyalty points cannot be negative.");
    }

    loyaltyPoints += points;
}

void Passenger::redeemLoyaltyPoints(int points)
{
    if (points <= 0)
    {
        throw std::invalid_argument("Points to redeem must be positive.");
    }

    if (points > loyaltyPoints)
    {
        throw std::runtime_error("Insufficient loyalty points.");
    }

    loyaltyPoints -= points;
}