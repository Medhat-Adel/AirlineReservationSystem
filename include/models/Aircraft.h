#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>

enum class AircraftStatus
{
    Available,
    InFlight,
    Maintenance,
    Unavailable
};

class Aircraft
{
private:
    int id;
    std::string registrationNumber;
    std::string manufacturer;
    std::string model;
    int capacity;
    AircraftStatus status;
    std::string nextMaintenanceDate;

public:
    Aircraft(
        int id,
        const std::string& registrationNumber,
        const std::string& manufacturer,
        const std::string& model,
        int capacity,
        AircraftStatus status = AircraftStatus::Available,
        const std::string& nextMaintenanceDate = ""
    );

    // Getters
    int getId() const;
    const std::string& getRegistrationNumber() const;
    const std::string& getManufacturer() const;
    const std::string& getModel() const;
    int getCapacity() const;
    AircraftStatus getStatus() const;
    const std::string& getNextMaintenanceDate() const;

    // Setters
    void setStatus(AircraftStatus status);
    void setNextMaintenanceDate(const std::string& date);

    // Utility
    bool isAvailable() const;
};

#endif