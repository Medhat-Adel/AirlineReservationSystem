#ifndef FLIGHT_SEARCH_SERVICE_H
#define FLIGHT_SEARCH_SERVICE_H

#include <memory>
#include <string>
#include <vector>

#include "models/Flight.h"

class FlightSearchService
{
public:
    std::vector<std::shared_ptr<Flight>> searchByDestination(
        const std::vector<std::shared_ptr<Flight>>& flights,
        const std::string& destination
    ) const;

    std::vector<std::shared_ptr<Flight>> searchByDate(
        const std::vector<std::shared_ptr<Flight>>& flights,
        const std::string& date
    ) const;

    std::vector<std::shared_ptr<Flight>> searchByMaximumPrice(
        const std::vector<std::shared_ptr<Flight>>& flights,
        double maximumPrice
    ) const;

    std::vector<std::shared_ptr<Flight>> search(
        const std::vector<std::shared_ptr<Flight>>& flights,
        const std::string& destination,
        const std::string& date,
        double maximumPrice
    ) const;
};

#endif