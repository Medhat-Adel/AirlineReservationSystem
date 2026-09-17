#include "services/FlightOperationsService.h"

#include <stdexcept>

void FlightOperationsService::startBoarding(
    const std::shared_ptr<Flight>& flight
) const
{
    if (flight == nullptr)
    {
        throw std::invalid_argument(
            "Flight cannot be null."
        );
    }

    if (flight->getStatus() !=
        FlightStatus::Scheduled)
    {
        throw std::invalid_argument(
            "Boarding can only start for a scheduled flight."
        );
    }

    flight->setStatus(
        FlightStatus::Boarding
    );
}

void FlightOperationsService::delayFlight(
    const std::shared_ptr<Flight>& flight
) const
{
    if (flight == nullptr)
    {
        throw std::invalid_argument(
            "Flight cannot be null."
        );
    }

    if (flight->getStatus() !=
        FlightStatus::Scheduled)
    {
        throw std::invalid_argument(
            "Only scheduled flights can be delayed."
        );
    }

    flight->setStatus(
        FlightStatus::Delayed
    );
}

void FlightOperationsService::cancelFlight(
    const std::shared_ptr<Flight>& flight
) const
{
    if (flight == nullptr)
    {
        throw std::invalid_argument(
            "Flight cannot be null."
        );
    }

    if (flight->getStatus() ==
            FlightStatus::Departed ||
        flight->getStatus() ==
            FlightStatus::Completed ||
        flight->getStatus() ==
            FlightStatus::Cancelled)
    {
        throw std::invalid_argument(
            "This flight cannot be cancelled."
        );
    }

    flight->setStatus(
        FlightStatus::Cancelled
    );
}

void FlightOperationsService::departFlight(
    const std::shared_ptr<Flight>& flight
) const
{
    if (flight == nullptr)
    {
        throw std::invalid_argument(
            "Flight cannot be null."
        );
    }

    if (flight->getStatus() !=
        FlightStatus::Boarding)
    {
        throw std::invalid_argument(
            "Only boarding flights can depart."
        );
    }

    flight->setStatus(
        FlightStatus::Departed
    );
}

void FlightOperationsService::completeFlight(
    const std::shared_ptr<Flight>& flight
) const
{
    if (flight == nullptr)
    {
        throw std::invalid_argument(
            "Flight cannot be null."
        );
    }

    if (flight->getStatus() !=
        FlightStatus::Departed)
    {
        throw std::invalid_argument(
            "Only departed flights can be completed."
        );
    }

    flight->setStatus(
        FlightStatus::Completed
    );
}