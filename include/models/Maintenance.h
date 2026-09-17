#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include <string>
#include <vector>

class Maintenance
{
private:
    int id;
    int aircraftId;

    std::string maintenanceDate;
    std::string issueDescription;

    std::vector<std::string> replacedParts;

    bool completed;

public:
    Maintenance(
        int id,
        int aircraftId,
        const std::string& maintenanceDate,
        const std::string& issueDescription,
        const std::vector<std::string>& replacedParts = {},
        bool completed = false
    );

    int getId() const;
    int getAircraftId() const;

    const std::string& getMaintenanceDate() const;
    const std::string& getIssueDescription() const;

    const std::vector<std::string>&
    getReplacedParts() const;

    bool isCompleted() const;

    void addReplacedPart(
        const std::string& part
    );

    void setCompleted(bool completed);
};

#endif