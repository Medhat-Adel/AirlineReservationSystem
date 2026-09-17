#include "services/ReportService.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>

void ReportService::generateFlightPerformanceReport(
    const std::vector<std::shared_ptr<Flight>>& flights
) const
{
    std::cout << "\n========================================\n";
    std::cout << "       FLIGHT PERFORMANCE REPORT\n";
    std::cout << "========================================\n";

    if (flights.empty())
    {
        std::cout << "No flight data available.\n";
        return;
    }

    std::map<FlightStatus, int> statusCounts;

    for (const auto& flight : flights)
    {
        if (flight != nullptr)
        {
            statusCounts[flight->getStatus()]++;
        }
    }

    std::cout << "\nTotal Flights: "
              << flights.size()
              << "\n";

    std::cout << "\nFlight Status Summary:\n";

    for (const auto& [status, count] : statusCounts)
    {
        std::string statusName;

        switch (status)
        {
        case FlightStatus::Scheduled:
            statusName = "Scheduled";
            break;

        case FlightStatus::Boarding:
            statusName = "Boarding";
            break;

        case FlightStatus::Departed:
            statusName = "Departed";
            break;

        case FlightStatus::Delayed:
            statusName = "Delayed";
            break;

        case FlightStatus::Cancelled:
            statusName = "Cancelled";
            break;

        case FlightStatus::Completed:
            statusName = "Completed";
            break;
        }

        std::cout << "  "
                  << statusName
                  << ": "
                  << count
                  << "\n";
    }

    const auto completedCount = std::count_if(
        flights.begin(),
        flights.end(),
        [](const auto& flight)
        {
            return flight != nullptr &&
                   flight->getStatus() ==
                       FlightStatus::Completed;
        }
    );

    const auto cancelledCount = std::count_if(
        flights.begin(),
        flights.end(),
        [](const auto& flight)
        {
            return flight != nullptr &&
                   flight->getStatus() ==
                       FlightStatus::Cancelled;
        }
    );

    const auto delayedCount = std::count_if(
        flights.begin(),
        flights.end(),
        [](const auto& flight)
        {
            return flight != nullptr &&
                   flight->getStatus() ==
                       FlightStatus::Delayed;
        }
    );

    std::cout << "\nCompleted Flights: "
              << completedCount
              << "\n";

    std::cout << "Delayed Flights: "
              << delayedCount
              << "\n";

    std::cout << "Cancelled Flights: "
              << cancelledCount
              << "\n";

    std::cout << "========================================\n";
}

void ReportService::generateReservationStatisticsReport(
    const std::vector<std::shared_ptr<Reservation>>& reservations
) const
{
    std::cout << "\n========================================\n";
    std::cout << "     RESERVATION STATISTICS REPORT\n";
    std::cout << "========================================\n";

    const auto confirmedCount = std::count_if(
        reservations.begin(),
        reservations.end(),
        [](const auto& reservation)
        {
            return reservation != nullptr &&
                   reservation->getStatus() ==
                       ReservationStatus::Confirmed;
        }
    );

    const auto waitlistedCount = std::count_if(
        reservations.begin(),
        reservations.end(),
        [](const auto& reservation)
        {
            return reservation != nullptr &&
                   reservation->getStatus() ==
                       ReservationStatus::Waitlisted;
        }
    );

    const auto cancelledCount = std::count_if(
        reservations.begin(),
        reservations.end(),
        [](const auto& reservation)
        {
            return reservation != nullptr &&
                   reservation->getStatus() ==
                       ReservationStatus::Cancelled;
        }
    );

    std::cout << "\nTotal Reservations: "
              << reservations.size()
              << "\n";

    std::cout << "Confirmed: "
              << confirmedCount
              << "\n";

    std::cout << "Waitlisted: "
              << waitlistedCount
              << "\n";

    std::cout << "Cancelled: "
              << cancelledCount
              << "\n";

    std::cout << "========================================\n";
}

