#ifndef RESERVATION_REPOSITORY_H
#define RESERVATION_REPOSITORY_H

#include <memory>
#include <string>
#include <vector>

#include "models/Passenger.h"
#include "models/Reservation.h"
#include "models/Flight.h"

class ReservationRepository
{
private:
    std::string filePath;

public:
    explicit ReservationRepository(
        const std::string& filePath = "data/reservations.json"
    );

    void save(
        const std::vector<std::shared_ptr<Reservation>>& reservations
    ) const;

    std::vector<std::shared_ptr<Reservation>> load(
        const std::vector<std::shared_ptr<Passenger>>& passengers,
        const std::vector<std::shared_ptr<Flight>>& flights
    ) const;
};

#endif