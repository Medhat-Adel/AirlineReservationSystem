#ifndef PILOT_H
#define PILOT_H

#include "models/CrewMember.h"

class Pilot : public CrewMember
{
private:
    std::string licenseNumber;

public:
    Pilot(
        int id,
        const std::string& employeeId,
        const std::string& fullName,
        int maximumFlightHours,
        const std::string& licenseNumber,
        bool isActive = true
    );

    const std::string& getLicenseNumber() const;

    std::string getCrewRole() const override;
};

#endif