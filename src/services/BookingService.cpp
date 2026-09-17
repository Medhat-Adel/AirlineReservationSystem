#include "services/BookingService.h"

#include <stdexcept>

BookingService::BookingService(
    PaymentService& paymentService,
    LoyaltyService& loyaltyService
)
    : paymentService(paymentService),
      loyaltyService(loyaltyService),
      nextReservationId(1)
{
}

std::shared_ptr<Reservation>
BookingService::createReservation(
    const std::shared_ptr<Passenger>& passenger,
    const std::shared_ptr<Flight>& flight,
    const std::string& seatNumber,
    const std::string& bookingDate,
    PaymentMethod paymentMethod
)
{
    if (passenger == nullptr)
    {
        throw std::invalid_argument(
            "Passenger cannot be null."
        );
    }

    if (flight == nullptr)
    {
        throw std::invalid_argument(
            "Flight cannot be null."
        );
    }

    if (seatNumber.empty())
    {
        throw std::invalid_argument(
            "Seat number cannot be empty."
        );
    }

    if (bookingDate.empty())
    {
        throw std::invalid_argument(
            "Booking date cannot be empty."
        );
    }

    if (flight->getStatus() == FlightStatus::Cancelled)
    {
        throw std::runtime_error(
            "Cannot book a cancelled flight."
        );
    }

    // If the selected seat is unavailable,
    // add the reservation to the waitlist.
    if (!flight->isSeatAvailable(seatNumber))
    {
        auto reservation = std::make_shared<Reservation>(
            nextReservationId++,
            passenger,
            flight,
            seatNumber,
            bookingDate,
            flight->getPrice(),
            paymentMethod,
            ReservationStatus::Waitlisted
        );

        reservations.push_back(reservation);
        waitlist.push(reservation);

        return reservation;
    }

    // Reserve the seat first.
    if (!flight->reserveSeat(seatNumber))
    {
        throw std::runtime_error(
            "Failed to reserve the selected seat."
        );
    }

    try
    {
        // Create reservation.
        auto reservation = std::make_shared<Reservation>(
            nextReservationId++,
            passenger,
            flight,
            seatNumber,
            bookingDate,
            flight->getPrice(),
            paymentMethod,
            ReservationStatus::Confirmed
        );

        // Process payment.
        auto payment = paymentService.processPayment(
            reservation->getId(),
            reservation->getTotalPrice(),
            paymentMethod,
            bookingDate
        );

        // Payment must be completed successfully.
        if (payment == nullptr ||
            payment->getStatus() != PaymentStatus::Completed)
        {
            throw std::runtime_error(
                "Payment processing failed."
            );
        }

        // Award loyalty points only after
        // successful payment.
        loyaltyService.awardPoints(
            passenger,
            reservation->getTotalPrice()
        );

        reservations.push_back(reservation);

        return reservation;
    }
    catch (...)
    {
        // Reservation creation or payment processing
        // failed, so release the reserved seat.
        flight->releaseSeat(seatNumber);

        throw;
    }
}

bool BookingService::modifyReservation(
    int reservationId,
    const std::string& newSeatNumber
)
{
    auto reservation = getReservation(reservationId);

    if (reservation == nullptr)
    {
        return false;
    }

    if (reservation->getStatus() ==
        ReservationStatus::Cancelled)
    {
        return false;
    }

    if (newSeatNumber.empty())
    {
        return false;
    }

    auto flight = reservation->getFlight();

    if (flight == nullptr)
    {
        return false;
    }

    // If the passenger selected the same seat,
    // there is nothing to modify.
    if (reservation->getSeatNumber() ==
        newSeatNumber)
    {
        return false;
    }

    // Check whether the new seat is available.
    if (!flight->isSeatAvailable(newSeatNumber))
    {
        return false;
    }

    const std::string oldSeatNumber =
        reservation->getSeatNumber();

    // Reserve the new seat first.
    if (!flight->reserveSeat(newSeatNumber))
    {
        return false;
    }

    try
    {
        // Update reservation.
        reservation->setSeatNumber(
            newSeatNumber
        );

        // Release the old seat.
        flight->releaseSeat(oldSeatNumber);

        return true;
    }
    catch (...)
    {
        // Roll back the new seat if
        // updating the reservation fails.
        flight->releaseSeat(newSeatNumber);

        throw;
    }
}

