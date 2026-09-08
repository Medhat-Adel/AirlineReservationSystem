#include <iostream>
#include <memory>

#include "models/Passenger.h"
#include "models/Aircraft.h"
#include "models/Flight.h"
#include "services/PaymentService.h"
#include "services/BookingService.h"

int main()
{
    std::cout << "========================================\n";
    std::cout << "      MODIFY RESERVATION TEST\n";
    std::cout << "========================================\n\n";

    try
    {
        // Create passenger.
        auto passenger = std::make_shared<Passenger>(
            1,
            "medhat@1001",
            "hashed_password",
            "Medhat Adel",
            "medhat@example.com",
            "01000000000",
            "A12345678"
        );

        // Create aircraft.
        auto aircraft = std::make_shared<Aircraft>(
            1,
            "SU-ABC",
            "Airbus",
            "A320",
            180
        );

        // Create flight.
        auto flight = std::make_shared<Flight>(
            "MS123",
            "Cairo",
            "Dubai",
            "2026-09-10 10:00",
            "2026-09-10 13:00",
            5000.0,
            aircraft,
            3
        );

        PaymentService paymentService;

        BookingService bookingService(
            paymentService
        );

        // ========================================
        // STEP 1: CREATE RESERVATION
        // ========================================

        std::cout << "[1] Creating reservation...\n";

        auto reservation =
            bookingService.createReservation(
                passenger,
                flight,
                "12A",
                "2026-09-07 18:00",
                PaymentMethod::Card
            );

        std::cout
            << "    Reservation ID: "
            << reservation->getId()
            << "\n";

        std::cout
            << "    Initial seat: "
            << reservation->getSeatNumber()
            << "\n";

        // ========================================
        // STEP 2: CHECK INITIAL SEATS
        // ========================================

        std::cout
            << "\n[2] Checking initial seats...\n";

        std::cout
            << "    Seat 12A available: "
            << (flight->isSeatAvailable("12A")
                ? "Yes"
                : "No")
            << "\n";

        std::cout
            << "    Seat 12B available: "
            << (flight->isSeatAvailable("12B")
                ? "Yes"
                : "No")
            << "\n";

        // ========================================
        // STEP 3: MODIFY RESERVATION
        // ========================================

        std::cout
            << "\n[3] Modifying reservation...\n";

        if (bookingService.modifyReservation(
                reservation->getId(),
                "12B"))
        {
            std::cout
                << "    Reservation modified successfully!\n";
        }
        else
        {
            std::cout
                << "    Failed to modify reservation.\n";
        }

        // ========================================
        // STEP 4: CHECK NEW RESERVATION
        // ========================================

        std::cout
            << "\n[4] Checking reservation...\n";

        std::cout
            << "    Current seat: "
            << reservation->getSeatNumber()
            << "\n";

        // ========================================
        // STEP 5: CHECK SEATS AFTER MODIFICATION
        // ========================================

        std::cout
            << "\n[5] Checking seats after modification...\n";

        std::cout
            << "    Seat 12A available: "
            << (flight->isSeatAvailable("12A")
                ? "Yes"
                : "No")
            << "\n";

        std::cout
            << "    Seat 12B available: "
            << (flight->isSeatAvailable("12B")
                ? "Yes"
                : "No")
            << "\n";

        // ========================================
        // STEP 6: CHECK PAYMENT
        // ========================================

        std::cout
            << "\n[6] Checking payment...\n";

        auto payment =
            paymentService.getPaymentByReservationId(
                reservation->getId()
            );

        if (payment != nullptr)
        {
            std::cout
                << "    Payment ID: "
                << payment->getId()
                << "\n";

            std::cout
                << "    Payment status: ";

            if (payment->getStatus() ==
                PaymentStatus::Completed)
            {
                std::cout << "Completed\n";
            }
        }
    }
    catch (const std::exception& exception)
    {
        std::cout
            << "\nERROR: "
            << exception.what()
            << "\n";
    }

    return 0;
}