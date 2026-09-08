#include "models/Aircraft.h"

Aircraft::Aircraft(
    int id,
    const std::string& registrationNumber,
    const std::string& manufacturer,
    const std::string& model,
    int capacity,
    AircraftStatus status,
    const std::string& nextMaintenanceDate
)
    : id(id),
      registrationNumber(registrationNumber),
      manufacturer(manufacturer),
      model(model),
      capacity(capacity),
      status(status),
      nextMaintenanceDate(nextMaintenanceDate)
{
}

// Getters

int Aircraft::getId() const
{
    return id;
}

const std::string& Aircraft::getRegistrationNumber() const
{
    return registrationNumber;
}

const std::string& Aircraft::getManufacturer() const
{
    return manufacturer;
}

const std::string& Aircraft::getModel() const
{
    return model;
}

int Aircraft::getCapacity() const
{
    return capacity;
}

AircraftStatus Aircraft::getStatus() const
{
    return status;
}

const std::string& Aircraft::getNextMaintenanceDate() const
{
    return nextMaintenanceDate;
}

// Setters

void Aircraft::setStatus(AircraftStatus status)
{
    this->status = status;
}

void Aircraft::setNextMaintenanceDate(const std::string& date)
{
    nextMaintenanceDate = date;
}

bool Aircraft::isAvailable() const
{
    return status == AircraftStatus::Available;
}