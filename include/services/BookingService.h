#ifndef BOOKING_SERVICE_H
#define BOOKING_SERVICE_H

#include <memory>
#include <vector>
#include <queue>

#include "models/Reservation.h"
#include "services/PaymentService.h"
#include "services/LoyaltyService.h"

class BookingService
{
private:
    std::vector<std::shared_ptr<Reservation>> reservations;

    PaymentService& paymentService;
    LoyaltyService& loyaltyService;

    int nextReservationId;

    // Waitlisted reservations
    std::queue<std::shared_ptr<Reservation>> waitlist;

public:
    explicit BookingService(
        PaymentService& paymentService,
        LoyaltyService& loyaltyService
    );

    std::shared_ptr<Reservation> createReservation(
        const std::shared_ptr<Passenger>& passenger,
        const std::shared_ptr<Flight>& flight,
        const std::string& seatNumber,
        const std::string& bookingDate,
        PaymentMethod paymentMethod
    );

    bool modifyReservation(
        int reservationId,
        const std::string& newSeatNumber
    );

    bool cancelReservation(
        int reservationId
    );

    std::shared_ptr<Reservation> getReservation(
        int reservationId
    ) const;

    const std::vector<std::shared_ptr<Reservation>>&
    getAllReservations() const;

    const std::queue<std::shared_ptr<Reservation>>&
        getWaitlist() const;

        void processWaitlist(
        const std::shared_ptr<Flight>& flight
    );
};

#endif