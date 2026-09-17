#ifndef MAINTENANCE_REPOSITORY_H
#define MAINTENANCE_REPOSITORY_H

#include <memory>
#include <string>
#include <vector>

#include "models/Maintenance.h"

class MaintenanceRepository
{
private:
    std::string filePath;

public:
    explicit MaintenanceRepository(
        const std::string& filePath = "data/maintenance.json"
    );

    void save(
        const std::vector<std::shared_ptr<Maintenance>>&
            maintenanceRecords
    ) const;

    std::vector<std::shared_ptr<Maintenance>>
    load() const;
};

#endif