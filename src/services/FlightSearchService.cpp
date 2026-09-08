#include "services/FlightSearchService.h"

#include <algorithm>
#include <stdexcept>

std::vector<std::shared_ptr<Flight>>
FlightSearchService::searchByDestination(
    const std::vector<std::shared_ptr<Flight>>& flights,
    const std::string& destination
) const
{
    if (destination.empty())
    {
        throw std::invalid_argument(
            "Destination cannot be empty."
        );
    }

    std::vector<std::shared_ptr<Flight>> results;

    std::copy_if(
        flights.begin(),
        flights.end(),
        std::back_inserter(results),
        [&destination](const std::shared_ptr<Flight>& flight)
        {
            return flight != nullptr &&
                   flight->getDestination() == destination;
        }
    );

    return results;
}

std::vector<std::shared_ptr<Flight>>
FlightSearchService::searchByDate(
    const std::vector<std::shared_ptr<Flight>>& flights,
    const std::string& date
) const
{
    if (date.empty())
    {
        throw std::invalid_argument(
            "Date cannot be empty."
        );
    }

    std::vector<std::shared_ptr<Flight>> results;

    std::copy_if(
        flights.begin(),
        flights.end(),
        std::back_inserter(results),
        [&date](const std::shared_ptr<Flight>& flight)
        {
            if (flight == nullptr)
            {
                return false;
            }

            const std::string& departureTime =
                flight->getDepartureTime();

            return departureTime.size() >= date.size() &&
                   departureTime.compare(
                       0,
                       date.size(),
                       date
                   ) == 0;
        }
    );

    return results;
}

std::vector<std::shared_ptr<Flight>>
FlightSearchService::searchByMaximumPrice(
    const std::vector<std::shared_ptr<Flight>>& flights,
    double maximumPrice
) const
{
    if (maximumPrice < 0)
    {
        throw std::invalid_argument(
            "Maximum price cannot be negative."
        );
    }

    std::vector<std::shared_ptr<Flight>> results;

    std::copy_if(
        flights.begin(),
        flights.end(),
        std::back_inserter(results),
        [maximumPrice](const std::shared_ptr<Flight>& flight)
        {
            return flight != nullptr &&
                   flight->getPrice() <= maximumPrice;
        }
    );

    return results;
}

std::vector<std::shared_ptr<Flight>>
FlightSearchService::search(
    const std::vector<std::shared_ptr<Flight>>& flights,
    const std::string& destination,
    const std::string& date,
    double maximumPrice
) const
{
    if (destination.empty())
    {
        throw std::invalid_argument(
            "Destination cannot be empty."
        );
    }

    if (date.empty())
    {
        throw std::invalid_argument(
            "Date cannot be empty."
        );
    }

    if (maximumPrice < 0)
    {
        throw std::invalid_argument(
            "Maximum price cannot be negative."
        );
    }

    std::vector<std::shared_ptr<Flight>> results;

    std::copy_if(
        flights.begin(),
        flights.end(),
        std::back_inserter(results),
        [&destination, &date, maximumPrice](
            const std::shared_ptr<Flight>& flight)
        {
            if (flight == nullptr)
            {
                return false;
            }

            const std::string& departureTime =
                flight->getDepartureTime();

            const bool matchesDestination =
                flight->getDestination() == destination;

            const bool matchesDate =
                departureTime.size() >= date.size() &&
                departureTime.compare(
                    0,
                    date.size(),
                    date
                ) == 0;

            const bool matchesPrice =
                flight->getPrice() <= maximumPrice;

            return matchesDestination &&
                   matchesDate &&
                   matchesPrice;
        }
    );

    return results;
}