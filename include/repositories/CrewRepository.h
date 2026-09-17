#ifndef CREW_REPOSITORY_H
#define CREW_REPOSITORY_H

#include <memory>
#include <string>
#include <vector>

#include "models/CrewMember.h"

class CrewRepository
{
private:
    std::string filePath;

public:
    explicit CrewRepository(
        const std::string& filePath = "data/crew.json"
    );

    void save(
        const std::vector<std::shared_ptr<CrewMember>>& crewMembers
    ) const;

    std::vector<std::shared_ptr<CrewMember>> load() const;
};

#endif