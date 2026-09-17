#include "models/CrewMember.h"

#include <stdexcept>

CrewMember::CrewMember(
    int id,
    const std::string& employeeId,
    const std::string& fullName,
    int maximumFlightHours,
    bool isActive
)
    : id(id),
      employeeId(employeeId),
      fullName(fullName),
      totalFlightHours(0),
      maximumFlightHours(maximumFlightHours),
      isActive(isActive)
{
    if (id <= 0)
    {
        throw std::invalid_argument(
            "Crew member ID must be positive."
        );
    }

    if (employeeId.empty())
    {
        throw std::invalid_argument(
            "Employee ID cannot be empty."
        );
    }

    if (fullName.empty())
    {
        throw std::invalid_argument(
            "Crew member name cannot be empty."
        );
    }

    if (maximumFlightHours <= 0)
    {
        throw std::invalid_argument(
            "Maximum flight hours must be positive."
        );
    }
}

// ============================================================
// GETTERS
// ============================================================

int CrewMember::getId() const
{
    return id;
}

const std::string& CrewMember::getEmployeeId() const
{
    return employeeId;
}

const std::string& CrewMember::getFullName() const
{
    return fullName;
}

int CrewMember::getTotalFlightHours() const
{
    return totalFlightHours;
}

int CrewMember::getMaximumFlightHours() const
{
    return maximumFlightHours;
}

bool CrewMember::getIsActive() const
{
    return isActive;
}

// ============================================================
// FLIGHT HOURS
// ============================================================

bool CrewMember::canAssignFlight(
    int flightHours
) const
{
    if (flightHours <= 0)
    {
        return false;
    }

    if (!isActive)
    {
        return false;
    }

    return totalFlightHours + flightHours
        <= maximumFlightHours;
}

void CrewMember::addFlightHours(
    int flightHours
)
{
    if (flightHours <= 0)
    {
        throw std::invalid_argument(
            "Flight hours must be positive."
        );
    }

    if (!canAssignFlight(flightHours))
    {
        throw std::runtime_error(
            "Crew member exceeded maximum flight hours."
        );
    }

    totalFlightHours += flightHours;
}

void CrewMember::subtractFlightHours(
    int flightHours
)
{
    if (flightHours <= 0)
    {
        throw std::invalid_argument(
            "Flight hours must be positive."
        );
    }

    if (flightHours > totalFlightHours)
    {
        throw std::runtime_error(
            "Cannot subtract more flight hours than assigned."
        );
    }

    totalFlightHours -= flightHours;
}

// ============================================================
// SETTERS
// ============================================================

void CrewMember::setFullName(
    const std::string& fullName
)
{
    if (fullName.empty())
    {
        throw std::invalid_argument(
            "Crew member name cannot be empty."
        );
    }

    this->fullName = fullName;
}

void CrewMember::setMaximumFlightHours(
    int maximumFlightHours
)
{
    if (maximumFlightHours <= 0)
    {
        throw std::invalid_argument(
            "Maximum flight hours must be positive."
        );
    }

    if (maximumFlightHours < totalFlightHours)
    {
        throw std::invalid_argument(
            "Maximum flight hours cannot be less than "
            "current total flight hours."
        );
    }

    this->maximumFlightHours = maximumFlightHours;
}

void CrewMember::setIsActive(
    bool isActive
)
{
    this->isActive = isActive;
}