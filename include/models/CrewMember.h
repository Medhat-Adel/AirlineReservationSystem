#ifndef CREW_MEMBER_H
#define CREW_MEMBER_H

#include <string>

class CrewMember
{
protected:
    int id;
    std::string employeeId;
    std::string fullName;

    int totalFlightHours;
    int maximumFlightHours;

    bool isActive;

public:
    CrewMember(
        int id,
        const std::string& employeeId,
        const std::string& fullName,
        int maximumFlightHours,
        bool isActive = true
    );

    virtual ~CrewMember() = default;

    int getId() const;
    const std::string& getEmployeeId() const;
    const std::string& getFullName() const;

    int getTotalFlightHours() const;
    int getMaximumFlightHours() const;

    bool getIsActive() const;

    bool canAssignFlight(int flightHours) const;

    void addFlightHours(int flightHours);
    void subtractFlightHours(int flightHours);

    virtual std::string getCrewRole() const = 0;
};

#endif