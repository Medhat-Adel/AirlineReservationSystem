#include "models/BoardingPass.h"

#include <stdexcept>

BoardingPass::BoardingPass(
    int id,
    const std::string& passNumber,
    int reservationId,
    const std::string& passengerName,
    const std::string& passportNumber,
    const std::string& flightNumber,
    const std::string& seatNumber,
    const std::string& gate,
    const std::string& boardingTime
)
    : id(id),
      passNumber(passNumber),
      reservationId(reservationId),
      passengerName(passengerName),
      passportNumber(passportNumber),
      flightNumber(flightNumber),
      seatNumber(seatNumber),
      gate(gate),
      boardingTime(boardingTime)
{
    if (id <= 0)
    {
        throw std::invalid_argument(
            "Boarding pass ID must be positive."
        );
    }

    if (passNumber.empty())
    {
        throw std::invalid_argument(
            "Boarding pass number cannot be empty."
        );
    }

    if (reservationId <= 0)
    {
        throw std::invalid_argument(
            "Reservation ID must be positive."
        );
    }

    if (passengerName.empty())
    {
        throw std::invalid_argument(
            "Passenger name cannot be empty."
        );
    }

    if (passportNumber.empty())
    {
        throw std::invalid_argument(
            "Passport number cannot be empty."
        );
    }

    if (flightNumber.empty())
    {
        throw std::invalid_argument(
            "Flight number cannot be empty."
        );
    }

    if (seatNumber.empty())
    {
        throw std::invalid_argument(
            "Seat number cannot be empty."
        );
    }

    if (gate.empty())
    {
        throw std::invalid_argument(
            "Gate cannot be empty."
        );
    }

    if (boardingTime.empty())
    {
        throw std::invalid_argument(
            "Boarding time cannot be empty."
        );
    }
}

int BoardingPass::getId() const
{
    return id;
}

const std::string& BoardingPass::getPassNumber() const
{
    return passNumber;
}

int BoardingPass::getReservationId() const
{
    return reservationId;
}

const std::string& BoardingPass::getPassengerName() const
{
    return passengerName;
}

const std::string& BoardingPass::getPassportNumber() const
{
    return passportNumber;
}

const std::string& BoardingPass::getFlightNumber() const
{
    return flightNumber;
}

const std::string& BoardingPass::getSeatNumber() const
{
    return seatNumber;
}

const std::string& BoardingPass::getGate() const
{
    return gate;
}

const std::string& BoardingPass::getBoardingTime() const
{
    return boardingTime;
}