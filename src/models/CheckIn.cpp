#include "models/CheckIn.h"

#include <stdexcept>

CheckIn::CheckIn(
    int id,
    int reservationId,
    CheckInType type,
    const std::string& checkInDateTime,
    CheckInStatus status
)
    : id(id),
      reservationId(reservationId),
      type(type),
      status(status),
      checkInDateTime(checkInDateTime)
{
    if (id <= 0)
    {
        throw std::invalid_argument(
            "Check-in ID must be positive."
        );
    }

    if (reservationId <= 0)
    {
        throw std::invalid_argument(
            "Reservation ID must be positive."
        );
    }

    if (checkInDateTime.empty())
    {
        throw std::invalid_argument(
            "Check-in date and time cannot be empty."
        );
    }
}

int CheckIn::getId() const
{
    return id;
}

int CheckIn::getReservationId() const
{
    return reservationId;
}

CheckInType CheckIn::getType() const
{
    return type;
}

CheckInStatus CheckIn::getStatus() const
{
    return status;
}

const std::string& CheckIn::getCheckInDateTime() const
{
    return checkInDateTime;
}

void CheckIn::setStatus(CheckInStatus status)
{
    this->status = status;
}