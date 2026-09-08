#include "models/Pilot.h"

#include <stdexcept>

Pilot::Pilot(
    int id,
    const std::string& employeeId,
    const std::string& fullName,
    int maximumFlightHours,
    const std::string& licenseNumber,
    bool isActive
)
    : CrewMember(
          id,
          employeeId,
          fullName,
          maximumFlightHours,
          isActive
      ),
      licenseNumber(licenseNumber)
{
    if (licenseNumber.empty())
    {
        throw std::invalid_argument(
            "Pilot license number cannot be empty."
        );
    }
}

const std::string& Pilot::getLicenseNumber() const
{
    return licenseNumber;
}

std::string Pilot::getCrewRole() const
{
    return "Pilot";
}