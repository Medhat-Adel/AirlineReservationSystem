#ifndef AIRCRAFT_REPOSITORY_H
#define AIRCRAFT_REPOSITORY_H

#include <memory>
#include <string>
#include <vector>

#include "models/Aircraft.h"

class AircraftRepository
{
private:
    std::string filePath;

public:
    explicit AircraftRepository(
        const std::string& filePath = "data/aircraft.json"
    );

    void save(
        const std::vector<std::shared_ptr<Aircraft>>& aircraft
    ) const;

    std::vector<std::shared_ptr<Aircraft>> load() const;
};

#endif