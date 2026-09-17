#ifndef PASSENGER_H
#define PASSENGER_H

#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "models/Flight.h"
#include "models/User.h"

class Passenger : public User
{
private:
    std::string passportNumber;
    int loyaltyPoints;

    // Passenger preferences
    std::string preferredSeat;
    std::string mealPreference;

    // Previous flights
    std::vector<std::shared_ptr<Flight>> travelHistory;

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

    const std::string& getPassportNumber() const;

    int getLoyaltyPoints() const;

    void addLoyaltyPoints(int points);

    bool redeemLoyaltyPoints(int points);

    // Preferences
    const std::string& getPreferredSeat() const;

    const std::string& getMealPreference() const;

    void setPreferredSeat(
        const std::string& preferredSeat
    );

    void setMealPreference(
        const std::string& mealPreference
    );

    // Travel history
    void addTravelHistory(
        const std::shared_ptr<Flight>& flight
    );

    const std::vector<std::shared_ptr<Flight>>&
    getTravelHistory() const;

    void displayMenu() const override;
};

#endif