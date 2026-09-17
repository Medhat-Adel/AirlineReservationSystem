#include <iostream>
#include <memory>
#include <vector>

#include "models/Aircraft.h"
#include "repositories/AircraftRepository.h"

int main()
{
    try
    {
        std::vector<std::shared_ptr<Aircraft>> aircraft;

        aircraft.push_back(
            std::make_shared<Aircraft>(
                1,
                "SU-GAB",
                "Boeing",
                "737-800",
                180,
                AircraftStatus::Available,
                "2026-10-01"
            )
        );

        aircraft.push_back(
            std::make_shared<Aircraft>(
                2,
                "SU-GAC",
                "Airbus",
                "A320",
                186,
                AircraftStatus::Maintenance,
                "2026-09-20"
            )
        );

        AircraftRepository repository;

        std::cout
            << "Saving aircraft data...\n";

        repository.save(aircraft);

        std::cout
            << "Aircraft data saved successfully.\n\n";

        aircraft.clear();

        std::cout
            << "Memory cleared.\n"
            << "Aircraft in memory: "
            << aircraft.size()
            << "\n\n";

        aircraft = repository.load();

        std::cout
            << "Aircraft loaded: "
            << aircraft.size()
            << "\n\n";

        for (const auto& item : aircraft)
        {
            std::cout
                << "-----------------------------\n";

            std::cout
                << "ID: "
                << item->getId()
                << "\n";

            std::cout
                << "Registration: "
                << item->getRegistrationNumber()
                << "\n";

            std::cout
                << "Manufacturer: "
                << item->getManufacturer()
                << "\n";

            std::cout
                << "Model: "
                << item->getModel()
                << "\n";

            std::cout
                << "Capacity: "
                << item->getCapacity()
                << "\n";

            std::cout
                << "Next Maintenance: "
                << item->getNextMaintenanceDate()
                << "\n";

            std::cout
                << "-----------------------------\n\n";
        }

        if (aircraft.size() != 2)
        {
            throw std::runtime_error(
                "Aircraft persistence failed."
            );
        }

        if (
            aircraft[0]->getRegistrationNumber()
            != "SU-GAB"
        )
        {
            throw std::runtime_error(
                "Registration persistence failed."
            );
        }

        if (
            aircraft[0]->getManufacturer()
            != "Boeing"
        )
        {
            throw std::runtime_error(
                "Manufacturer persistence failed."
            );
        }

        if (
            aircraft[0]->getCapacity()
            != 180
        )
        {
            throw std::runtime_error(
                "Capacity persistence failed."
            );
        }

        if (
            aircraft[0]->getStatus()
            != AircraftStatus::Available
        )
        {
            throw std::runtime_error(
                "Aircraft status persistence failed."
            );
        }

        if (
            aircraft[1]->getStatus()
            != AircraftStatus::Maintenance
        )
        {
            throw std::runtime_error(
                "Maintenance status persistence failed."
            );
        }

        std::cout
            << "Aircraft persistence test "
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