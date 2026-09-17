#ifndef FLIGHT_OPERATIONS_SERVICE_H
#define FLIGHT_OPERATIONS_SERVICE_H

#include <memory>

#include "models/Flight.h"

class FlightOperationsService
{
public:
    void startBoarding(
        const std::shared_ptr<Flight>& flight
    ) const;

    void delayFlight(
        const std::shared_ptr<Flight>& flight
    ) const;

    void cancelFlight(
        const std::shared_ptr<Flight>& flight
    ) const;

    void departFlight(
        const std::shared_ptr<Flight>& flight
    ) const;

    void completeFlight(
        const std::shared_ptr<Flight>& flight
    ) const;
};

#endif