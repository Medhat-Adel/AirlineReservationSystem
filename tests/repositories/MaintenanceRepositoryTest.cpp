#include <iostream>
#include <memory>
#include <vector>

#include "models/Maintenance.h"
#include "repositories/MaintenanceRepository.h"

int main()
{
    try
    {
        std::vector<std::shared_ptr<Maintenance>>
            maintenanceRecords;

        auto maintenance1 =
            std::make_shared<Maintenance>(
                1,
                101,
                "2026-09-10",
                "Engine inspection",
                std::vector<std::string>{
                    "Oil Filter",
                    "Air Filter"
                },
                true
            );

        auto maintenance2 =
            std::make_shared<Maintenance>(
                2,
                102,
                "2026-09-15",
                "Brake system issue",
                std::vector<std::string>{
                    "Brake Pad"
                },
                false
            );

        maintenanceRecords.push_back(
            maintenance1
        );

        maintenanceRecords.push_back(
            maintenance2
        );

        MaintenanceRepository repository;

        std::cout
            << "Saving maintenance records...\n";

        repository.save(
            maintenanceRecords
        );

        std::cout
            << "Maintenance data saved successfully.\n\n";

        maintenanceRecords.clear();

        std::cout
            << "Memory cleared.\n"
            << "Maintenance records in memory: "
            << maintenanceRecords.size()
            << "\n\n";

        maintenanceRecords =
            repository.load();

        std::cout
            << "Maintenance records loaded: "
            << maintenanceRecords.size()
            << "\n\n";

        for (const auto& maintenance :
             maintenanceRecords)
        {
            std::cout
                << "-----------------------------\n";

            std::cout
                << "ID: "
                << maintenance->getId()
                << "\n";

            std::cout
                << "Aircraft ID: "
                << maintenance->getAircraftId()
                << "\n";

            std::cout
                << "Date: "
                << maintenance->getMaintenanceDate()
                << "\n";

            std::cout
                << "Issue: "
                << maintenance->getIssueDescription()
                << "\n";

            std::cout
                << "Replaced Parts:\n";

            for (const auto& part :
                 maintenance->getReplacedParts())
            {
                std::cout
                    << "  - "
                    << part
                    << "\n";
            }

            std::cout
                << "Completed: "
                << (maintenance->isCompleted()
                        ? "Yes"
                        : "No")
                << "\n";

            std::cout
                << "-----------------------------\n\n";
        }

        if (maintenanceRecords.size() != 2)
        {
            throw std::runtime_error(
                "Maintenance persistence failed."
            );
        }

        if (
            maintenanceRecords[0]
                ->getAircraftId()
            != 101
        )
        {
            throw std::runtime_error(
                "Aircraft ID persistence failed."
            );
        }

        if (
            maintenanceRecords[0]
                ->getReplacedParts()
                .size()
            != 2
        )
        {
            throw std::runtime_error(
                "Replaced parts persistence failed."
            );
        }

        if (
            !maintenanceRecords[0]
                ->isCompleted()
        )
        {
            throw std::runtime_error(
                "Completed status persistence failed."
            );
        }

        if (
            maintenanceRecords[1]
                ->isCompleted()
        )
        {
            throw std::runtime_error(
                "Pending status persistence failed."
            );
        }

        std::cout
            << "Maintenance persistence test "
            << "completed successfully!\n";
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "Test failed: "
            << e.what()
            << "\n";

        return 1;
    }

    return 0;
}