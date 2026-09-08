#ifndef BOOKING_SERVICE_H
#define BOOKING_SERVICE_H

#include <memory>
#include <vector>

#include "models/Reservation.h"
#include "services/PaymentService.h"

class BookingService
{
private:
    std::vector<std::shared_ptr<Reservation>> reservations;
    PaymentService& paymentService;
    int nextReservationId;

public:
    explicit BookingService(
        PaymentService& paymentService
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
};

#endif