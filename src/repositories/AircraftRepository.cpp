#include "repositories/AircraftRepository.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace
{
    std::string aircraftStatusToString(AircraftStatus status)
    {
        switch (status)
        {
        case AircraftStatus::Available:
            return "Available";

        case AircraftStatus::InFlight:
            return "InFlight";

        case AircraftStatus::Maintenance:
            return "Maintenance";

        case AircraftStatus::Unavailable:
            return "Unavailable";
        }

        throw std::runtime_error("Invalid aircraft status.");
    }

    AircraftStatus stringToAircraftStatus(const std::string& status)
    {
        if (status == "Available")
            return AircraftStatus::Available;

        if (status == "InFlight")
            return AircraftStatus::InFlight;

        if (status == "Maintenance")
            return AircraftStatus::Maintenance;

        if (status == "Unavailable")
            return AircraftStatus::Unavailable;

        throw std::runtime_error("Invalid aircraft status: " + status);
    }
}

AircraftRepository::AircraftRepository(
    const std::string& filePath
)
    : filePath(filePath)
{
}

void AircraftRepository::save(
    const std::vector<std::shared_ptr<Aircraft>>& aircraft
) const
{
    std::filesystem::path path(filePath);

    if (!path.parent_path().empty())
    {
        std::filesystem::create_directories(path.parent_path());
    }

    json data = json::array();

    for (const auto& aircraftItem : aircraft)
    {
        if (!aircraftItem)
        {
            continue;
        }

        data.push_back({
            {"id", aircraftItem->getId()},
            {"registrationNumber", aircraftItem->getRegistrationNumber()},
            {"manufacturer", aircraftItem->getManufacturer()},
            {"model", aircraftItem->getModel()},
            {"capacity", aircraftItem->getCapacity()},
            {"status", aircraftStatusToString(aircraftItem->getStatus())},
            {"nextMaintenanceDate", aircraftItem->getNextMaintenanceDate()}
        });
    }

    std::ofstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open aircraft file for writing: " + filePath
        );
    }

    file << data.dump(4);

    if (!file)
    {
        throw std::runtime_error(
            "Failed to write aircraft data."
        );
    }
}

std::vector<std::shared_ptr<Aircraft>>
AircraftRepository::load() const
{
    std::vector<std::shared_ptr<Aircraft>> aircraft;

    if (!std::filesystem::exists(filePath))
    {
        return aircraft;
    }

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open aircraft file for reading: " + filePath
        );
    }

    json data;

    try
    {
        file >> data;
    }
    catch (const json::parse_error& e)
    {
        throw std::runtime_error(
            std::string("Invalid aircraft JSON file: ") + e.what()
        );
    }

    if (!data.is_array())
    {
        throw std::runtime_error(
            "Aircraft JSON root must be an array."
        );
    }

    for (const auto& item : data)
    {
        try
        {
            int id = item.at("id").get<int>();
            std::string registrationNumber =
                item.at("registrationNumber").get<std::string>();

            std::string manufacturer =
                item.at("manufacturer").get<std::string>();

            std::string model =
                item.at("model").get<std::string>();

            int capacity =
                item.at("capacity").get<int>();

            AircraftStatus status =
                stringToAircraftStatus(
                    item.at("status").get<std::string>()
                );

            std::string nextMaintenanceDate =
                item.at("nextMaintenanceDate").get<std::string>();

            aircraft.push_back(
                std::make_shared<Aircraft>(
                    id,
                    registrationNumber,
                    manufacturer,
                    model,
                    capacity,
                    status,
                    nextMaintenanceDate
                )
            );
        }
        catch (const json::exception& e)
        {
            throw std::runtime_error(
                std::string("Invalid aircraft data: ") + e.what()
            );
        }
    }

    return aircraft;
}