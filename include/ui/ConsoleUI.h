#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include <memory>
#include <string>
#include <vector>

#include "models/User.h"
#include "models/Flight.h"
#include "models/Aircraft.h"
#include "models/CrewMember.h"

class AuthenticationService;
class UserManagementService;

class ConsoleUI
{
private:
    std::vector<std::shared_ptr<User>>& users;

    AuthenticationService& authenticationService;
    UserManagementService& userManagementService;

    std::vector<std::shared_ptr<Aircraft>> aircraft;
    std::vector<std::shared_ptr<CrewMember>> crewMembers;
    std::vector<std::shared_ptr<Flight>> flights;

    std::shared_ptr<User> currentUser;

public:
    ConsoleUI(
        std::vector<std::shared_ptr<User>>& users,
        AuthenticationService& authenticationService,
        UserManagementService& userManagementService
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
    void changeFlightStatus();

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