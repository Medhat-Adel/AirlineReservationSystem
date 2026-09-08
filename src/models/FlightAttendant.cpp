#include "models/FlightAttendant.h"

FlightAttendant::FlightAttendant(
    int id,
    const std::string& employeeId,
    const std::string& fullName,
    int maximumFlightHours,
    bool isActive
)
    : CrewMember(
          id,
          employeeId,
          fullName,
          maximumFlightHours,
          isActive
      )
{
}

std::string FlightAttendant::getCrewRole() const
{
    return "Flight Attendant";
}