void ReportService::generateFinancialSummaryReport(
    const std::vector<std::shared_ptr<Payment>>& payments
) const
{
    std::cout << "\n========================================\n";
    std::cout << "        FINANCIAL SUMMARY REPORT\n";
    std::cout << "========================================\n";

    double completedAmount = 0.0;
    double refundedAmount = 0.0;

    for (const auto& payment : payments)
    {
        if (payment == nullptr)
        {
            continue;
        }

        if (payment->getStatus() ==
            PaymentStatus::Completed)
        {
            completedAmount += payment->getAmount();
        }
        else if (payment->getStatus() ==
                 PaymentStatus::Refunded)
        {
            refundedAmount += payment->getAmount();
        }
    }

    const double netRevenue =
        completedAmount - refundedAmount;

    std::cout << std::fixed
              << std::setprecision(2);

    std::cout << "\nTotal Payments: "
              << payments.size()
              << "\n";

    std::cout << "Completed Payments: "
              << completedAmount
              << "\n";

    std::cout << "Refunded Amount: "
              << refundedAmount
              << "\n";

    std::cout << "Net Revenue: "
              << netRevenue
              << "\n";

    std::cout << "========================================\n";
}

void ReportService::generateAircraftUtilizationReport(
    const std::vector<std::shared_ptr<Aircraft>>& aircraft
) const
{
    std::cout << "\n========================================\n";
    std::cout << "      AIRCRAFT UTILIZATION REPORT\n";
    std::cout << "========================================\n";

    if (aircraft.empty())
    {
        std::cout << "No aircraft data available.\n";
        return;
    }

    std::map<AircraftStatus, int> statusCounts;

    for (const auto& currentAircraft : aircraft)
    {
        if (currentAircraft != nullptr)
        {
            statusCounts[
                currentAircraft->getStatus()
            ]++;
        }
    }

    std::cout << "\nTotal Aircraft: "
              << aircraft.size()
              << "\n";

    std::cout << "\nAircraft Status Summary:\n";

    for (const auto& [status, count] : statusCounts)
    {
        std::string statusName;

        switch (status)
        {
        case AircraftStatus::Available:
            statusName = "Available";
            break;

        case AircraftStatus::InFlight:
            statusName = "In Flight";
            break;

        case AircraftStatus::Maintenance:
            statusName = "Maintenance";
            break;

        case AircraftStatus::Unavailable:
            statusName = "Unavailable";
            break;
        }

        std::cout << "  "
                  << statusName
                  << ": "
                  << count
                  << "\n";
    }

    const auto availableCount = std::count_if(
        aircraft.begin(),
        aircraft.end(),
        [](const auto& currentAircraft)
        {
            return currentAircraft != nullptr &&
                   currentAircraft->getStatus() ==
                       AircraftStatus::Available;
        }
    );

    std::cout << "\nAvailable Aircraft: "
              << availableCount
              << "\n";

    std::cout << "========================================\n";
}

void ReportService::generateMaintenanceReport(
    const std::vector<std::shared_ptr<Maintenance>>& maintenanceRecords
) const
{
    std::cout << "\n========================================\n";
    std::cout << "        MAINTENANCE REPORT\n";
    std::cout << "========================================\n";

    const auto completedCount = std::count_if(
        maintenanceRecords.begin(),
        maintenanceRecords.end(),
        [](const auto& maintenance)
        {
            return maintenance != nullptr &&
                   maintenance->isCompleted();
        }
    );

    const auto pendingCount = std::count_if(
        maintenanceRecords.begin(),
        maintenanceRecords.end(),
        [](const auto& maintenance)
        {
            return maintenance != nullptr &&
                   !maintenance->isCompleted();
        }
    );

    std::size_t totalReplacedParts = 0;

    for (const auto& maintenance :
         maintenanceRecords)
    {
        if (maintenance != nullptr)
        {
            totalReplacedParts +=
                maintenance->getReplacedParts().size();
        }
    }

    std::cout << "\nTotal Maintenance Records: "
              << maintenanceRecords.size()
              << "\n";

    std::cout << "Completed: "
              << completedCount
              << "\n";

    std::cout << "Pending: "
              << pendingCount
              << "\n";

    std::cout << "Total Replaced Parts: "
              << totalReplacedParts
              << "\n";

    std::cout << "========================================\n";
}