#include <iostream>
#include <memory>
#include <vector>

#include "models/Aircraft.h"
#include "models/Flight.h"
#include "models/FlightAttendant.h"
#include "models/Pilot.h"
#include "repositories/FlightRepository.h"

int main()
{
    try
    {
        // ---------------------------------
        // Create Aircraft
        // ---------------------------------

        auto aircraft =
            std::make_shared<Aircraft>(
                1,
                "SU-GAB",
                "Boeing",
                "737-800",
                180
            );

        std::vector<std::shared_ptr<Aircraft>>
            aircraftList = {
                aircraft
            };

        // ---------------------------------
        // Create Crew
        // ---------------------------------

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

        std::vector<std::shared_ptr<CrewMember>>
            crewMembers = {
                pilot,
                flightAttendant
            };

        // ---------------------------------
        // Create Flight
        // ---------------------------------

        auto flight =
            std::make_shared<Flight>(
                "MS123",
                "Cairo",
                "Dubai",
                "2026-09-10 09:00",
                "2026-09-10 12:00",
                5000.0,
                aircraft,
                3
            );

        // ---------------------------------
        // Reserve Seats
        // ---------------------------------

        flight->reserveSeat("12A");
        flight->reserveSeat("12B");

        // ---------------------------------
        // Assign Crew
        // ---------------------------------

        flight->assignCrewMember(pilot);
        flight->assignCrewMember(flightAttendant);

        std::vector<std::shared_ptr<Flight>>
            flights = {
                flight
            };

        // ---------------------------------
        // Save
        // ---------------------------------

        FlightRepository repository;

        std::cout
            << "Saving flights...\n";

        repository.save(flights);

        std::cout
            << "Flight data saved successfully.\n\n";

        // ---------------------------------
        // Clear memory
        // ---------------------------------

        flights.clear();
        aircraftList.clear();
        crewMembers.clear();

        flight.reset();
        aircraft.reset();
        pilot.reset();
        flightAttendant.reset();

        std::cout
            << "Memory cleared.\n"
            << "Flights in memory: "
            << flights.size()
            << "\n";

        // ---------------------------------
        // Recreate related objects
        // ---------------------------------

        auto loadedAircraft =
            std::make_shared<Aircraft>(
                1,
                "SU-GAB",
                "Boeing",
                "737-800",
                180
            );

        auto loadedPilot =
            std::make_shared<Pilot>(
                1,
                "P001",
                "Ahmed Ali",
                100,
                "LIC-12345"
            );

        auto loadedFlightAttendant =
            std::make_shared<FlightAttendant>(
                2,
                "FA001",
                "Sara Mohamed",
                80
            );

        loadedPilot->addFlightHours(3);
        loadedFlightAttendant->addFlightHours(3);

        std::vector<std::shared_ptr<Aircraft>>
            loadedAircraftList = {
                loadedAircraft
            };

        std::vector<std::shared_ptr<CrewMember>>
            loadedCrewMembers = {
                loadedPilot,
                loadedFlightAttendant
            };

        // ---------------------------------
        // Load
        // ---------------------------------

        flights =
            repository.load(
                loadedAircraftList,
                loadedCrewMembers
            );

        std::cout
            << "Flights loaded: "
            << flights.size()
            << "\n\n";

        // ---------------------------------
        // Display
        // ---------------------------------

        for (const auto& loadedFlight :
             flights)
        {
            std::cout
                << "-----------------------------\n";

            std::cout
                << "Flight Number: "
                << loadedFlight->getFlightNumber()
                << "\n";

            std::cout
                << "Origin: "
                << loadedFlight->getOrigin()
                << "\n";

            std::cout
                << "Destination: "
                << loadedFlight->getDestination()
                << "\n";

            std::cout
                << "Price: "
                << loadedFlight->getPrice()
                << "\n";

            std::cout
                << "Duration: "
                << loadedFlight
                       ->getFlightDurationHours()
                << " hours\n";

            std::cout
                << "Occupied Seats: "
                << loadedFlight
                       ->getOccupiedSeats()
                       .size()
                << "\n";

            std::cout
                << "Crew Members: "
                << loadedFlight
                       ->getCrewMembers()
                       .size()
                << "\n";

            std::cout
                << "-----------------------------\n\n";
        }

        // ---------------------------------
        // Validate Flight
        // ---------------------------------

        if (flights.size() != 1)
        {
            throw std::runtime_error(
                "Flight persistence failed."
            );
        }

        auto loadedFlight = flights[0];

        // ---------------------------------
        // Validate Aircraft Relationship
        // ---------------------------------

        if (!loadedFlight->getAircraft())
        {
            throw std::runtime_error(
                "Aircraft relationship failed."
            );
        }

        if (
            loadedFlight
                ->getAircraft()
                ->getId()
            != 1
        )
        {
            throw std::runtime_error(
                "Aircraft ID relationship failed."
            );
        }

        // ---------------------------------
        // Validate Seats
        // ---------------------------------

        if (
            loadedFlight
                ->getOccupiedSeats()
                .size()
            != 2
        )
        {
            throw std::runtime_error(
                "Occupied seats persistence failed."
            );
        }

        if (
            !loadedFlight
                ->getOccupiedSeats()
                .count("12A")
        )
        {
            throw std::runtime_error(
                "Seat 12A persistence failed."
            );
        }

        if (
            !loadedFlight
                ->getOccupiedSeats()
                .count("12B")
        )
        {
            throw std::runtime_error(
                "Seat 12B persistence failed."
            );
        }

        // ---------------------------------
        // Validate Crew Relationship
        // ---------------------------------

        if (
            loadedFlight
                ->getCrewMembers()
                .size()
            != 2
        )
        {
            throw std::runtime_error(
                "Crew relationship persistence failed."
            );
        }

        for (
            const auto& crew :
            loadedFlight->getCrewMembers()
        )
        {
            if (!crew)
            {
                throw std::runtime_error(
                    "Invalid crew relationship."
                );
            }

            std::cout
                << "Crew: "
                << crew->getFullName()
                << " | Role: "
                << crew->getCrewRole()
                << " | Hours: "
                << crew->getTotalFlightHours()
                << "\n";
        }

        // ---------------------------------
        // Validate Crew Hours
        // ---------------------------------

        if (
            loadedPilot->getTotalFlightHours()
            != 3
        )
        {
            throw std::runtime_error(
                "Pilot flight hours relationship failed."
            );
        }

        if (
            loadedFlightAttendant
                ->getTotalFlightHours()
            != 3
        )
        {
            throw std::runtime_error(
                "Flight Attendant hours relationship failed."
            );
        }

        std::cout
            << "\nAircraft Relationship: OK\n";
        std::cout
            << "Crew Relationships: OK\n";

        std::cout
            << "\nFlight relationship persistence test "
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





