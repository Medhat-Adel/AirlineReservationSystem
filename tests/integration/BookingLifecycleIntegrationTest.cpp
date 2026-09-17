#include <cassert>
#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>

#include "models/Aircraft.h"
#include "models/Flight.h"
#include "models/Passenger.h"

#include "models/Payment.h"
#include "models/Reservation.h"

#include "services/BookingService.h"
#include "services/LoyaltyService.h"
#include "services/PaymentService.h"

#include "repositories/FlightRepository.h"
#include "repositories/PaymentRepository.h"
#include "repositories/ReservationRepository.h"
#include "repositories/AircraftRepository.h"

int main()
{
    const std::string aircraftFile =
        "data/integration_aircraft_test.json";

    const std::string flightFile =
        "data/integration_flights_test.json";

    const std::string reservationFile =
        "data/integration_reservations_test.json";

    const std::string paymentFile =
        "data/integration_payments_test.json";

    try
    {
        // =================================================
        // CLEAN OLD TEST DATA
        // =================================================

        std::filesystem::remove(aircraftFile);
        std::filesystem::remove(flightFile);
        std::filesystem::remove(reservationFile);
        std::filesystem::remove(paymentFile);

        // =================================================
        // STEP 1: CREATE AIRCRAFT
        // =================================================

        std::cout
            << "\n[1] Creating aircraft...\n";

        auto aircraft =
            std::make_shared<Aircraft>(
                1,
                "SU-INT",
                "Boeing",
                "737-800",
                180,
                AircraftStatus::Available,
                "2026-12-01"
            );

        std::vector<std::shared_ptr<Aircraft>> aircraftList{
            aircraft
        };

        std::cout
            << "Aircraft created successfully.\n";

        // =================================================
        // STEP 2: CREATE FLIGHT
        // =================================================

        std::cout
            << "\n[2] Creating flight...\n";

        auto flight =
            std::make_shared<Flight>(
                "MS999",
                "Cairo",
                "Dubai",
                "2026-09-20 10:00",
                "2026-09-20 13:00",
                5000.0,
                aircraft,
                3,
                FlightStatus::Scheduled
            );

        std::vector<std::shared_ptr<Flight>> flights{
            flight
        };

        std::cout
            << "Flight created successfully.\n";

        // =================================================
        // STEP 3: CREATE PASSENGER
        // =================================================

        std::cout
            << "\n[3] Creating passenger...\n";

        auto passenger =
            std::make_shared<Passenger>(
                1,
                "booking_passenger",
                "test_hash",
                "Booking Passenger",
                "booking@test.com",
                "01000000000",
                "P999999",
                0
            );

        passenger->setPreferredSeat("12A");
        passenger->setMealPreference("Vegetarian");

        std::cout
            << "Passenger created successfully.\n";

        // =================================================
        // STEP 4: CREATE SERVICES
        // =================================================

        std::cout
            << "\n[4] Creating booking services...\n";

        PaymentService paymentService;

        LoyaltyService loyaltyService;

        BookingService bookingService(
            paymentService,
            loyaltyService
        );

        std::cout
            << "Booking services created successfully.\n";

        // =================================================
        // STEP 5: CREATE RESERVATION
        // =================================================

        std::cout
            << "\n[5] Creating reservation...\n";

        auto reservation =
            bookingService.createReservation(
                passenger,
                flight,
                "12A",
                "2026-09-17",
                PaymentMethod::Card
            );

        assert(reservation != nullptr);
        
        const int reservationId =
            reservation->getId();
        
        std::cout
            << "Reservation created successfully.\n";

        // =================================================
        // STEP 6: VERIFY RESERVATION
        // =================================================

        std::cout
            << "\n[6] Verifying reservation...\n";

        assert(
            reservation->getPassenger()
            == passenger
        );

        assert(
            reservation->getFlight()
            == flight
        );

        assert(
            reservation->getSeatNumber()
            == "12A"
        );

        assert(
            reservation->getTotalPrice()
            == 5000.0
        );

        assert(
            reservation->getPaymentMethod()
            == PaymentMethod::Card
        );

        assert(
            reservation->getStatus()
            == ReservationStatus::Confirmed
        );

        std::cout
            << "Reservation verification PASSED.\n";

        // =================================================
        // STEP 7: VERIFY SEAT
        // =================================================

        std::cout
            << "\n[7] Verifying seat reservation...\n";

        assert(
            !flight->isSeatAvailable("12A")
        );

        assert(
            flight->getOccupiedSeats().count("12A")
            == 1
        );

        std::cout
            << "Seat reservation verification PASSED.\n";

        // =================================================
        // STEP 8: VERIFY PAYMENT
        // =================================================

        std::cout
            << "\n[8] Verifying payment...\n";

        auto payment =
            paymentService.getPaymentByReservationId(
                reservation->getId()
            );

        assert(payment != nullptr);

        assert(
            payment->getReservationId()
            == reservation->getId()
        );

        assert(
            payment->getAmount()
            == 5000.0
        );

        assert(
            payment->getMethod()
            == PaymentMethod::Card
        );

        assert(
            payment->getStatus()
            == PaymentStatus::Completed
        );

        std::cout
            << "Payment verification PASSED.\n";

        // =================================================
        // STEP 9: VERIFY LOYALTY POINTS
        // =================================================

        std::cout
            << "\n[9] Verifying loyalty points...\n";

        assert(
            passenger->getLoyaltyPoints()
            == 50
        );

        std::cout
            << "Loyalty points verification PASSED.\n";

        // =================================================
        // STEP 10: SAVE ALL DATA
        // =================================================

        std::cout
            << "\n[10] Saving system data...\n";

        AircraftRepository aircraftRepository(
            aircraftFile
        );

        FlightRepository flightRepository(
            flightFile
        );

        ReservationRepository reservationRepository(
            reservationFile
        );

        PaymentRepository paymentRepository(
            paymentFile
        );

        aircraftRepository.save(aircraftList);

        flightRepository.save(flights);

        reservationRepository.save(
            bookingService.getAllReservations()
        );

        paymentRepository.save(
            paymentService.getAllPayments()
        );

        assert(
            std::filesystem::exists(aircraftFile)
        );

        assert(
            std::filesystem::exists(flightFile)
        );

        assert(
            std::filesystem::exists(reservationFile)
        );

        assert(
            std::filesystem::exists(paymentFile)
        );

        std::cout
            << "All data saved successfully.\n";

        // =================================================
        // STEP 11: SIMULATE APPLICATION RESTART
        // =================================================

        std::cout
            << "\n[11] Simulating application restart...\n";

        aircraft.reset();
        flight.reset();
        passenger.reset();
        reservation.reset();
        payment.reset();

        aircraftList.clear();
        flights.clear();

        std::cout
            << "Memory cleared.\n";

        // =================================================
        // STEP 12: LOAD AIRCRAFT
        // =================================================

        std::cout
            << "\n[12] Loading aircraft...\n";

        aircraftList =
            aircraftRepository.load();

        assert(
            aircraftList.size()
            == 1
        );

        auto loadedAircraft =
            aircraftList[0];

        assert(
            loadedAircraft->getRegistrationNumber()
            == "SU-INT"
        );

        std::cout
            << "Aircraft loaded successfully.\n";

        // =================================================
        // STEP 13: LOAD PASSENGER
        // =================================================

        std::cout
            << "\n[13] Recreating passenger for relationship loading...\n";

        passenger =
            std::make_shared<Passenger>(
                1,
                "booking_passenger",
                "test_hash",
                "Booking Passenger",
                "booking@test.com",
                "01000000000",
                "P999999",
                50
            );

        passenger->setPreferredSeat("12A");
        passenger->setMealPreference("Vegetarian");

        std::vector<std::shared_ptr<Passenger>>
            passengers{
                passenger
            };

        std::cout
            << "Passenger recreated.\n";

        // =================================================
        // STEP 14: LOAD FLIGHT
        // =================================================

        std::cout
            << "\n[14] Loading flight...\n";

        flights =
            flightRepository.load(
                aircraftList,
                {}
            );

        assert(
            flights.size()
            == 1
        );

        auto loadedFlight =
            flights[0];

        assert(
            loadedFlight->getFlightNumber()
            == "MS999"
        );

        assert(
            loadedFlight->getOrigin()
            == "Cairo"
        );

        assert(
            loadedFlight->getDestination()
            == "Dubai"
        );

        assert(
            loadedFlight->getPrice()
            == 5000.0
        );

        std::cout
            << "Flight loaded successfully.\n";

        // =================================================
        // STEP 15: VERIFY OCCUPIED SEAT AFTER LOAD
        // =================================================

        std::cout
            << "\n[15] Verifying occupied seat after reload...\n";

        assert(
            !loadedFlight->isSeatAvailable("12A")
        );

        assert(
            loadedFlight->getOccupiedSeats().count("12A")
            == 1
        );

        std::cout
            << "Occupied seat persistence PASSED.\n";

        // =================================================
        // STEP 16: LOAD RESERVATION
        // =================================================

        std::cout
            << "\n[16] Loading reservation...\n";

        auto loadedReservations =
            reservationRepository.load(
                passengers,
                flights
            );

        assert(
            loadedReservations.size()
            == 1
        );

        auto loadedReservation =
            loadedReservations[0];

        assert(
            loadedReservation->getId()
            == reservationId
        );

        assert(
            loadedReservation->getSeatNumber()
            == "12A"
        );

        assert(
            loadedReservation->getTotalPrice()
            == 5000.0
        );

        assert(
            loadedReservation->getStatus()
            == ReservationStatus::Confirmed
        );

        assert(
            loadedReservation->getPassenger()
            == passenger
        );

        assert(
            loadedReservation->getFlight()
            == loadedFlight
        );

        std::cout
            << "Reservation persistence PASSED.\n";

        // =================================================
        // STEP 17: LOAD PAYMENT
        // =================================================

        std::cout
            << "\n[17] Loading payment...\n";

        auto loadedPayments =
            paymentRepository.load();

        assert(
            loadedPayments.size()
            == 1
        );

        auto loadedPayment =
            loadedPayments[0];

        assert(
            loadedPayment->getReservationId()
            == loadedReservation->getId()
        );

        assert(
            loadedPayment->getAmount()
            == 5000.0
        );

        assert(
            loadedPayment->getMethod()
            == PaymentMethod::Card
        );

        assert(
            loadedPayment->getStatus()
            == PaymentStatus::Completed
        );

        std::cout
            << "Payment persistence PASSED.\n";

        // =================================================
        // STEP 18: VERIFY LOYALTY AFTER RELOAD
        // =================================================

        std::cout
            << "\n[18] Verifying loyalty points after reload...\n";

        assert(
            passenger->getLoyaltyPoints()
            == 50
        );

        std::cout
            << "Loyalty persistence verification PASSED.\n";

        // =================================================
        // CLEANUP
        // =================================================

        std::filesystem::remove(aircraftFile);
        std::filesystem::remove(flightFile);
        std::filesystem::remove(reservationFile);
        std::filesystem::remove(paymentFile);

        std::cout
            << "\n========================================\n"
            << "BOOKING LIFECYCLE INTEGRATION TEST PASSED\n"
            << "========================================\n";

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "\n========================================\n"
            << "BOOKING INTEGRATION TEST FAILED\n"
            << "========================================\n"
            << e.what()
            << "\n";

        std::filesystem::remove(aircraftFile);
        std::filesystem::remove(flightFile);
        std::filesystem::remove(reservationFile);
        std::filesystem::remove(paymentFile);

        return 1;
    }
}