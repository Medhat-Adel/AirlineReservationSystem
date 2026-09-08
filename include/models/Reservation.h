#ifndef RESERVATION_H
#define RESERVATION_H

#include <memory>
#include <string>

#include "models/Passenger.h"
#include "models/Flight.h"

enum class ReservationStatus
{
    Confirmed,
    Waitlisted,
    Cancelled
};

class Reservation
{
private:
    int id;

    std::shared_ptr<Passenger> passenger;
    std::shared_ptr<Flight> flight;

    std::string seatNumber;
    std::string bookingDate;

    double totalPrice;
    ReservationStatus status;

public:
    Reservation(
        int id,
        const std::shared_ptr<Passenger>& passenger,
        const std::shared_ptr<Flight>& flight,
        const std::string& seatNumber,
        const std::string& bookingDate,
        double totalPrice,
        ReservationStatus status = ReservationStatus::Confirmed
    );

    int getId() const;

    std::shared_ptr<Passenger> getPassenger() const;
    std::shared_ptr<Flight> getFlight() const;

    const std::string& getSeatNumber() const;
    const std::string& getBookingDate() const;

    double getTotalPrice() const;
    ReservationStatus getStatus() const;

    void setStatus(ReservationStatus status);

    void setSeatNumber(
        const std::string& seatNumber
    );
};

#endif