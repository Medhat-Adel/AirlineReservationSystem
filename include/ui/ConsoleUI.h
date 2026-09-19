#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include <memory>
#include <string>
#include <vector>

#include "models/User.h"
#include "models/Flight.h"
#include "models/Aircraft.h"
#include "models/CrewMember.h"
#include "models/Maintenance.h"
#include "services/MaintenanceService.h"
#include "services/PassengerService.h"

class AuthenticationService;
class UserManagementService;
class BookingService;
class PassengerService;
class CheckInService;

class ConsoleUI
{
private:
    std::vector<std::shared_ptr<User>>& users;

    AuthenticationService& authenticationService;
    UserManagementService& userManagementService;
    BookingService& bookingService;
    PassengerService& passengerService;
    CheckInService& checkInService;

    std::vector<std::shared_ptr<Aircraft>> aircraft;
    std::vector<std::shared_ptr<CrewMember>> crewMembers;
    std::vector<std::shared_ptr<Flight>> flights;
    std::unique_ptr<MaintenanceService> maintenanceService;
    std::shared_ptr<User> currentUser;

public:
    ConsoleUI(
        std::vector<std::shared_ptr<User>>& users,
        AuthenticationService& authenticationService,
        UserManagementService& userManagementService,
        std::unique_ptr<MaintenanceService> maintenanceService,
        BookingService& bookingService,
        PassengerService& passengerService,
        CheckInService& checkInService
    );

    void run();

private:
    void showWelcomeScreen() const;
    void showLoginScreen();
    void showMainMenu();

    void showAdministratorMenu();
    void showBookingAgentMenu();
    void showPassengerMenu();

    void setupInitialAdministrator();

    // ========================================================
    // USER MANAGEMENT
    // ========================================================

    void manageUsers();
    void listUsers() const;

    // ========================================================
    // FLIGHT MANAGEMENT
    // ========================================================

    void manageFlights();
    void listFlights() const;
    void createFlight();
    void updateFlight();
    void deleteFlight();
    void searchFlights() const;
    void createReservation();
    void modifyReservation();
    void changeFlightStatus();
    void cancelReservation();
    void viewReservationDetails() const;
    void airportCheckIn();
    void processPayment();
    void managePassengers();

    
    // ========================================================
    // PASSENGER SERVICES
    // ========================================================

    void passengerCreateReservation();
    void passengerManageReservations();
    void passengerModifyReservation();
    void passengerCancelReservation();
    void passengerOnlineCheckIn();
    void passengerViewBoardingPass();
    void passengerViewProfile();
    void passengerLoyaltyProgram();

    // ========================================================
    // AIRCRAFT MANAGEMENT
    // ========================================================

    void manageAircraft();
    void listAircraft() const;
    void createAircraft();
    void updateAircraft();
    void deleteAircraft();
    void changeAircraftStatus();

    // ========================================================
    // CREW MANAGEMENT
    // ========================================================

    void manageCrew();
    void listCrewMembers() const;
    void createCrewMember();
    void updateCrewMember();
    void deleteCrewMember();
    void changeCrewStatus();
    void assignCrewToFlight();
    void removeCrewFromFlight();

    // ============================================================
    // MAINTENANCE MANAGEMENT
    // ============================================================

    void manageMaintenance();
    void listMaintenanceRecords() const;
    void scheduleMaintenance();
    void addReplacedPart();
    void completeMaintenance();

    // ============================================================
    // REPORTS & ANALYTICS
    // ============================================================

    void showReportsMenu();
    void generateFlightPerformanceReport();
    void generateReservationStatisticsReport();
    void generateFinancialSummaryReport();
    void generateAircraftUtilizationReport();
    void generateMaintenanceReport();

    // ========================================================
    // GENERAL
    // ========================================================

    void logout();

    void pause() const;
    void clearScreen() const;

    std::string flightStatusToString(
        FlightStatus status
    ) const;
};

#endif