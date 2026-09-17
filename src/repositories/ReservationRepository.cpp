#include "repositories/ReservationRepository.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <algorithm>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace
{
    std::string paymentMethodToString(
        PaymentMethod method
    )
    {
        switch (method)
        {
            case PaymentMethod::Cash:
                return "Cash";

            case PaymentMethod::Card:
                return "Card";

            case PaymentMethod::BankTransfer:
                return "BankTransfer";
        }

        throw std::runtime_error(
            "Unknown payment method."
        );
    }

    PaymentMethod stringToPaymentMethod(
        const std::string& method
    )
    {
        if (method == "Cash")
        {
            return PaymentMethod::Cash;
        }

        if (method == "Card")
        {
            return PaymentMethod::Card;
        }

        if (method == "BankTransfer")
        {
            return PaymentMethod::BankTransfer;
        }

        throw std::runtime_error(
            "Unknown payment method: " + method
        );
    }

    std::string reservationStatusToString(
        ReservationStatus status
    )
    {
        switch (status)
        {
            case ReservationStatus::Confirmed:
                return "Confirmed";

            case ReservationStatus::Waitlisted:
                return "Waitlisted";

            case ReservationStatus::Cancelled:
                return "Cancelled";
        }

        throw std::runtime_error(
            "Unknown reservation status."
        );
    }

    ReservationStatus stringToReservationStatus(
        const std::string& status
    )
    {
        if (status == "Confirmed")
        {
            return ReservationStatus::Confirmed;
        }

        if (status == "Waitlisted")
        {
            return ReservationStatus::Waitlisted;
        }

        if (status == "Cancelled")
        {
            return ReservationStatus::Cancelled;
        }

        throw std::runtime_error(
            "Unknown reservation status: " + status
        );
    }
}

ReservationRepository::ReservationRepository(
    const std::string& filePath
)
    : filePath(filePath)
{
}

void ReservationRepository::save(
    const std::vector<std::shared_ptr<Reservation>>& reservations
) const
{
    std::filesystem::path path(filePath);

    if (!path.parent_path().empty())
    {
        std::filesystem::create_directories(
            path.parent_path()
        );
    }

    json data = json::array();

    for (const auto& reservation : reservations)
    {
        if (!reservation)
        {
            continue;
        }

        auto passenger =
            reservation->getPassenger();

        auto flight =
            reservation->getFlight();

        if (!passenger || !flight)
        {
            throw std::runtime_error(
                "Reservation contains invalid relationship."
            );
        }

        json item = {
            {"id", reservation->getId()},
            {"passengerId", passenger->getId()},
            {"flightNumber", flight->getFlightNumber()},
            {"seatNumber", reservation->getSeatNumber()},
            {"bookingDate", reservation->getBookingDate()},
            {"totalPrice", reservation->getTotalPrice()},
            {"paymentMethod",
             paymentMethodToString(
                 reservation->getPaymentMethod()
             )},
            {"status",
             reservationStatusToString(
                 reservation->getStatus()
             )}
        };

        data.push_back(item);
    }

    std::ofstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open reservation file for writing: "
            + filePath
        );
    }

    file << data.dump(4);

    if (!file)
    {
        throw std::runtime_error(
            "Failed to write reservation data."
        );
    }
}

std::vector<std::shared_ptr<Reservation>>
ReservationRepository::load(
    const std::vector<std::shared_ptr<Passenger>>& passengers,
    const std::vector<std::shared_ptr<Flight>>& flights
) const
{
    std::vector<std::shared_ptr<Reservation>> reservations;

    if (!std::filesystem::exists(filePath))
    {
        return reservations;
    }

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open reservation file for reading: "
            + filePath
        );
    }

    json data;

    try
    {
        file >> data;
    }
    catch (const json::parse_error& e)
    {
        throw std::runtime_error(
            std::string("Invalid reservation JSON file: ")
            + e.what()
        );
    }

    if (!data.is_array())
    {
        throw std::runtime_error(
            "Reservation JSON root must be an array."
        );
    }

    for (const auto& item : data)
    {
        try
        {
            int id =
                item.at("id").get<int>();

            int passengerId =
                item.at("passengerId").get<int>();

            std::string flightNumber =
                item.at("flightNumber")
                    .get<std::string>();

            std::string seatNumber =
                item.at("seatNumber")
                    .get<std::string>();

            std::string bookingDate =
                item.at("bookingDate")
                    .get<std::string>();

            double totalPrice =
                item.at("totalPrice")
                    .get<double>();

            PaymentMethod paymentMethod =
                stringToPaymentMethod(
                    item.at("paymentMethod")
                        .get<std::string>()
                );

            ReservationStatus status =
                stringToReservationStatus(
                    item.at("status")
                        .get<std::string>()
                );

            auto passengerIt =
                std::find_if(
                    passengers.begin(),
                    passengers.end(),
                    [passengerId](
                        const std::shared_ptr<Passenger>& passenger
                    )
                    {
                        return passenger &&
                               passenger->getId()
                                   == passengerId;
                    }
                );

            if (passengerIt == passengers.end())
            {
                throw std::runtime_error(
                    "Passenger not found for reservation ID: "
                    + std::to_string(id)
                );
            }

            auto flightIt =
                std::find_if(
                    flights.begin(),
                    flights.end(),
                    [&flightNumber](
                        const std::shared_ptr<Flight>& flight
                    )
                    {
                        return flight &&
                               flight->getFlightNumber()
                                   == flightNumber;
                    }
                );

            if (flightIt == flights.end())
            {
                throw std::runtime_error(
                    "Flight not found for reservation ID: "
                    + std::to_string(id)
                );
            }

            auto reservation =
                std::make_shared<Reservation>(
                    id,
                    *passengerIt,
                    *flightIt,
                    seatNumber,
                    bookingDate,
                    totalPrice,
                    paymentMethod,
                    status
                );

            reservations.push_back(
                reservation
            );
        }
        catch (const json::exception& e)
        {
            throw std::runtime_error(
                std::string(
                    "Invalid reservation data: "
                )
                + e.what()
            );
        }
    }

    return reservations;
}