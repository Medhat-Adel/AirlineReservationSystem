#ifndef CHECK_IN_SERVICE_H
#define CHECK_IN_SERVICE_H

#include <memory>
#include <string>
#include <vector>

#include "models/CheckIn.h"
#include "models/BoardingPass.h"
#include "models/Reservation.h"

class CheckInService
{
private:
    std::vector<std::shared_ptr<CheckIn>> checkIns;
    std::vector<std::shared_ptr<BoardingPass>> boardingPasses;

    int nextCheckInId;
    int nextBoardingPassId;

public:
    CheckInService();

    std::shared_ptr<CheckIn> onlineCheckIn(
        const std::shared_ptr<Reservation>& reservation,
        const std::string& checkInDateTime,
        const std::string& gate,
        const std::string& boardingTime
    );

    std::shared_ptr<CheckIn> airportCheckIn(
        const std::shared_ptr<Reservation>& reservation,
        const std::string& checkInDateTime,
        const std::string& gate,
        const std::string& boardingTime
    );

    std::shared_ptr<CheckIn> getCheckInByReservationId(
        int reservationId
    ) const;

    std::shared_ptr<BoardingPass> getBoardingPassByReservationId(
        int reservationId
    ) const;

    const std::vector<std::shared_ptr<CheckIn>>&
    getAllCheckIns() const;

    const std::vector<std::shared_ptr<BoardingPass>>&
    getAllBoardingPasses() const;
};

#endif