#include <iostream>
#include <memory>
#include <vector>

#include "models/Aircraft.h"
#include "models/Flight.h"
#include "services/FlightSearchService.h"

void printFlights(
    const std::vector<std::shared_ptr<Flight>>& flights
)
{
    for (const auto& flight : flights)
    {
        std::cout
            << flight->getFlightNumber()
            << " | "
            << flight->getOrigin()
            << " -> "
            << flight->getDestination()
            << " | "
            << flight->getDepartureTime()
            << " | Price: "
            << flight->getPrice()
            << '\n';
    }
}

int main()
{
    std::cout << "========================================\n";
    std::cout << "       FLIGHT SEARCH SERVICE TEST\n";
    std::cout << "========================================\n\n";

    auto aircraft = std::make_shared<Aircraft>(
        1,
        "SU-ABC",
        "Airbus",
        "A320",
        180
    );

    auto flight1 = std::make_shared<Flight>(
        "MS123",
        "Cairo",
        "Dubai",
        "2026-09-10 10:00",
        "2026-09-10 13:00",
        5000,
        aircraft,
        3
    );

    auto flight2 = std::make_shared<Flight>(
        "MS456",
        "Cairo",
        "Dubai",
        "2026-09-10 15:00",
        "2026-09-10 18:00",
        7000,
        aircraft,
        3
    );

    auto flight3 = std::make_shared<Flight>(
        "MS789",
        "Cairo",
        "Riyadh",
        "2026-09-11 09:00",
        "2026-09-11 12:00",
        4500,
        aircraft,
        3
    );

    std::vector<std::shared_ptr<Flight>> flights =
    {
        flight1,
        flight2,
        flight3
    };

    FlightSearchService searchService;

    // Test 1: Search by destination
    std::cout << "[1] Searching flights to Dubai...\n";

    auto dubaiFlights =
        searchService.searchByDestination(
            flights,
            "Dubai"
        );

    std::cout << "    Results: "
              << dubaiFlights.size()
              << "\n\n";

    printFlights(dubaiFlights);

    // Test 2: Search by date
    std::cout << "\n[2] Searching flights on 2026-09-10...\n";

    auto dateFlights =
        searchService.searchByDate(
            flights,
            "2026-09-10"
        );

    std::cout << "    Results: "
              << dateFlights.size()
              << "\n\n";

    printFlights(dateFlights);

    // Test 3: Search by maximum price
    std::cout << "\n[3] Searching flights under 6000...\n";

    auto cheapFlights =
        searchService.searchByMaximumPrice(
            flights,
            6000
        );

    std::cout << "    Results: "
              << cheapFlights.size()
              << "\n\n";

    printFlights(cheapFlights);

    // Test 4: Combined search
    std::cout
        << "\n[4] Combined search:\n"
        << "    Destination: Dubai\n"
        << "    Date: 2026-09-10\n"
        << "    Maximum price: 6000\n";

    auto combinedResults =
        searchService.search(
            flights,
            "Dubai",
            "2026-09-10",
            6000
        );

    std::cout << "    Results: "
              << combinedResults.size()
              << "\n\n";

    printFlights(combinedResults);

    std::cout << "\n========================================\n";
    std::cout << "      FLIGHT SEARCH TEST COMPLETED\n";
    std::cout << "========================================\n";

    return 0;
}