#include "repositories/FlightRepository.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace
{
    std::string flightStatusToString(FlightStatus status)
    {
        switch (status)
        {
        case FlightStatus::Scheduled:
            return "Scheduled";

        case FlightStatus::Boarding:
            return "Boarding";

        case FlightStatus::Departed:
            return "Departed";

        case FlightStatus::Delayed:
            return "Delayed";

        case FlightStatus::Cancelled:
            return "Cancelled";

        case FlightStatus::Completed:
            return "Completed";
        }

        throw std::runtime_error("Invalid flight status.");
    }

    FlightStatus stringToFlightStatus(const std::string& status)
    {
        if (status == "Scheduled")
            return FlightStatus::Scheduled;

        if (status == "Boarding")
            return FlightStatus::Boarding;

        if (status == "Departed")
            return FlightStatus::Departed;

        if (status == "Delayed")
            return FlightStatus::Delayed;

        if (status == "Cancelled")
            return FlightStatus::Cancelled;

        if (status == "Completed")
            return FlightStatus::Completed;

        throw std::runtime_error(
            "Invalid flight status: " + status
        );
    }
}

FlightRepository::FlightRepository(
    const std::string& filePath
)
    : filePath(filePath)
{
}

void FlightRepository::save(
    const std::vector<std::shared_ptr<Flight>>& flights
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

    for (const auto& flight : flights)
    {
        if (!flight)
        {
            continue;
        }

        int aircraftId = 0;

        if (flight->getAircraft())
        {
            aircraftId = flight->getAircraft()->getId();
        }

        json occupiedSeats = json::array();

        for (const auto& seat : flight->getOccupiedSeats())
        {
            occupiedSeats.push_back(seat);
        }

        json crewMembers = json::array();

        for (const auto& crewMember : flight->getCrewMembers())
        {
            if (crewMember)
            {
                crewMembers.push_back(
                    crewMember->getId()
                );
            }
        }

        data.push_back({
            {"flightNumber", flight->getFlightNumber()},
            {"origin", flight->getOrigin()},
            {"destination", flight->getDestination()},
            {"departureTime", flight->getDepartureTime()},
            {"arrivalTime", flight->getArrivalTime()},
            {"price", flight->getPrice()},
            {"status", flightStatusToString(
                flight->getStatus()
            )},
            {"aircraftId", aircraftId},
            {"occupiedSeats", occupiedSeats},
            {"crewMemberIds", crewMembers},
            {"flightDurationHours",
                flight->getFlightDurationHours()}
        });
    }

    std::ofstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open flight file for writing: "
            + filePath
        );
    }

    file << data.dump(4);

    if (!file)
    {
        throw std::runtime_error(
            "Failed to write flight data."
        );
    }
}

std::vector<std::shared_ptr<Flight>>
FlightRepository::load(
    const std::vector<std::shared_ptr<Aircraft>>& aircraft,
    const std::vector<std::shared_ptr<CrewMember>>& crewMembers
) const
{
    std::vector<std::shared_ptr<Flight>> flights;

    if (!std::filesystem::exists(filePath))
    {
        return flights;
    }

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open flight file for reading: "
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
            std::string("Invalid flight JSON file: ")
            + e.what()
        );
    }

    if (!data.is_array())
    {
        throw std::runtime_error(
            "Flight JSON root must be an array."
        );
    }

    for (const auto& item : data)
    {
        try
        {
            std::string flightNumber =
                item.at("flightNumber").get<std::string>();

            std::string origin =
                item.at("origin").get<std::string>();

            std::string destination =
                item.at("destination").get<std::string>();

            std::string departureTime =
                item.at("departureTime").get<std::string>();

            std::string arrivalTime =
                item.at("arrivalTime").get<std::string>();

            double price =
                item.at("price").get<double>();

            FlightStatus status =
                stringToFlightStatus(
                    item.at("status").get<std::string>()
                );

            int aircraftId =
                item.at("aircraftId").get<int>();

            int flightDurationHours =
                item.at("flightDurationHours").get<int>();

            std::shared_ptr<Aircraft> aircraftPtr = nullptr;

            for (const auto& aircraftItem : aircraft)
            {
                if (aircraftItem &&
                    aircraftItem->getId() == aircraftId)
                {
                    aircraftPtr = aircraftItem;
                    break;
                }
            }

            if (!aircraftPtr)
            {
                throw std::runtime_error(
                    "Aircraft with ID "
                    + std::to_string(aircraftId)
                    + " was not found."
                );
            }

            auto flight = std::make_shared<Flight>(
                flightNumber,
                origin,
                destination,
                departureTime,
                arrivalTime,
                price,
                aircraftPtr,
                flightDurationHours,
                status
            );

if (item.contains("occupiedSeats") &&
    item.at("occupiedSeats").is_array())
{
    for (const auto& seat :
         item.at("occupiedSeats"))
    {
        flight->reserveSeat(
            seat.get<std::string>()
        );
    }
}

if (item.contains("crewMemberIds") &&
    item.at("crewMemberIds").is_array())
{
    for (const auto& crewIdValue :
         item.at("crewMemberIds"))
    {
        int crewMemberId =
            crewIdValue.get<int>();

        auto crewIt = std::find_if(
            crewMembers.begin(),
            crewMembers.end(),
            [crewMemberId](
                const std::shared_ptr<CrewMember>& crewMember
            )
            {
                return crewMember &&
                       crewMember->getId() == crewMemberId;
            }
        );

        if (crewIt == crewMembers.end())
        {
            throw std::runtime_error(
                "Crew member not found for flight: "
                + std::to_string(crewMemberId)
            );
        }

        flight->restoreCrewMember(*crewIt);
    }
}

            /*
                Crew members will be connected later
                when CrewRepository is implemented.
            */

            flights.push_back(flight);
        }
        catch (const json::exception& e)
        {
            throw std::runtime_error(
                std::string("Invalid flight data: ")
                + e.what()
            );
        }
    }

    return flights;
}