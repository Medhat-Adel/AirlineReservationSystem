#ifndef FLIGHT_ATTENDANT_H
#define FLIGHT_ATTENDANT_H

#include "models/CrewMember.h"

class FlightAttendant : public CrewMember
{
public:
    FlightAttendant(
        int id,
        const std::string& employeeId,
        const std::string& fullName,
        int maximumFlightHours,
        bool isActive = true
    );

    std::string getCrewRole() const override;
};

#endif