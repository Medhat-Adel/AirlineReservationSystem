#include "models/Flight.h"

#include <stdexcept>

Flight::Flight(
    const std::string& flightNumber,
    const std::string& origin,
    const std::string& destination,
    const std::string& departureTime,
    const std::string& arrivalTime,
    double price,
    const std::shared_ptr<Aircraft>& aircraft,
    int flightDurationHours,
    FlightStatus status
)
    : flightNumber(flightNumber),
      origin(origin),
      destination(destination),
      departureTime(departureTime),
      arrivalTime(arrivalTime),
      price(price),
      status(status),
      aircraft(aircraft),
      flightDurationHours(flightDurationHours)
{
    if (flightNumber.empty())
    {
        throw std::invalid_argument(
            "Flight number cannot be empty."
        );
    }

    if (origin.empty())
    {
        throw std::invalid_argument(
            "Origin cannot be empty."
        );
    }

    if (destination.empty())
    {
        throw std::invalid_argument(
            "Destination cannot be empty."
        );
    }

    if (price < 0)
    {
        throw std::invalid_argument(
            "Flight price cannot be negative."
        );
    }

    if (aircraft == nullptr)
    {
        throw std::invalid_argument(
            "Aircraft cannot be null."
        );
    }

    if (flightDurationHours <= 0)
    {
        throw std::invalid_argument(
            "Flight duration must be positive."
        );
    }
}

// Getters

const std::string& Flight::getFlightNumber() const
{
    return flightNumber;
}

const std::string& Flight::getOrigin() const
{
    return origin;
}

const std::string& Flight::getDestination() const
{
    return destination;
}

const std::string& Flight::getDepartureTime() const
{
    return departureTime;
}

const std::string& Flight::getArrivalTime() const
{
    return arrivalTime;
}

double Flight::getPrice() const
{
    return price;
}

FlightStatus Flight::getStatus() const
{
    return status;
}

std::shared_ptr<Aircraft> Flight::getAircraft() const
{
    return aircraft;
}

// Setters

void Flight::setPrice(double price)
{
    if (price < 0)
    {
        throw std::invalid_argument(
            "Flight price cannot be negative."
        );
    }

    this->price = price;
}

void Flight::setStatus(FlightStatus status)
{
    this->status = status;
}

void Flight::setAircraft(
    const std::shared_ptr<Aircraft>& aircraft
)
{
    if (!aircraft)
    {
        throw std::invalid_argument(
            "Aircraft cannot be null."
        );
    }

    this->aircraft = aircraft;
}

// Seat management

bool Flight::reserveSeat(const std::string& seatNumber)
{
    if (seatNumber.empty())
    {
        throw std::invalid_argument(
            "Seat number cannot be empty."
        );
    }

    if (!isSeatAvailable(seatNumber))
    {
        return false;
    }

    if (static_cast<int>(occupiedSeats.size())
        >= aircraft->getCapacity())
    {
        return false;
    }

    occupiedSeats.insert(seatNumber);

    return true;
}

bool Flight::releaseSeat(const std::string& seatNumber)
{
    return occupiedSeats.erase(seatNumber) > 0;
}

bool Flight::isSeatAvailable(
    const std::string& seatNumber
) const
{
    return occupiedSeats.find(seatNumber)
        == occupiedSeats.end();
}

const std::set<std::string>& Flight::getOccupiedSeats() const
{
    return occupiedSeats;
}

int Flight::getAvailableSeats() const
{
    return aircraft->getCapacity()
        - static_cast<int>(occupiedSeats.size());
}

bool Flight::assignCrewMember(
    const std::shared_ptr<CrewMember>& crewMember
)
{
    if (crewMember == nullptr)
    {
        throw std::invalid_argument(
            "Crew member cannot be null."
        );
    }

    for (const auto& member : crewMembers)
    {
        if (member->getId() == crewMember->getId())
        {
            return false;
        }
    }

    if (!crewMember->canAssignFlight(flightDurationHours))
    {
        return false;
    }

    crewMember->addFlightHours(flightDurationHours);

    crewMembers.push_back(crewMember);

    return true;
}

bool Flight::removeCrewMember(int crewMemberId)
{
    for (auto it = crewMembers.begin();
         it != crewMembers.end();
         ++it)
    {
        if ((*it)->getId() == crewMemberId)
        {
            (*it)->subtractFlightHours(
                flightDurationHours
            );

            crewMembers.erase(it);

            return true;
        }
    }

    return false;
}

const std::vector<std::shared_ptr<CrewMember>>&
Flight::getCrewMembers() const
{
    return crewMembers;
}

int Flight::getFlightDurationHours() const
{
    return flightDurationHours;
}