bool BookingService::cancelReservation(
    int reservationId
)
{
    auto reservation = getReservation(reservationId);

    if (reservation == nullptr)
    {
        return false;
    }

    if (reservation->getStatus() ==
        ReservationStatus::Cancelled)
    {
        return false;
    }

    // Find the payment associated with this reservation.
    auto payment =
        paymentService.getPaymentByReservationId(
            reservationId
        );

    if (payment != nullptr)
    {
        // Refund the payment.
        if (!paymentService.refundPayment(
                payment->getId()))
        {
            return false;
        }
    }

    // Release the reserved seat.
    auto flight = reservation->getFlight();

    if (flight != nullptr)
    {
        flight->releaseSeat(
            reservation->getSeatNumber()
        );

        processWaitlist(flight);
    }

    // Mark reservation as cancelled.
    reservation->setStatus(
        ReservationStatus::Cancelled
    );

    return true;
}

std::shared_ptr<Reservation>
BookingService::getReservation(
    int reservationId
) const
{
    for (const auto& reservation : reservations)
    {
        if (reservation->getId() == reservationId)
        {
            return reservation;
        }
    }

    return nullptr;
}

const std::vector<std::shared_ptr<Reservation>>&
BookingService::getAllReservations() const
{
    return reservations;
}

std::shared_ptr<Payment>
BookingService::getPaymentByReservationId(
    int reservationId
) const
{
    return paymentService.getPaymentByReservationId(
        reservationId
    );
}

const std::queue<std::shared_ptr<Reservation>>&
BookingService::getWaitlist() const
{
    return waitlist;
}

void BookingService::processWaitlist(
    const std::shared_ptr<Flight>& flight
)
{
    if (flight == nullptr)
    {
        return;
    }

    if (waitlist.empty())
    {
        return;
    }

    std::queue<std::shared_ptr<Reservation>> remainingWaitlist;

    bool promoted = false;

    while (!waitlist.empty())
    {
        auto reservation = waitlist.front();
        waitlist.pop();

        if (reservation == nullptr)
        {
            continue;
        }

        // Keep reservations for other flights
        // in the waitlist.
        if (reservation->getFlight() != flight)
        {
            remainingWaitlist.push(reservation);
            continue;
        }

        // Only promote the first eligible reservation.
        if (promoted)
        {
            remainingWaitlist.push(reservation);
            continue;
        }

        const std::string seatNumber =
            reservation->getSeatNumber();

        // The requested seat must be available.
        if (!flight->isSeatAvailable(seatNumber))
        {
            remainingWaitlist.push(reservation);
            continue;
        }

        // Reserve the seat.
        if (!flight->reserveSeat(seatNumber))
        {
            remainingWaitlist.push(reservation);
            continue;
        }

        try
        {
            // Process payment now that the seat
            // has become available.
            auto payment = paymentService.processPayment(
                reservation->getId(),
                reservation->getTotalPrice(),
                reservation->getPaymentMethod(),
                reservation->getBookingDate()
            );

            if (payment == nullptr ||
                payment->getStatus() != PaymentStatus::Completed)
            {
                throw std::runtime_error(
                    "Payment processing failed."
                );
            }

            // Award loyalty points only after
            // successful payment.
            loyaltyService.awardPoints(
                reservation->getPassenger(),
                reservation->getTotalPrice()
            );

            // Promote reservation.
            reservation->setStatus(
                ReservationStatus::Confirmed
            );

            promoted = true;
        }
        catch (...)
        {
            // Payment failed, so release the seat
            // and keep the reservation waitlisted.
            flight->releaseSeat(seatNumber);

            remainingWaitlist.push(reservation);
        }
    }
    

    waitlist = std::move(remainingWaitlist);
}