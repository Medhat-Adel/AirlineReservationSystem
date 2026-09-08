#ifndef PASSENGER_H
#define PASSENGER_H

#include "models/User.h"

class Passenger : public User
{
private:
    std::string passportNumber;
    int loyaltyPoints;

public:
    Passenger(
        int id,
        const std::string& username,
        const std::string& passwordHash,
        const std::string& fullName,
        const std::string& email,
        const std::string& phone,
        const std::string& passportNumber,
        int loyaltyPoints = 0,
        bool isActive = true
    );

    void displayMenu() const override;

    const std::string& getPassportNumber() const;
    int getLoyaltyPoints() const;

    void addLoyaltyPoints(int points);
    void redeemLoyaltyPoints(int points);
};

#endif