#ifndef REPORT_SERVICE_H
#define REPORT_SERVICE_H

#include <memory>
#include <vector>

#include "models/Flight.h"
#include "models/Reservation.h"
#include "models/Aircraft.h"
#include "models/Maintenance.h"
#include "models/Payment.h"

class ReportService
{
public:
    void generateFlightPerformanceReport(
        const std::vector<std::shared_ptr<Flight>>& flights
    ) const;

    void generateReservationStatisticsReport(
        const std::vector<std::shared_ptr<Reservation>>& reservations
    ) const;

    void generateFinancialSummaryReport(
        const std::vector<std::shared_ptr<Payment>>& payments
    ) const;

    void generateAircraftUtilizationReport(
        const std::vector<std::shared_ptr<Aircraft>>& aircraft
    ) const;

    void generateMaintenanceReport(
        const std::vector<std::shared_ptr<Maintenance>>& maintenanceRecords
    ) const;
};

#endif