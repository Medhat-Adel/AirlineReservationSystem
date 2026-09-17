#ifndef BOARDING_PASS_H
#define BOARDING_PASS_H

#include <string>

class BoardingPass
{
private:
    int id;
    std::string passNumber;

    int reservationId;

    std::string passengerName;
    std::string passportNumber;

    std::string flightNumber;
    std::string seatNumber;

    std::string gate;
    std::string boardingTime;

public:
    BoardingPass(
        int id,
        const std::string& passNumber,
        int reservationId,
        const std::string& passengerName,
        const std::string& passportNumber,
        const std::string& flightNumber,
        const std::string& seatNumber,
        const std::string& gate,
        const std::string& boardingTime
    );

    int getId() const;

    const std::string& getPassNumber() const;

    int getReservationId() const;

    const std::string& getPassengerName() const;

    const std::string& getPassportNumber() const;

    const std::string& getFlightNumber() const;

    const std::string& getSeatNumber() const;

    const std::string& getGate() const;

    const std::string& getBoardingTime() const;
};

#endif