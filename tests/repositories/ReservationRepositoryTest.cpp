#include <iostream>
#include <memory>
#include <vector>

#include "models/Aircraft.h"
#include "models/Flight.h"
#include "models/Passenger.h"
#include "models/Reservation.h"
#include "repositories/ReservationRepository.h"

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
        // Create Passengers
        // ---------------------------------

        auto passenger1 =
            std::make_shared<Passenger>(
                1,
                "medhat",
                "hash123",
                "Medhat Adel",
                "medhat@example.com",
                "01000000000",
                "P12345678",
                150
            );

        auto passenger2 =
            std::make_shared<Passenger>(
                2,
                "ahmed",
                "hash456",
                "Ahmed Ali",
                "ahmed@example.com",
                "01111111111",
                "P87654321",
                50
            );

        // ---------------------------------
        // Reserve seats
        // ---------------------------------

        flight->reserveSeat("12A");
        flight->reserveSeat("12B");

        // ---------------------------------
        // Create Reservations
        // ---------------------------------

        std::vector<std::shared_ptr<Reservation>>
            reservations;

        reservations.push_back(
            std::make_shared<Reservation>(
                1,
                passenger1,
                flight,
                "12A",
                "2026-09-09",
                5000.0,
                PaymentMethod::Card,
                ReservationStatus::Confirmed
            )
        );

        reservations.push_back(
            std::make_shared<Reservation>(
                2,
                passenger2,
                flight,
                "12B",
                "2026-09-09",
                5000.0,
                PaymentMethod::Cash,
                ReservationStatus::Cancelled
            )
        );

        // ---------------------------------
        // Save
        // ---------------------------------

        ReservationRepository repository;

        std::cout
            << "Saving reservations...\n";

        repository.save(reservations);

        std::cout
            << "Reservations saved successfully.\n\n";

        // ---------------------------------
        // Clear memory
        // ---------------------------------

        reservations.clear();

        std::cout
            << "Memory cleared.\n"
            << "Reservations in memory: "
            << reservations.size()
            << "\n\n";

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

        auto loadedFlight =
            std::make_shared<Flight>(
                "MS123",
                "Cairo",
                "Dubai",
                "2026-09-10 09:00",
                "2026-09-10 12:00",
                5000.0,
                loadedAircraft,
                3
            );

        loadedFlight->reserveSeat("12A");
        loadedFlight->reserveSeat("12B");

        auto loadedPassenger1 =
            std::make_shared<Passenger>(
                1,
                "medhat",
                "hash123",
                "Medhat Adel",
                "medhat@example.com",
                "01000000000",
                "P12345678",
                150
            );

        auto loadedPassenger2 =
            std::make_shared<Passenger>(
                2,
                "ahmed",
                "hash456",
                "Ahmed Ali",
                "ahmed@example.com",
                "01111111111",
                "P87654321",
                50
            );

        std::vector<std::shared_ptr<Passenger>>
            passengers = {
                loadedPassenger1,
                loadedPassenger2
            };

        std::vector<std::shared_ptr<Flight>>
            flights = {
                loadedFlight
            };

        // ---------------------------------
        // Load
        // ---------------------------------

        reservations =
            repository.load(
                passengers,
                flights
            );

        std::cout
            << "Reservations loaded: "
            << reservations.size()
            << "\n\n";

        // ---------------------------------
        // Display
        // ---------------------------------

        for (const auto& reservation :
             reservations)
        {
            std::cout
                << "-----------------------------\n";

            std::cout
                << "Reservation ID: "
                << reservation->getId()
                << "\n";

            std::cout
                << "Passenger: "
                << reservation
                       ->getPassenger()
                       ->getFullName()
                << "\n";

            std::cout
                << "Flight: "
                << reservation
                       ->getFlight()
                       ->getFlightNumber()
                << "\n";

            std::cout
                << "Seat: "
                << reservation->getSeatNumber()
                << "\n";

            std::cout
                << "Booking Date: "
                << reservation->getBookingDate()
                << "\n";

            std::cout
                << "Total Price: "
                << reservation->getTotalPrice()
                << "\n";

            std::cout
                << "-----------------------------\n\n";
        }

        // ---------------------------------
        // Validate
        // ---------------------------------

        if (reservations.size() != 2)
        {
            throw std::runtime_error(
                "Reservation persistence failed."
            );
        }

        if (
            reservations[0]
                ->getPassenger()
                ->getId()
            != 1
        )
        {
            throw std::runtime_error(
                "Passenger relationship failed."
            );
        }

        if (
            reservations[0]
                ->getFlight()
                ->getFlightNumber()
            != "MS123"
        )
        {
            throw std::runtime_error(
                "Flight relationship failed."
            );
        }

        if (
            reservations[0]
                ->getSeatNumber()
            != "12A"
        )
        {
            throw std::runtime_error(
                "Seat persistence failed."
            );
        }

        if (
            reservations[0]
                ->getStatus()
            != ReservationStatus::Confirmed
        )
        {
            throw std::runtime_error(
                "Reservation status persistence failed."
            );
        }

        if (
            reservations[1]
                ->getStatus()
            != ReservationStatus::Cancelled
        )
        {
            throw std::runtime_error(
                "Cancelled status persistence failed."
            );
        }

        std::cout
            << "Reservation persistence test "
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