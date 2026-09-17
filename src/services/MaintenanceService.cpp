#include "services/MaintenanceService.h"

#include <stdexcept>

MaintenanceService::MaintenanceService()
    : nextMaintenanceId(1)
{
}

std::shared_ptr<Maintenance>
MaintenanceService::scheduleMaintenance(
    const std::shared_ptr<Aircraft>& aircraft,
    const std::string& maintenanceDate,
    const std::string& issueDescription
)
{
    if (aircraft == nullptr)
    {
        throw std::invalid_argument(
            "Aircraft cannot be null."
        );
    }

    if (aircraft->getStatus() ==
        AircraftStatus::Maintenance)
    {
        throw std::invalid_argument(
            "Aircraft is already under maintenance."
        );
    }

    if (aircraft->getStatus() ==
        AircraftStatus::InFlight)
    {
        throw std::invalid_argument(
            "Cannot schedule maintenance for an aircraft in flight."
        );
    }

    auto maintenance =
        std::make_shared<Maintenance>(
            nextMaintenanceId++,
            aircraft->getId(),
            maintenanceDate,
            issueDescription
        );

    aircraft->setStatus(
        AircraftStatus::Maintenance
    );

    maintenanceRecords.push_back(maintenance);

    return maintenance;
}

bool MaintenanceService::completeMaintenance(
    int maintenanceId,
    const std::shared_ptr<Aircraft>& aircraft
)
{
    auto maintenance = getMaintenance(
        maintenanceId
    );

    if (maintenance == nullptr ||
        aircraft == nullptr)
    {
        return false;
    }

    if (maintenance->getAircraftId() !=
        aircraft->getId())
    {
        return false;
    }

    if (maintenance->isCompleted())
    {
        return false;
    }

    maintenance->setCompleted(true);

    aircraft->setStatus(
        AircraftStatus::Available
    );

    return true;
}

bool MaintenanceService::addReplacedPart(
    int maintenanceId,
    const std::string& part
)
{
    auto maintenance = getMaintenance(
        maintenanceId
    );

    if (maintenance == nullptr ||
        maintenance->isCompleted())
    {
        return false;
    }

    maintenance->addReplacedPart(part);

    return true;
}

std::shared_ptr<Maintenance>
MaintenanceService::getMaintenance(
    int maintenanceId
) const
{
    for (const auto& maintenance :
         maintenanceRecords)
    {
        if (maintenance->getId() ==
            maintenanceId)
        {
            return maintenance;
        }
    }

    return nullptr;
}

std::vector<std::shared_ptr<Maintenance>>
MaintenanceService::getAircraftMaintenanceHistory(
    int aircraftId
) const
{
    std::vector<std::shared_ptr<Maintenance>>
        history;

    for (const auto& maintenance :
         maintenanceRecords)
    {
        if (maintenance->getAircraftId() ==
            aircraftId)
        {
            history.push_back(maintenance);
        }
    }

    return history;
}

const std::vector<std::shared_ptr<Maintenance>>&
MaintenanceService::getAllMaintenanceRecords() const
{
    return maintenanceRecords;
}