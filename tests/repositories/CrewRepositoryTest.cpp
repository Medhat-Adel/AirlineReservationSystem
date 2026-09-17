#include <iostream>
#include <memory>
#include <vector>

#include "models/FlightAttendant.h"
#include "models/Pilot.h"
#include "repositories/CrewRepository.h"

int main()
{
    try
    {
        std::vector<std::shared_ptr<CrewMember>> crewMembers;

        auto pilot =
            std::make_shared<Pilot>(
                1,
                "P001",
                "Ahmed Ali",
                100,
                "LIC-12345"
            );

        auto flightAttendant =
            std::make_shared<FlightAttendant>(
                2,
                "FA001",
                "Sara Mohamed",
                80
            );

        pilot->addFlightHours(30);
        flightAttendant->addFlightHours(20);

        crewMembers.push_back(pilot);
        crewMembers.push_back(flightAttendant);

        CrewRepository repository;

        std::cout
            << "Saving crew...\n";

        repository.save(crewMembers);

        std::cout
            << "Crew data saved successfully.\n\n";

        crewMembers.clear();

        std::cout
            << "Memory cleared.\n"
            << "Crew in memory: "
            << crewMembers.size()
            << "\n\n";

        crewMembers = repository.load();

        std::cout
            << "Crew loaded: "
            << crewMembers.size()
            << "\n\n";

        for (const auto& crew : crewMembers)
        {
            std::cout
                << "-----------------------------\n";

            std::cout
                << "ID: "
                << crew->getId()
                << "\n";

            std::cout
                << "Employee ID: "
                << crew->getEmployeeId()
                << "\n";

            std::cout
                << "Name: "
                << crew->getFullName()
                << "\n";

            std::cout
                << "Role: "
                << crew->getCrewRole()
                << "\n";

            std::cout
                << "Flight Hours: "
                << crew->getTotalFlightHours()
                << "\n";

            std::cout
                << "Maximum Hours: "
                << crew->getMaximumFlightHours()
                << "\n";

            if (auto loadedPilot =
                    std::dynamic_pointer_cast<Pilot>(crew))
            {
                std::cout
                    << "License Number: "
                    << loadedPilot->getLicenseNumber()
                    << "\n";
            }

            std::cout
                << "-----------------------------\n\n";
        }

        if (crewMembers.size() != 2)
        {
            throw std::runtime_error(
                "Crew persistence failed."
            );
        }

        auto loadedPilot =
            std::dynamic_pointer_cast<Pilot>(
                crewMembers[0]
            );

        if (!loadedPilot)
        {
            throw std::runtime_error(
                "Pilot polymorphism failed."
            );
        }

        if (
            loadedPilot->getTotalFlightHours()
            != 30
        )
        {
            throw std::runtime_error(
                "Pilot flight hours persistence failed."
            );
        }

        if (
            loadedPilot->getLicenseNumber()
            != "LIC-12345"
        )
        {
            throw std::runtime_error(
                "Pilot license persistence failed."
            );
        }

        auto loadedFlightAttendant =
            std::dynamic_pointer_cast<FlightAttendant>(
                crewMembers[1]
            );

        if (!loadedFlightAttendant)
        {
            throw std::runtime_error(
                "Flight Attendant polymorphism failed."
            );
        }

        if (
            loadedFlightAttendant->getTotalFlightHours()
            != 20
        )
        {
            throw std::runtime_error(
                "Flight Attendant hours persistence failed."
            );
        }

        std::cout
            << "Crew persistence test "
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