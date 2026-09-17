#include "repositories/MaintenanceRepository.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

MaintenanceRepository::MaintenanceRepository(
    const std::string& filePath
)
    : filePath(filePath)
{
}

void MaintenanceRepository::save(
    const std::vector<std::shared_ptr<Maintenance>>&
        maintenanceRecords
) const
{
    std::filesystem::path path(filePath);

    if (!path.parent_path().empty())
    {
        std::filesystem::create_directories(
            path.parent_path()
        );
    }

    json data = json::array();

    for (const auto& maintenance : maintenanceRecords)
    {
        if (!maintenance)
        {
            continue;
        }

        json item = {
            {"id", maintenance->getId()},
            {"aircraftId",
             maintenance->getAircraftId()},
            {"maintenanceDate",
             maintenance->getMaintenanceDate()},
            {"issueDescription",
             maintenance->getIssueDescription()},
            {"replacedParts",
             maintenance->getReplacedParts()},
            {"completed",
             maintenance->isCompleted()}
        };

        data.push_back(item);
    }

    std::ofstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open maintenance file for writing: "
            + filePath
        );
    }

    file << data.dump(4);

    if (!file)
    {
        throw std::runtime_error(
            "Failed to write maintenance data."
        );
    }
}

std::vector<std::shared_ptr<Maintenance>>
MaintenanceRepository::load() const
{
    std::vector<std::shared_ptr<Maintenance>>
        maintenanceRecords;

    if (!std::filesystem::exists(filePath))
    {
        return maintenanceRecords;
    }

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open maintenance file for reading: "
            + filePath
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
            std::string(
                "Invalid maintenance JSON file: "
            )
            + e.what()
        );
    }

    if (!data.is_array())
    {
        throw std::runtime_error(
            "Maintenance JSON root must be an array."
        );
    }

    for (const auto& item : data)
    {
        try
        {
            int id =
                item.at("id").get<int>();

            int aircraftId =
                item.at("aircraftId").get<int>();

            std::string maintenanceDate =
                item.at("maintenanceDate")
                    .get<std::string>();

            std::string issueDescription =
                item.at("issueDescription")
                    .get<std::string>();

            std::vector<std::string> replacedParts =
                item.at("replacedParts")
                    .get<std::vector<std::string>>();

            bool completed =
                item.at("completed").get<bool>();

            auto maintenance =
                std::make_shared<Maintenance>(
                    id,
                    aircraftId,
                    maintenanceDate,
                    issueDescription,
                    replacedParts,
                    completed
                );

            maintenanceRecords.push_back(
                maintenance
            );
        }
        catch (const json::exception& e)
        {
            throw std::runtime_error(
                std::string(
                    "Invalid maintenance data: "
                )
                + e.what()
            );
        }
    }

    return maintenanceRecords;
}