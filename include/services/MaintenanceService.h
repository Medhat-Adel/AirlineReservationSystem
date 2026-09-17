#ifndef MAINTENANCE_SERVICE_H
#define MAINTENANCE_SERVICE_H

#include <memory>
#include <vector>

#include "models/Aircraft.h"
#include "models/Maintenance.h"

class MaintenanceService
{
private:
    std::vector<std::shared_ptr<Maintenance>> maintenanceRecords;
    int nextMaintenanceId;

public:
    MaintenanceService();

    std::shared_ptr<Maintenance> scheduleMaintenance(
        const std::shared_ptr<Aircraft>& aircraft,
        const std::string& maintenanceDate,
        const std::string& issueDescription
    );

    bool completeMaintenance(
        int maintenanceId,
        const std::shared_ptr<Aircraft>& aircraft
    );

    bool addReplacedPart(
        int maintenanceId,
        const std::string& part
    );

    std::shared_ptr<Maintenance> getMaintenance(
        int maintenanceId
    ) const;

    std::vector<std::shared_ptr<Maintenance>>
    getAircraftMaintenanceHistory(
        int aircraftId
    ) const;

    const std::vector<std::shared_ptr<Maintenance>>&
    getAllMaintenanceRecords() const;
};

#endif