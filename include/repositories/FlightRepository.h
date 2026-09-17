#ifndef FLIGHT_REPOSITORY_H
#define FLIGHT_REPOSITORY_H

#include <memory>
#include <string>
#include <vector>


#include "models/Flight.h"

class FlightRepository
{
private:
    std::string filePath;

public:
    explicit FlightRepository(
        const std::string& filePath = "data/flights.json"
    );

    void save(
        const std::vector<std::shared_ptr<Flight>>& flights
    ) const;

    std::vector<std::shared_ptr<Flight>> load(
        const std::vector<std::shared_ptr<Aircraft>>& aircraft,
        const std::vector<std::shared_ptr<CrewMember>>& crewMembers
    ) const;
};

#endif