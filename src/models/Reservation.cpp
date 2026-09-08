#include "models/Reservation.h"

#include <stdexcept>

Reservation::Reservation(
    int id,
    const std::shared_ptr<Passenger>& passenger,
    const std::shared_ptr<Flight>& flight,
    const std::string& seatNumber,
    const std::string& bookingDate,
    double totalPrice,
    PaymentMethod paymentMethod,
    ReservationStatus status
)
    : id(id),
      passenger(passenger),
      flight(flight),
      seatNumber(seatNumber),
      bookingDate(bookingDate),
      totalPrice(totalPrice),
      status(status),
      paymentMethod(paymentMethod)
{
    if (id <= 0)
    {
        throw std::invalid_argument(
            "Reservation ID must be positive."
        );
    }

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

    if (totalPrice < 0)
    {
        throw std::invalid_argument(
            "Reservation price cannot be negative."
        );
    }
}

int Reservation::getId() const
{
    return id;
}

std::shared_ptr<Passenger> Reservation::getPassenger() const
{
    return passenger;
}

std::shared_ptr<Flight> Reservation::getFlight() const
{
    return flight;
}

const std::string& Reservation::getSeatNumber() const
{
    return seatNumber;
}

const std::string& Reservation::getBookingDate() const
{
    return bookingDate;
}

double Reservation::getTotalPrice() const
{
    return totalPrice;
}

PaymentMethod Reservation::getPaymentMethod() const
{
    return paymentMethod;
}

ReservationStatus Reservation::getStatus() const
{
    return status;
}

void Reservation::setStatus(ReservationStatus status)
{
    this->status = status;
}

void Reservation::setSeatNumber(
    const std::string& seatNumber
)
{
    if (seatNumber.empty())
    {
        throw std::invalid_argument(
            "Seat number cannot be empty."
        );
    }

    this->seatNumber = seatNumber;
}
