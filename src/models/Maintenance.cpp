#include "models/Maintenance.h"

#include <stdexcept>

Maintenance::Maintenance(
    int id,
    int aircraftId,
    const std::string& maintenanceDate,
    const std::string& issueDescription,
    const std::vector<std::string>& replacedParts,
    bool completed
)
    : id(id),
      aircraftId(aircraftId),
      maintenanceDate(maintenanceDate),
      issueDescription(issueDescription),
      replacedParts(replacedParts),
      completed(completed)
{
    if (id <= 0)
    {
        throw std::invalid_argument(
            "Maintenance ID must be positive."
        );
    }

    if (aircraftId <= 0)
    {
        throw std::invalid_argument(
            "Aircraft ID must be positive."
        );
    }

    if (maintenanceDate.empty())
    {
        throw std::invalid_argument(
            "Maintenance date cannot be empty."
        );
    }

    if (issueDescription.empty())
    {
        throw std::invalid_argument(
            "Issue description cannot be empty."
        );
    }

    for (const auto& part : replacedParts)
    {
        if (part.empty())
        {
            throw std::invalid_argument(
                "Replaced part name cannot be empty."
            );
        }
    }
}

int Maintenance::getId() const
{
    return id;
}

int Maintenance::getAircraftId() const
{
    return aircraftId;
}

const std::string&
Maintenance::getMaintenanceDate() const
{
    return maintenanceDate;
}

const std::string&
Maintenance::getIssueDescription() const
{
    return issueDescription;
}

const std::vector<std::string>&
Maintenance::getReplacedParts() const
{
    return replacedParts;
}

bool Maintenance::isCompleted() const
{
    return completed;
}

void Maintenance::addReplacedPart(
    const std::string& part
)
{
    if (part.empty())
    {
        throw std::invalid_argument(
            "Replaced part name cannot be empty."
        );
    }

    replacedParts.push_back(part);
}

void Maintenance::setCompleted(
    bool completed
)
{
    this->completed = completed;
}