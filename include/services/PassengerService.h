#ifndef PASSENGER_SERVICE_H
#define PASSENGER_SERVICE_H

#include <memory>
#include <string>
#include <vector>

#include "models/Passenger.h"
#include "models/Flight.h"

class PassengerService
{
public:
    void updateContactInformation(
        const std::shared_ptr<Passenger>& passenger,
        const std::string& email,
        const std::string& phone
    ) const;

    void setPreferences(
        const std::shared_ptr<Passenger>& passenger,
        const std::string& preferredSeat,
        const std::string& mealPreference
    ) const;

    void addTravelHistory(
        const std::shared_ptr<Passenger>& passenger,
        const std::shared_ptr<Flight>& flight
    ) const;

    const std::vector<std::shared_ptr<Flight>>&
    getTravelHistory(
        const std::shared_ptr<Passenger>& passenger
    ) const;

    void displayProfile(
        const std::shared_ptr<Passenger>& passenger
    ) const;
};

#endif