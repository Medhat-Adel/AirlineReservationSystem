#include "repositories/CrewRepository.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "models/FlightAttendant.h"
#include "models/Pilot.h"

using json = nlohmann::json;

namespace
{
    std::string crewRoleToString(
        const std::shared_ptr<CrewMember>& crewMember
    )
    {
        if (!crewMember)
        {
            throw std::runtime_error(
                "Cannot determine role of null crew member."
            );
        }

        return crewMember->getCrewRole();
    }
}

CrewRepository::CrewRepository(
    const std::string& filePath
)
    : filePath(filePath)
{
}

void CrewRepository::save(
    const std::vector<std::shared_ptr<CrewMember>>& crewMembers
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

    for (const auto& crewMember : crewMembers)
    {
        if (!crewMember)
        {
            continue;
        }

        json item = {
            {"id", crewMember->getId()},
            {"employeeId", crewMember->getEmployeeId()},
            {"fullName", crewMember->getFullName()},
            {"totalFlightHours", crewMember->getTotalFlightHours()},
            {"maximumFlightHours", crewMember->getMaximumFlightHours()},
            {"isActive", crewMember->getIsActive()},
            {"role", crewRoleToString(crewMember)}
        };

        // Pilot has additional license information.
        auto pilot =
            std::dynamic_pointer_cast<Pilot>(crewMember);

        if (pilot)
        {
            item["licenseNumber"] =
                pilot->getLicenseNumber();
        }

        data.push_back(item);
    }

    std::ofstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open crew file for writing: "
            + filePath
        );
    }

    file << data.dump(4);

    if (!file)
    {
        throw std::runtime_error(
            "Failed to write crew data."
        );
    }
}

std::vector<std::shared_ptr<CrewMember>>
CrewRepository::load() const
{
    std::vector<std::shared_ptr<CrewMember>> crewMembers;

    if (!std::filesystem::exists(filePath))
    {
        return crewMembers;
    }

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open crew file for reading: "
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
            std::string("Invalid crew JSON file: ")
            + e.what()
        );
    }

    if (!data.is_array())
    {
        throw std::runtime_error(
            "Crew JSON root must be an array."
        );
    }

    for (const auto& item : data)
    {
        try
        {
            int id =
                item.at("id").get<int>();

            std::string employeeId =
                item.at("employeeId").get<std::string>();

            std::string fullName =
                item.at("fullName").get<std::string>();

            int totalFlightHours =
                item.at("totalFlightHours").get<int>();

            int maximumFlightHours =
                item.at("maximumFlightHours").get<int>();

            bool isActive =
                item.at("isActive").get<bool>();

            std::string role =
                item.at("role").get<std::string>();

            std::shared_ptr<CrewMember> crewMember;

            if (role == "Pilot")
            {
                std::string licenseNumber =
                    item.at("licenseNumber")
                        .get<std::string>();

                auto pilot = std::make_shared<Pilot>(
                    id,
                    employeeId,
                    fullName,
                    maximumFlightHours,
                    licenseNumber,
                    isActive
                );

                // Restore accumulated flight hours.
                if (totalFlightHours > 0)
                {
                    pilot->addFlightHours(
                        totalFlightHours
                    );
                }

                crewMember = pilot;
            }
            else if (role == "Flight Attendant")
            {
                auto flightAttendant =
                    std::make_shared<FlightAttendant>(
                        id,
                        employeeId,
                        fullName,
                        maximumFlightHours,
                        isActive
                    );

                // Restore accumulated flight hours.
                if (totalFlightHours > 0)
                {
                    flightAttendant->addFlightHours(
                        totalFlightHours
                    );
                }

                crewMember = flightAttendant;
            }
            else
            {
                throw std::runtime_error(
                    "Unknown crew role: " + role
                );
            }

            crewMembers.push_back(crewMember);
        }
        catch (const json::exception& e)
        {
            throw std::runtime_error(
                std::string("Invalid crew data: ")
                + e.what()
            );
        }
    }

    return crewMembers;
}