#include "ui/ConsoleUI.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>

#include "models/Administrator.h"
#include "models/BookingAgent.h"
#include "models/Passenger.h"
#include "models/Pilot.h"
#include "models/FlightAttendant.h"

#include "repositories/UserRepository.h"
#include "repositories/FlightRepository.h"
#include "repositories/AircraftRepository.h"
#include "repositories/CrewRepository.h"
#include "repositories/MaintenanceRepository.h"
#include "repositories/ReservationRepository.h"
#include "repositories/PaymentRepository.h"

#include "services/AuthenticationService.h"
#include "services/UserManagementService.h"
#include "services/FlightSearchService.h"
#include "services/FlightOperationsService.h"
#include "services/MaintenanceService.h"
#include "services/ReportService.h"
#include "services/BookingService.h"
#include "services/PassengerService.h"
#include "services/CheckInService.h"

// ============================================================
// CONSTRUCTOR
// ============================================================

ConsoleUI::ConsoleUI(
    std::vector<std::shared_ptr<User>>& users,
    AuthenticationService& authenticationService,
    UserManagementService& userManagementService,
    std::unique_ptr<MaintenanceService> maintenanceService,
    BookingService& bookingService,
    PassengerService& passengerService,
    CheckInService& checkInService
)
: users(users), 
  authenticationService(authenticationService), 
  userManagementService(userManagementService), 
  bookingService(bookingService), 
  passengerService(passengerService),
  checkInService(checkInService),
  maintenanceService(std::move(maintenanceService)), 
  currentUser(nullptr)
{
    AircraftRepository aircraftRepository;
    CrewRepository crewRepository;
    FlightRepository flightRepository;

    aircraft = aircraftRepository.load();

    crewMembers = crewRepository.load();

    flights =
        flightRepository.load(
            aircraft,
            crewMembers
        );
}

// ============================================================
// RUN
// ============================================================

void ConsoleUI::run()
{
    showWelcomeScreen();

    if (users.empty())
    {
        setupInitialAdministrator();
    }

    while (true)
    {
        try
        {
            showLoginScreen();

            if (!currentUser)
            {
                continue;
            }

            showMainMenu();

            if (!currentUser)
            {
                continue;
            }
        }
        catch (const std::exception& e)
        {
            std::cerr
                << "\nError: "
                << e.what()
                << "\n";

            pause();
        }
    }
}

// ============================================================
// WELCOME
// ============================================================

void ConsoleUI::showWelcomeScreen() const
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "   Airline Reservation System\n"
        << "=====================================\n"
        << "          Welcome!\n"
        << "=====================================\n\n";
}

// ============================================================
// LOGIN
// ============================================================

void ConsoleUI::showLoginScreen()
{
    std::string username;
    std::string password;

    std::cout
        << "\n========== Login ==========\n";

    std::cout
        << "Username: ";

    std::getline(
        std::cin,
        username
    );

    if (std::cin.eof())
    {
        throw std::runtime_error(
            "Input stream closed."
        );
    }

    std::cout
        << "Password: ";

    std::getline(
        std::cin,
        password
    );

    if (std::cin.eof())
    {
        throw std::runtime_error(
            "Input stream closed."
        );
    }

    try
    {
        currentUser =
            authenticationService.login(
                username,
                password
            );

        std::cout
            << "\nLogin successful!\n"
            << "Welcome, "
            << currentUser->getFullName()
            << "!\n";

        pause();
    }
    catch (const std::exception& e)
    {
        currentUser = nullptr;

        std::cout
            << "\nLogin failed: "
            << e.what()
            << "\n";

        pause();
    }
}

// ============================================================
// MAIN MENU
// ============================================================

void ConsoleUI::showMainMenu()
{
    if (!currentUser)
    {
        return;
    }

    switch (currentUser->getRole())
    {
        case Role::Administrator:
            showAdministratorMenu();
            break;

        case Role::BookingAgent:
            showBookingAgentMenu();
            break;

        case Role::Passenger:
            showPassengerMenu();
            break;

        default:
            throw std::runtime_error(
                "Unknown user role."
            );
    }
}

// ============================================================
// ADMINISTRATOR MENU
// ============================================================

void ConsoleUI::showAdministratorMenu()
{
    while (currentUser)
    {
        clearScreen();

        std::cout
            << "=====================================\n"
            << "       Administrator Dashboard\n"
            << "=====================================\n"
            << "Welcome, "
            << currentUser->getFullName()
            << "\n\n";

        std::cout
            << "1. Manage Users\n"
            << "2. Manage Flights\n"
            << "3. Manage Aircraft\n"
            << "4. Manage Crew\n"
            << "5. Maintenance Management\n"
            << "6. Reports & Analytics\n"
            << "7. Logout\n\n"
            << "Choose an option: ";

        int choice;

        if (!(std::cin >> choice))
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout
                << "\nInvalid input.\n";

            pause();
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        switch (choice)
        {
            case 1:
                manageUsers();
                break;

            case 2:
                manageFlights();
                break;

            case 3:
                manageAircraft();
                break;

            case 4:
                manageCrew();
                break;

            case 5:
                manageMaintenance();
                break;

            case 6:
                showReportsMenu();
                break;

            case 7:
                logout();
                break;

            default:
                std::cout
                    << "\nInvalid option.\n";

                pause();
                break;
        }
    }
}

// ============================================================
// BOOKING AGENT MENU
// ============================================================

void ConsoleUI::showBookingAgentMenu()
{
    while (currentUser)
    {
        clearScreen();

        std::cout
            << "=====================================\n"
            << "        Booking Agent Dashboard\n"
            << "=====================================\n"
            << "Welcome, "
            << currentUser->getFullName()
            << "\n\n";

        std::cout
            << "1. Search Flights\n"
            << "2. Create Reservation\n"
            << "3. Modify Reservation\n"
            << "4. Cancel Reservation\n"
            << "5. Airport Check-In\n"
            << "6. Process Payment\n"
            << "7. Manage Passengers\n"
            << "8. Logout\n\n"
            << "Choose an option: ";

        int choice;

        if (!(std::cin >> choice))
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout
                << "\nInvalid input.\n";

            pause();
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        switch (choice)
        {
            
            case 1:
                searchFlights();
                break;

            case 2:
                createReservation();
                break;

            case 3:
                modifyReservation();
                break;

            case 4:
                cancelReservation();
                break;    

            case 6:
                processPayment();
                break;

            case 5:
                airportCheckIn();
                break;

            case 7:
                managePassengers();
                break;
                
            case 8:
                logout();
                break;

            default:
                std::cout
                    << "\nThis feature will be implemented next.\n";

                pause();
                break;
        }
    }
}

// ============================================================
// PASSENGER MENU
// ============================================================

void ConsoleUI::showPassengerMenu()
{
    while (currentUser)
    {
        clearScreen();

        std::cout
            << "=====================================\n"
            << "          Passenger Dashboard\n"
            << "=====================================\n"
            << "Welcome, "
            << currentUser->getFullName()
            << "\n\n";

        std::cout
            << "1. Search Flights\n"
            << "2. Make Reservation\n"
            << "3. Manage Reservations\n"
            << "4. Online Check-In\n"
            << "5. View Boarding Pass\n"
            << "6. View Profile\n"
            << "7. Loyalty Program\n"
            << "8. Logout\n\n"
            << "Choose an option: ";

        int choice;

        if (!(std::cin >> choice))
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout
                << "\nInvalid input.\n";

            pause();
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        switch (choice)
        {
            case 1:
                searchFlights();
                break;

            case 2:
                passengerCreateReservation();
                break;

            case 3:
                passengerManageReservations();
                break;

            case 4:
                passengerOnlineCheckIn();
                break;

            case 5:
                passengerViewBoardingPass();
                break;

            case 6:
                passengerViewProfile();
                break;

            case 7:
                passengerLoyaltyProgram();
                break;
                
            case 8:
                logout();
                break;

            default:
                std::cout
                    << "\nThis feature will be implemented next.\n";

                pause();
                break;
        }
    }
}

// ============================================================
// INITIAL ADMINISTRATOR
// ============================================================

void ConsoleUI::setupInitialAdministrator()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "       Initial System Setup\n"
        << "=====================================\n"
        << "No users were found.\n"
        << "Create the first Administrator account.\n\n";

    std::string username;
    std::string password;
    std::string fullName;
    std::string email;
    std::string phone;

    std::cout << "Username: ";
    std::getline(
        std::cin,
        username
    );

    std::cout << "Password: ";
    std::getline(
        std::cin,
        password
    );

    std::cout << "Full Name: ";
    std::getline(
        std::cin,
        fullName
    );

    std::cout << "Email: ";
    std::getline(
        std::cin,
        email
    );

    std::cout << "Phone: ";
    std::getline(
        std::cin,
        phone
    );

    auto administrator =
        userManagementService.createAdministrator(
            username,
            password,
            fullName,
            email,
            phone
        );

    UserRepository userRepository;

    userRepository.save(users);

    std::cout
        << "\nAdministrator account created successfully!\n"
        << "User ID: "
        << administrator->getId()
        << "\n";

    pause();
}

// ============================================================
// USER MANAGEMENT
// ============================================================

void ConsoleUI::manageUsers()
{
    while (true)
    {
        clearScreen();

        std::cout
            << "=====================================\n"
            << "            Manage Users\n"
            << "=====================================\n\n"
            << "1. List All Users\n"
            << "2. Create Administrator\n"
            << "3. Create Booking Agent\n"
            << "4. Create Passenger\n"
            << "5. Activate User\n"
            << "6. Deactivate User\n"
            << "7. Back\n\n"
            << "Select an option: ";

        int choice;

        if (!(std::cin >> choice))
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout
                << "\nInvalid input.\n";

            pause();
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        switch (choice)
        {
            case 1:
                listUsers();
                break;

            case 2:
            {
                clearScreen();

                std::cout
                    << "=====================================\n"
                    << "       Create Administrator\n"
                    << "=====================================\n\n";

                std::string username;
                std::string password;
                std::string fullName;
                std::string email;
                std::string phone;

                std::cout << "Username: ";
                std::getline(
                    std::cin,
                    username
                );

                std::cout << "Password: ";
                std::getline(
                    std::cin,
                    password
                );

                std::cout << "Full Name: ";
                std::getline(
                    std::cin,
                    fullName
                );

                std::cout << "Email: ";
                std::getline(
                    std::cin,
                    email
                );

                std::cout << "Phone: ";
                std::getline(
                    std::cin,
                    phone
                );

                try
                {
                    auto administrator =
                        userManagementService.createAdministrator(
                            username,
                            password,
                            fullName,
                            email,
                            phone
                        );

                    UserRepository userRepository;

                    userRepository.save(users);

                    std::cout
                        << "\nAdministrator created successfully!\n"
                        << "User ID: "
                        << administrator->getId()
                        << "\n";

                    pause();
                }
                catch (const std::exception& e)
                {
                    std::cout
                        << "\nFailed to create administrator.\n"
                        << e.what()
                        << "\n";

                    pause();
                }

                break;
            }

            case 3:
            {
                clearScreen();

                std::cout
                    << "=====================================\n"
                    << "        Create Booking Agent\n"
                    << "=====================================\n\n";

                std::string username;
                std::string password;
                std::string fullName;
                std::string email;
                std::string phone;

                std::cout << "Username: ";
                std::getline(
                    std::cin,
                    username
                );

                std::cout << "Password: ";
                std::getline(
                    std::cin,
                    password
                );

                std::cout << "Full Name: ";
                std::getline(
                    std::cin,
                    fullName
                );

                std::cout << "Email: ";
                std::getline(
                    std::cin,
                    email
                );

                std::cout << "Phone: ";
                std::getline(
                    std::cin,
                    phone
                );

                try
                {
                    auto bookingAgent =
                        userManagementService.createBookingAgent(
                            username,
                            password,
                            fullName,
                            email,
                            phone
                        );

                    UserRepository userRepository;

                    userRepository.save(users);

                    std::cout
                        << "\nBooking Agent created successfully!\n"
                        << "User ID: "
                        << bookingAgent->getId()
                        << "\n";

                    pause();
                }
                catch (const std::exception& e)
                {
                    std::cout
                        << "\nFailed to create booking agent.\n"
                        << e.what()
                        << "\n";

                    pause();
                }

                break;
            }

            case 4:
            {
                clearScreen();

                std::cout
                    << "=====================================\n"
                    << "          Create Passenger\n"
                    << "=====================================\n\n";

                std::string username;
                std::string password;
                std::string fullName;
                std::string email;
                std::string phone;
                std::string passportNumber;

                std::cout << "Username: ";
                std::getline(
                    std::cin,
                    username
                );

                std::cout << "Password: ";
                std::getline(
                    std::cin,
                    password
                );

                std::cout << "Full Name: ";
                std::getline(
                    std::cin,
                    fullName
                );

                std::cout << "Email: ";
                std::getline(
                    std::cin,
                    email
                );

                std::cout << "Phone: ";
                std::getline(
                    std::cin,
                    phone
                );

                std::cout << "Passport Number: ";
                std::getline(
                    std::cin,
                    passportNumber
                );

                try
                {
                    auto passenger =
                        userManagementService.createPassenger(
                            username,
                            password,
                            fullName,
                            email,
                            phone,
                            passportNumber
                        );

                    UserRepository userRepository;

                    userRepository.save(users);

                    std::cout
                        << "\nPassenger created successfully!\n"
                        << "User ID: "
                        << passenger->getId()
                        << "\n";

                    pause();
                }
                catch (const std::exception& e)
                {
                    std::cout
                        << "\nFailed to create passenger.\n"
                        << e.what()
                        << "\n";

                    pause();
                }

                break;
            }

            case 5:
            {
                clearScreen();

                std::cout
                    << "=====================================\n"
                    << "            Activate User\n"
                    << "=====================================\n\n";

                int userId;

                std::cout << "Enter User ID: ";

                if (!(std::cin >> userId))
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout
                        << "\nInvalid user ID.\n";

                    pause();
                    break;
                }

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );

                try
                {
                    auto user =
                        userManagementService.findUserById(
                            userId
                        );

                    if (!user)
                    {
                        std::cout
                            << "\nUser not found.\n";

                        pause();
                        break;
                    }

                    if (user->getIsActive())
                    {
                        std::cout
                            << "\nUser is already active.\n";

                        pause();
                        break;
                    }

                    userManagementService.activateUser(
                        userId
                    );

                    UserRepository userRepository;

                    userRepository.save(users);

                    std::cout
                        << "\nUser activated successfully!\n";

                    pause();
                }
                catch (const std::exception& e)
                {
                    std::cout
                        << "\nFailed to activate user.\n"
                        << e.what()
                        << "\n";

                    pause();
                }

                break;
            }

            case 6:
            {
                clearScreen();

                std::cout
                    << "=====================================\n"
                    << "          Deactivate User\n"
                    << "=====================================\n\n";

                int userId;

                std::cout << "Enter User ID: ";

                if (!(std::cin >> userId))
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout
                        << "\nInvalid user ID.\n";

                    pause();
                    break;
                }

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );

                try
                {
                    auto user =
                        userManagementService.findUserById(
                            userId
                        );

                    if (!user)
                    {
                        std::cout
                            << "\nUser not found.\n";

                        pause();
                        break;
                    }

                    if (!user->getIsActive())
                    {
                        std::cout
                            << "\nUser is already inactive.\n";

                        pause();
                        break;
                    }

                    userManagementService.deactivateUser(
                        userId
                    );

                    UserRepository userRepository;

                    userRepository.save(users);

                    std::cout
                        << "\nUser deactivated successfully!\n";

                    pause();
                }
                catch (const std::exception& e)
                {
                    std::cout
                        << "\nFailed to deactivate user.\n"
                        << e.what()
                        << "\n";

                    pause();
                }

                break;
            }

            case 7:
                return;

            default:
                std::cout
                    << "\nInvalid option.\n";

                pause();
                break;
        }
    }
}

void ConsoleUI::listUsers() const
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "             All Users\n"
        << "=====================================\n\n";

    const auto& allUsers =
        userManagementService.getAllUsers();

    if (allUsers.empty())
    {
        std::cout
            << "No users found.\n";

        pause();
        return;
    }

    for (const auto& user : allUsers)
    {
        if (!user)
        {
            continue;
        }

        std::string role;

        switch (user->getRole())
        {
            case Role::Administrator:
                role = "Administrator";
                break;

            case Role::BookingAgent:
                role = "Booking Agent";
                break;

            case Role::Passenger:
                role = "Passenger";
                break;
        }

        std::cout
            << "-------------------------------------\n"
            << "ID       : "
            << user->getId()
            << "\n"
            << "Username : "
            << user->getUsername()
            << "\n"
            << "Name     : "
            << user->getFullName()
            << "\n"
            << "Email    : "
            << user->getEmail()
            << "\n"
            << "Phone    : "
            << user->getPhone()
            << "\n"
            << "Role     : "
            << role
            << "\n"
            << "Status   : "
            << (
                user->getIsActive()
                    ? "Active"
                    : "Inactive"
            )
            << "\n";

        auto passenger =
            std::dynamic_pointer_cast<Passenger>(user);

        if (passenger)
        {
            std::cout
                << "Passport  : "
                << passenger->getPassportNumber()
                << "\n"
                << "Loyalty   : "
                << passenger->getLoyaltyPoints()
                << " points\n";
        }
    }

    std::cout
        << "-------------------------------------\n";

    pause();
}

// ============================================================
// FLIGHT MANAGEMENT
// ============================================================

void ConsoleUI::manageFlights()
{
    while (true)
    {
        clearScreen();

        std::cout
            << "=====================================\n"
            << "           Manage Flights\n"
            << "=====================================\n\n"
            << "1. List All Flights\n"
            << "2. Create Flight\n"
            << "3. Update Flight\n"
            << "4. Delete Flight\n"
            << "5. Search Flights\n"
            << "6. Change Flight Status\n"
            << "7. Back\n\n"
            << "Select an option: ";

        int choice;

        if (!(std::cin >> choice))
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout
                << "\nInvalid input.\n";

            pause();
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        switch (choice)
        {
            case 1:
                listFlights();
                break;

            case 2:
                createFlight();
                break;

            case 3:
                updateFlight();
                break;

            case 4:
                deleteFlight();
                break;

            case 5:
                searchFlights();
                break;

            case 6:
                changeFlightStatus();
                break;

            case 7:
                return;

            default:
                std::cout
                    << "\nInvalid option.\n";

                pause();
                break;
        }
    }
}

// ============================================================
// LIST FLIGHTS
// ============================================================

void ConsoleUI::listFlights() const
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "             All Flights\n"
        << "=====================================\n\n";

    if (flights.empty())
    {
        std::cout
            << "No flights found.\n";

        pause();
        return;
    }

    for (const auto& flight : flights)
    {
        if (!flight)
        {
            continue;
        }

        std::cout
            << "-------------------------------------\n"
            << "Flight Number : "
            << flight->getFlightNumber()
            << "\n"
            << "Origin        : "
            << flight->getOrigin()
            << "\n"
            << "Destination   : "
            << flight->getDestination()
            << "\n"
            << "Departure     : "
            << flight->getDepartureTime()
            << "\n"
            << "Arrival       : "
            << flight->getArrivalTime()
            << "\n"
            << "Price         : "
            << flight->getPrice()
            << "\n"
            << "Status        : "
            << flightStatusToString(
                flight->getStatus()
            )
            << "\n"
            << "Available Seats: "
            << flight->getAvailableSeats()
            << "\n"
            << "Duration      : "
            << flight->getFlightDurationHours()
            << " hours\n";

        if (flight->getAircraft())
        {
            std::cout
                << "Aircraft ID   : "
                << flight->getAircraft()->getId()
                << "\n"
                << "Aircraft      : "
                << flight->getAircraft()->getManufacturer()
                << " "
                << flight->getAircraft()->getModel()
                << "\n"
                << "Registration  : "
                << flight->getAircraft()->getRegistrationNumber()
                << "\n";
        }

        std::cout
            << "Occupied Seats: "
            << flight->getOccupiedSeats().size()
            << "\n"
            << "Crew Members  : "
            << flight->getCrewMembers().size()
            << "\n";
    }

    std::cout
        << "-------------------------------------\n";

    pause();
}

// ============================================================
// CREATE FLIGHT
// ============================================================

void ConsoleUI::createFlight()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "            Create Flight\n"
        << "=====================================\n\n";

    if (aircraft.empty())
    {
        std::cout
            << "No aircraft are available.\n\n"
            << "Please create an aircraft first\n"
            << "from Aircraft Management.\n";

        pause();
        return;
    }

    std::cout
        << "Available Aircraft:\n\n";

    for (const auto& aircraftItem : aircraft)
    {
        if (!aircraftItem)
        {
            continue;
        }

        std::cout
            << "ID: "
            << aircraftItem->getId()
            << " | "
            << aircraftItem->getManufacturer()
            << " "
            << aircraftItem->getModel()
            << " | Registration: "
            << aircraftItem->getRegistrationNumber()
            << " | Capacity: "
            << aircraftItem->getCapacity()
            << " | Available: "
            << (
                aircraftItem->isAvailable()
                    ? "Yes"
                    : "No"
            )
            << "\n";
    }

    std::cout << "\n";

    std::string flightNumber;
    std::string origin;
    std::string destination;
    std::string departureTime;
    std::string arrivalTime;

    double price;
    int aircraftId;
    int duration;

    std::cout
        << "Flight Number: ";
    std::getline(
        std::cin,
        flightNumber
    );

    std::cout
        << "Origin: ";
    std::getline(
        std::cin,
        origin
    );

    std::cout
        << "Destination: ";
    std::getline(
        std::cin,
        destination
    );

    std::cout
        << "Departure Time (YYYY-MM-DD HH:MM): ";
    std::getline(
        std::cin,
        departureTime
    );

    std::cout
        << "Arrival Time (YYYY-MM-DD HH:MM): ";
    std::getline(
        std::cin,
        arrivalTime
    );

    std::cout
        << "Price: ";

    if (!(std::cin >> price))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid price.\n";

        pause();
        return;
    }

    std::cout
        << "Aircraft ID: ";

    if (!(std::cin >> aircraftId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid aircraft ID.\n";

        pause();
        return;
    }

    std::cout
        << "Flight Duration (hours): ";

    if (!(std::cin >> duration))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid duration.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    try
    {
        std::shared_ptr<Aircraft> selectedAircraft;

        for (const auto& aircraftItem : aircraft)
        {
            if (aircraftItem &&
                aircraftItem->getId() == aircraftId)
            {
                selectedAircraft = aircraftItem;
                break;
            }
        }

        if (!selectedAircraft)
        {
            throw std::runtime_error(
                "Aircraft not found."
            );
        }

        if (!selectedAircraft->isAvailable())
        {
            throw std::runtime_error(
                "Selected aircraft is not available."
            );
        }

        for (const auto& existingFlight : flights)
        {
            if (existingFlight &&
                existingFlight->getFlightNumber()
                    == flightNumber)
            {
                throw std::runtime_error(
                    "Flight number already exists."
                );
            }
        }

        auto flight =
            std::make_shared<Flight>(
                flightNumber,
                origin,
                destination,
                departureTime,
                arrivalTime,
                price,
                selectedAircraft,
                duration
            );

        flights.push_back(flight);

        FlightRepository flightRepository;

        flightRepository.save(flights);

        std::cout
            << "\nFlight created successfully!\n";

        pause();
    }
    catch (const std::exception& e)
    {
        std::cout
            << "\nFailed to create flight.\n"
            << e.what()
            << "\n";

        pause();
    }
}

// ============================================================
// UPDATE FLIGHT
// ============================================================

void ConsoleUI::updateFlight()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "             Update Flight\n"
        << "=====================================\n\n";

    if (flights.empty())
    {
        std::cout
            << "No flights found.\n";

        pause();
        return;
    }

    std::string flightNumber;

    std::cout
        << "Enter Flight Number: ";

    std::getline(
        std::cin,
        flightNumber
    );

    std::shared_ptr<Flight> flight;

    for (const auto& flightItem : flights)
    {
        if (flightItem &&
            flightItem->getFlightNumber()
                == flightNumber)
        {
            flight = flightItem;
            break;
        }
    }

    if (!flight)
    {
        std::cout
            << "\nFlight not found.\n";

        pause();
        return;
    }

    while (true)
    {
        clearScreen();

        std::cout
            << "=====================================\n"
            << "             Update Flight\n"
            << "=====================================\n\n"
            << "Flight: "
            << flight->getFlightNumber()
            << "\n\n"
            << "1. Update Price\n"
            << "2. Change Aircraft\n"
            << "3. Back\n\n"
            << "Choose an option: ";

        int choice;

        if (!(std::cin >> choice))
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout
                << "\nInvalid input.\n";

            pause();
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        if (choice == 1)
        {
            double newPrice;

            std::cout
                << "\nCurrent Price: "
                << flight->getPrice()
                << "\n";

            std::cout
                << "New Price: ";

            if (!(std::cin >> newPrice))
            {
                std::cin.clear();

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );

                std::cout
                    << "\nInvalid price.\n";

                pause();
                continue;
            }

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            try
            {
                flight->setPrice(newPrice);

                FlightRepository flightRepository;

                flightRepository.save(flights);

                std::cout
                    << "\nPrice updated successfully.\n";

                pause();
            }
            catch (const std::exception& e)
            {
                std::cout
                    << "\nFailed to update price.\n"
                    << e.what()
                    << "\n";

                pause();
            }
        }
        else if (choice == 2)
        {
            if (aircraft.empty())
            {
                std::cout
                    << "\nNo aircraft available.\n";

                pause();
                continue;
            }

            std::cout
                << "\nAvailable Aircraft:\n\n";

            for (const auto& aircraftItem : aircraft)
            {
                if (!aircraftItem)
                {
                    continue;
                }

                std::cout
                    << "ID: "
                    << aircraftItem->getId()
                    << " | "
                    << aircraftItem->getManufacturer()
                    << " "
                    << aircraftItem->getModel()
                    << " | Registration: "
                    << aircraftItem->getRegistrationNumber()
                    << " | Available: "
                    << (
                        aircraftItem->isAvailable()
                            ? "Yes"
                            : "No"
                    )
                    << "\n";
            }

            int aircraftId;

            std::cout
                << "\nNew Aircraft ID: ";

            if (!(std::cin >> aircraftId))
            {
                std::cin.clear();

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );

                std::cout
                    << "\nInvalid aircraft ID.\n";

                pause();
                continue;
            }

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::shared_ptr<Aircraft> selectedAircraft;

            for (const auto& aircraftItem : aircraft)
            {
                if (aircraftItem &&
                    aircraftItem->getId() == aircraftId)
                {
                    selectedAircraft = aircraftItem;
                    break;
                }
            }

            if (!selectedAircraft)
            {
                std::cout
                    << "\nAircraft not found.\n";

                pause();
                continue;
            }

            if (!selectedAircraft->isAvailable())
            {
                std::cout
                    << "\nSelected aircraft is not available.\n";

                pause();
                continue;
            }

            try
            {
                flight->setAircraft(
                    selectedAircraft
                );

                FlightRepository flightRepository;

                flightRepository.save(flights);

                std::cout
                    << "\nAircraft updated successfully.\n";

                pause();
            }
            catch (const std::exception& e)
            {
                std::cout
                    << "\nFailed to update aircraft.\n"
                    << e.what()
                    << "\n";

                pause();
            }
        }
        else if (choice == 3)
        {
            return;
        }
        else
        {
            std::cout
                << "\nInvalid option.\n";

            pause();
        }
    }
}

// ============================================================
// DELETE FLIGHT
// ============================================================

void ConsoleUI::deleteFlight()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "             Delete Flight\n"
        << "=====================================\n\n";

    if (flights.empty())
    {
        std::cout
            << "No flights found.\n";

        pause();
        return;
    }

    std::string flightNumber;

    std::cout
        << "Enter Flight Number: ";

    std::getline(
        std::cin,
        flightNumber
    );

    auto flightIt =
        std::find_if(
            flights.begin(),
            flights.end(),
            [&flightNumber](
                const std::shared_ptr<Flight>& flight
            )
            {
                return flight &&
                       flight->getFlightNumber()
                           == flightNumber;
            }
        );

    if (flightIt == flights.end())
    {
        std::cout
            << "\nFlight not found.\n";

        pause();
        return;
    }

    auto flight = *flightIt;

    std::cout
        << "\nFlight: "
        << flight->getFlightNumber()
        << "\n"
        << "From: "
        << flight->getOrigin()
        << "\n"
        << "To: "
        << flight->getDestination()
        << "\n"
        << "Status: "
        << flightStatusToString(
            flight->getStatus()
        )
        << "\n";

    std::cout
        << "\nAre you sure you want to delete this flight?\n"
        << "1. Yes\n"
        << "2. No\n"
        << "Choose: ";

    int confirmation;

    if (!(std::cin >> confirmation))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid input.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    if (confirmation != 1)
    {
        std::cout
            << "\nDeletion cancelled.\n";

        pause();
        return;
    }

    flights.erase(flightIt);

    try
    {
        FlightRepository flightRepository;

        flightRepository.save(flights);

        std::cout
            << "\nFlight deleted successfully.\n";
    }
    catch (const std::exception& e)
    {
        std::cout
            << "\nFailed to save flight data.\n"
            << e.what()
            << "\n";
    }

    pause();
}

// ============================================================
// SEARCH FLIGHTS
// ============================================================

void ConsoleUI::searchFlights() const
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "           Search Flights\n"
        << "=====================================\n\n";

    std::string destination;
    std::string date;
    double maximumPrice;

    std::cout << "Destination: ";
    std::getline(std::cin, destination);

    std::cout << "Departure Date (YYYY-MM-DD): ";
    std::getline(std::cin, date);

    std::cout << "Maximum Price: ";

    if (!(std::cin >> maximumPrice))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid price.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    try
    {
        FlightSearchService searchService;

        auto results =
            searchService.search(
                flights,
                destination,
                date,
                maximumPrice
            );

        std::cout
            << "\n=====================================\n"
            << "           Search Results\n"
            << "=====================================\n\n";

        if (results.empty())
        {
            std::cout
                << "No flights found matching "
                << "your search criteria.\n";

            pause();
            return;
        }

        for (const auto& flight : results)
        {
            if (flight == nullptr)
            {
                continue;
            }

            std::cout
                << "-------------------------------------\n"
                << "Flight Number : "
                << flight->getFlightNumber()
                << "\n"
                << "Origin        : "
                << flight->getOrigin()
                << "\n"
                << "Destination   : "
                << flight->getDestination()
                << "\n"
                << "Departure     : "
                << flight->getDepartureTime()
                << "\n"
                << "Arrival       : "
                << flight->getArrivalTime()
                << "\n"
                << "Price         : "
                << flight->getPrice()
                << "\n"
                << "Status        : "
                << flightStatusToString(
                    flight->getStatus()
                )
                << "\n"
                << "-------------------------------------\n";
        }
    }
    catch (const std::exception& exception)
    {
        std::cout
            << "\nSearch failed: "
            << exception.what()
            << "\n";
    }

    pause();
}

void ConsoleUI::createReservation()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "         Create Reservation\n"
        << "=====================================\n\n";

    // ========================================================
    // 1. Select Passenger
    // ========================================================

    std::vector<std::shared_ptr<Passenger>> passengers;

    for (const auto& user : users)
    {
        auto passenger =
            std::dynamic_pointer_cast<Passenger>(user);

        if (passenger != nullptr)
        {
            passengers.push_back(passenger);
        }
    }

    if (passengers.empty())
    {
        std::cout
            << "No passengers available.\n";

        pause();
        return;
    }

    std::cout
        << "Available Passengers:\n\n";

    for (std::size_t i = 0; i < passengers.size(); ++i)
    {
        std::cout
            << i + 1
            << ". "
            << passengers[i]->getFullName()
            << " | Passport: "
            << passengers[i]->getPassportNumber()
            << "\n";
    }

    std::cout << "\nSelect Passenger: ";

    int passengerChoice;

    if (!(std::cin >> passengerChoice))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid input.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    if (passengerChoice < 1 ||
        passengerChoice >
            static_cast<int>(passengers.size()))
    {
        std::cout
            << "\nInvalid passenger selection.\n";

        pause();
        return;
    }

    auto passenger =
        passengers[passengerChoice - 1];

    // ========================================================
    // 2. Select Flight
    // ========================================================

    std::cout
        << "\nAvailable Flights:\n\n";

    std::vector<std::shared_ptr<Flight>> availableFlights;

    for (const auto& flight : flights)
    {
        if (flight == nullptr)
        {
            continue;
        }

        if (flight->getStatus() ==
                FlightStatus::Cancelled ||
            flight->getStatus() ==
                FlightStatus::Completed)
        {
            continue;
        }

        availableFlights.push_back(flight);
    }

    if (availableFlights.empty())
    {
        std::cout
            << "No available flights.\n";

        pause();
        return;
    }

    for (std::size_t i = 0;
         i < availableFlights.size();
         ++i)
    {
        const auto& flight =
            availableFlights[i];

        std::cout
            << i + 1
            << ". "
            << flight->getFlightNumber()
            << " | "
            << flight->getOrigin()
            << " -> "
            << flight->getDestination()
            << " | Departure: "
            << flight->getDepartureTime()
            << " | Price: "
            << flight->getPrice()
            << " | Available Seats: "
            << flight->getAvailableSeats()
            << "\n";
    }

    std::cout
        << "\nSelect Flight: ";

    int flightChoice;

    if (!(std::cin >> flightChoice))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid input.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    if (flightChoice < 1 ||
        flightChoice >
            static_cast<int>(
                availableFlights.size()
            ))
    {
        std::cout
            << "\nInvalid flight selection.\n";

        pause();
        return;
    }

    auto flight =
        availableFlights[flightChoice - 1];

    // ========================================================
    // 3. Select Seat
    // ========================================================

    std::cout
        << "\n=====================================\n"
        << "Seat Selection\n"
        << "=====================================\n\n";

    std::cout
        << "Available Seats: "
        << flight->getAvailableSeats()
        << "\n";

    std::cout
        << "Enter Seat Number "
        << "(example: 12A): ";

    std::string seatNumber;

    std::getline(
        std::cin,
        seatNumber
    );

    if (seatNumber.empty())
    {
        std::cout
            << "\nSeat number cannot be empty.\n";

        pause();
        return;
    }

    // ========================================================
    // 4. Booking Date
    // ========================================================

    std::cout
        << "Booking Date (YYYY-MM-DD): ";

    std::string bookingDate;

    std::getline(
        std::cin,
        bookingDate
    );

    if (bookingDate.empty())
    {
        std::cout
            << "\nBooking date cannot be empty.\n";

        pause();
        return;
    }

    // ========================================================
    // 5. Payment Method
    // ========================================================

    std::cout
        << "\nPayment Method:\n"
        << "1. Cash\n"
        << "2. Card\n"
        << "3. Bank Transfer\n"
        << "\nChoose Payment Method: ";

    int paymentChoice;

    if (!(std::cin >> paymentChoice))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid payment method.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    PaymentMethod paymentMethod;

    switch (paymentChoice)
    {
        case 1:
            paymentMethod = PaymentMethod::Cash;
            break;

        case 2:
            paymentMethod = PaymentMethod::Card;
            break;

        case 3:
            paymentMethod =
                PaymentMethod::BankTransfer;
            break;

        default:
            std::cout
                << "\nInvalid payment method.\n";

            pause();
            return;
    }

    // ========================================================
    // 6. Create Reservation
    // ========================================================

    try
    {
        auto reservation =
            bookingService.createReservation(
                passenger,
                flight,
                seatNumber,
                bookingDate,
                paymentMethod
            );

        if (reservation == nullptr)
        {
            std::cout
                << "\nFailed to create reservation.\n";

            pause();
            return;
        }

        std::cout
            << "\n=====================================\n"
            << "       Reservation Result\n"
            << "=====================================\n\n";

        std::cout
            << "Reservation ID : "
            << reservation->getId()
            << "\n"
            << "Passenger      : "
            << passenger->getFullName()
            << "\n"
            << "Flight         : "
            << flight->getFlightNumber()
            << "\n"
            << "Route          : "
            << flight->getOrigin()
            << " -> "
            << flight->getDestination()
            << "\n"
            << "Seat           : "
            << reservation->getSeatNumber()
            << "\n"
            << "Price          : "
            << reservation->getTotalPrice()
            << "\n";

        if (reservation->getStatus() ==
            ReservationStatus::Confirmed)
        {
            std::cout
                << "Status         : Confirmed\n"
                << "Payment        : Completed\n";
        }
        else if (
            reservation->getStatus() ==
            ReservationStatus::Waitlisted)
        {
            std::cout
                << "Status         : Waitlisted\n"
                << "Payment        : Pending\n";
        }

        std::cout
            << "\nReservation created successfully.\n";
    }
    catch (const std::exception& exception)
    {
        std::cout
            << "\nReservation failed: "
            << exception.what()
            << "\n";
    }

    pause();
}

void ConsoleUI::passengerCreateReservation()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "         Make Reservation\n"
        << "=====================================\n\n";

    // ========================================================
    // 1. Get Current Passenger
    // ========================================================

    auto passenger =
        std::dynamic_pointer_cast<Passenger>(currentUser);

    if (passenger == nullptr)
    {
        std::cout
            << "Current user is not a passenger.\n";

        pause();
        return;
    }

    std::cout
        << "Passenger: "
        << passenger->getFullName()
        << "\n"
        << "Passport: "
        << passenger->getPassportNumber()
        << "\n\n";

    // ========================================================
    // 2. Select Flight
    // ========================================================

    std::cout
        << "Available Flights:\n\n";

    std::vector<std::shared_ptr<Flight>> availableFlights;

    for (const auto& flight : flights)
    {
        if (flight == nullptr)
        {
            continue;
        }

        if (flight->getStatus() ==
                FlightStatus::Cancelled ||
            flight->getStatus() ==
                FlightStatus::Completed)
        {
            continue;
        }

        availableFlights.push_back(flight);
    }

    if (availableFlights.empty())
    {
        std::cout
            << "No available flights.\n";

        pause();
        return;
    }

    for (std::size_t i = 0;
         i < availableFlights.size();
         ++i)
    {
        const auto& flight =
            availableFlights[i];

        std::cout
            << i + 1
            << ". "
            << flight->getFlightNumber()
            << " | "
            << flight->getOrigin()
            << " -> "
            << flight->getDestination()
            << " | Departure: "
            << flight->getDepartureTime()
            << " | Price: "
            << flight->getPrice()
            << " | Available Seats: "
            << flight->getAvailableSeats()
            << "\n";
    }

    std::cout
        << "\nSelect Flight: ";

    int flightChoice;

    if (!(std::cin >> flightChoice))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid input.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    if (flightChoice < 1 ||
        flightChoice >
            static_cast<int>(
                availableFlights.size()
            ))
    {
        std::cout
            << "\nInvalid flight selection.\n";

        pause();
        return;
    }

    auto flight =
        availableFlights[flightChoice - 1];

    // ========================================================
    // 3. Select Seat
    // ========================================================

    std::cout
        << "\n=====================================\n"
        << "Seat Selection\n"
        << "=====================================\n\n";

    std::cout
        << "Available Seats: "
        << flight->getAvailableSeats()
        << "\n";

    std::cout
        << "Enter Seat Number "
        << "(example: 12A): ";

    std::string seatNumber;

    std::getline(
        std::cin,
        seatNumber
    );

    if (seatNumber.empty())
    {
        std::cout
            << "\nSeat number cannot be empty.\n";

        pause();
        return;
    }

    // ========================================================
    // 4. Booking Date
    // ========================================================

    std::cout
        << "Booking Date (YYYY-MM-DD): ";

    std::string bookingDate;

    std::getline(
        std::cin,
        bookingDate
    );

    if (bookingDate.empty())
    {
        std::cout
            << "\nBooking date cannot be empty.\n";

        pause();
        return;
    }

    // ========================================================
    // 5. Payment Method
    // ========================================================

    std::cout
        << "\nPayment Method:\n"
        << "1. Cash\n"
        << "2. Card\n"
        << "3. Bank Transfer\n"
        << "\nChoose Payment Method: ";

    int paymentChoice;

    if (!(std::cin >> paymentChoice))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid payment method.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    PaymentMethod paymentMethod;

    switch (paymentChoice)
    {
        case 1:
            paymentMethod = PaymentMethod::Cash;
            break;

        case 2:
            paymentMethod = PaymentMethod::Card;
            break;

        case 3:
            paymentMethod =
                PaymentMethod::BankTransfer;
            break;

        default:
            std::cout
                << "\nInvalid payment method.\n";

            pause();
            return;
    }

    // ========================================================
    // 6. Create Reservation
    // ========================================================

    try
    {
        auto reservation =
            bookingService.createReservation(
                passenger,
                flight,
                seatNumber,
                bookingDate,
                paymentMethod
            );

        if (reservation == nullptr)
        {
            std::cout
                << "\nFailed to create reservation.\n";

            pause();
            return;
        }

        std::cout
            << "\n=====================================\n"
            << "       Reservation Result\n"
            << "=====================================\n\n";

        std::cout
            << "Reservation ID : "
            << reservation->getId()
            << "\n"
            << "Passenger      : "
            << passenger->getFullName()
            << "\n"
            << "Flight         : "
            << flight->getFlightNumber()
            << "\n"
            << "Route          : "
            << flight->getOrigin()
            << " -> "
            << flight->getDestination()
            << "\n"
            << "Seat           : "
            << reservation->getSeatNumber()
            << "\n"
            << "Price          : "
            << reservation->getTotalPrice()
            << "\n";

        if (reservation->getStatus() ==
            ReservationStatus::Confirmed)
        {
            std::cout
                << "Status         : Confirmed\n"
                << "Payment        : Completed\n";
        }
        else if (
            reservation->getStatus() ==
            ReservationStatus::Waitlisted)
        {
            std::cout
                << "Status         : Waitlisted\n"
                << "Payment        : Pending\n";
        }

        std::cout
            << "\nReservation created successfully.\n";
    }
    catch (const std::exception& exception)
    {
        std::cout
            << "\nReservation failed: "
            << exception.what()
            << "\n";
    }

    pause();
}

void ConsoleUI::passengerManageReservations()
{
    while (currentUser)
    {
        clearScreen();

        std::cout
            << "=====================================\n"
            << "        Manage My Reservations\n"
            << "=====================================\n\n";

        auto passenger =
            std::dynamic_pointer_cast<Passenger>(
                currentUser
            );

        if (passenger == nullptr)
        {
            std::cout
                << "Current user is not a passenger.\n";

            pause();
            return;
        }

        std::cout
            << "Passenger: "
            << passenger->getFullName()
            << "\n\n";

        const auto& reservations =
            bookingService.getAllReservations();

        bool hasReservations = false;

        for (const auto& reservation : reservations)
        {
            if (reservation == nullptr)
            {
                continue;
            }

            auto reservationPassenger =
                reservation->getPassenger();

            if (reservationPassenger == nullptr ||
                reservationPassenger->getId() !=
                    passenger->getId())
            {
                continue;
            }

            hasReservations = true;

            auto flight =
                reservation->getFlight();

            std::cout
                << "-------------------------------------\n"
                << "Reservation ID : "
                << reservation->getId()
                << "\n"
                << "Flight         : "
                << (flight != nullptr
                        ? flight->getFlightNumber()
                        : "Unknown")
                << "\n"
                << "Route          : "
                << (flight != nullptr
                        ? flight->getOrigin() +
                              " -> " +
                              flight->getDestination()
                        : "Unknown")
                << "\n"
                << "Seat           : "
                << reservation->getSeatNumber()
                << "\n"
                << "Price          : "
                << reservation->getTotalPrice()
                << "\n"
                << "Status         : ";

            if (reservation->getStatus() ==
                ReservationStatus::Confirmed)
            {
                std::cout << "Confirmed";
            }
            else if (
                reservation->getStatus() ==
                ReservationStatus::Waitlisted)
            {
                std::cout << "Waitlisted";
            }
            else
            {
                std::cout << "Cancelled";
            }

            std::cout
                << "\n"
                << "-------------------------------------\n";
        }

        if (!hasReservations)
        {
            std::cout
                << "You do not have any reservations.\n\n";

            std::cout
                << "1. Back\n\n"
                << "Choose an option: ";

            int choice;

            if (!(std::cin >> choice))
            {
                std::cin.clear();

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );

                continue;
            }

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            if (choice == 1)
            {
                return;
            }

            continue;
        }

        std::cout
            << "\n"
            << "1. View Reservation Details\n"
            << "2. Modify Reservation\n"
            << "3. Cancel Reservation\n"
            << "4. Back\n\n"
            << "Choose an option: ";

        int choice;

        if (!(std::cin >> choice))
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout
                << "\nInvalid input.\n";

            pause();
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        switch (choice)
        {
            case 1:
            {
                std::cout
                    << "\nEnter Reservation ID: ";

                int reservationId;

                if (!(std::cin >> reservationId))
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout
                        << "\nInvalid reservation ID.\n";

                    pause();
                    break;
                }

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );

                auto reservation =
                    bookingService.getReservation(
                        reservationId
                    );

                if (reservation == nullptr ||
                    reservation->getPassenger() == nullptr ||
                    reservation->getPassenger()->getId() !=
                        passenger->getId())
                {
                    std::cout
                        << "\nReservation not found "
                        << "in your account.\n";

                    pause();
                    break;
                }

                auto flight =
                    reservation->getFlight();

                std::cout
                    << "\n=====================================\n"
                    << "       Reservation Details\n"
                    << "=====================================\n\n"
                    << "Reservation ID : "
                    << reservation->getId()
                    << "\n"
                    << "Passenger      : "
                    << passenger->getFullName()
                    << "\n"
                    << "Flight         : "
                    << (flight != nullptr
                            ? flight->getFlightNumber()
                            : "Unknown")
                    << "\n"
                    << "Route          : "
                    << (flight != nullptr
                            ? flight->getOrigin() +
                              " -> " +
                              flight->getDestination()
                            : "Unknown")
                    << "\n"
                    << "Seat           : "
                    << reservation->getSeatNumber()
                    << "\n"
                    << "Price          : "
                    << reservation->getTotalPrice()
                    << "\n"
                    << "Status         : ";

                if (reservation->getStatus() ==
                    ReservationStatus::Confirmed)
                {
                    std::cout << "Confirmed";
                }
                else if (
                    reservation->getStatus() ==
                    ReservationStatus::Waitlisted)
                {
                    std::cout << "Waitlisted";
                }
                else
                {
                    std::cout << "Cancelled";
                }

                std::cout << "\n";

                pause();
                break;
            }

            case 2:
                passengerModifyReservation();
                break;

            case 3:
                passengerCancelReservation();
                break;

            case 4:
                return;

            default:
                std::cout
                    << "\nInvalid option.\n";

                pause();
                break;
        }
    }
}

void ConsoleUI::passengerModifyReservation()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "      Modify My Reservation\n"
        << "=====================================\n\n";

    auto passenger =
        std::dynamic_pointer_cast<Passenger>(
            currentUser
        );

    if (passenger == nullptr)
    {
        std::cout
            << "Current user is not a passenger.\n";

        pause();
        return;
    }

    const auto& reservations =
        bookingService.getAllReservations();

    bool hasReservations = false;

    for (const auto& reservation : reservations)
    {
        if (reservation == nullptr ||
            reservation->getPassenger() == nullptr)
        {
            continue;
        }

        if (reservation->getPassenger()->getId() !=
            passenger->getId())
        {
            continue;
        }

        hasReservations = true;

        auto flight =
            reservation->getFlight();

        std::cout
            << "-------------------------------------\n"
            << "Reservation ID : "
            << reservation->getId()
            << "\n"
            << "Flight         : "
            << (flight != nullptr
                    ? flight->getFlightNumber()
                    : "Unknown")
            << "\n"
            << "Seat           : "
            << reservation->getSeatNumber()
            << "\n"
            << "Status         : ";

        if (reservation->getStatus() ==
            ReservationStatus::Confirmed)
        {
            std::cout << "Confirmed";
        }
        else if (
            reservation->getStatus() ==
            ReservationStatus::Waitlisted)
        {
            std::cout << "Waitlisted";
        }
        else
        {
            std::cout << "Cancelled";
        }

        std::cout
            << "\n"
            << "-------------------------------------\n";
    }

    if (!hasReservations)
    {
        std::cout
            << "You do not have any reservations.\n";

        pause();
        return;
    }

    std::cout
        << "\nEnter Reservation ID: ";

    int reservationId;

    if (!(std::cin >> reservationId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid reservation ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto reservation =
        bookingService.getReservation(
            reservationId
        );

    if (reservation == nullptr ||
        reservation->getPassenger() == nullptr ||
        reservation->getPassenger()->getId() !=
            passenger->getId())
    {
        std::cout
            << "\nReservation not found in your account.\n";

        pause();
        return;
    }

    if (reservation->getStatus() ==
        ReservationStatus::Cancelled)
    {
        std::cout
            << "\nCannot modify a cancelled reservation.\n";

        pause();
        return;
    }

    if (reservation->getStatus() ==
        ReservationStatus::Waitlisted)
    {
        std::cout
            << "\nCannot modify a waitlisted reservation.\n";

        pause();
        return;
    }

    auto flight =
        reservation->getFlight();

    if (flight == nullptr)
    {
        std::cout
            << "\nFlight information is unavailable.\n";

        pause();
        return;
    }

    std::cout
        << "\n=====================================\n"
        << "Current Reservation\n"
        << "=====================================\n"
        << "Flight         : "
        << flight->getFlightNumber()
        << "\n"
        << "Route          : "
        << flight->getOrigin()
        << " -> "
        << flight->getDestination()
        << "\n"
        << "Current Seat   : "
        << reservation->getSeatNumber()
        << "\n"
        << "Available Seats: "
        << flight->getAvailableSeats()
        << "\n";

    std::cout
        << "\nEnter New Seat Number "
        << "(example: 12B): ";

    std::string newSeatNumber;

    std::getline(
        std::cin,
        newSeatNumber
    );

    if (newSeatNumber.empty())
    {
        std::cout
            << "\nSeat number cannot be empty.\n";

        pause();
        return;
    }

    const std::string oldSeatNumber =
        reservation->getSeatNumber();

    try
    {
        const bool modified =
            bookingService.modifyReservation(
                reservationId,
                newSeatNumber
            );

        if (!modified)
        {
            std::cout
                << "\nFailed to modify reservation.\n"
                << "The new seat may be unavailable.\n";

            pause();
            return;
        }

        std::cout
            << "\n=====================================\n"
            << " Reservation Modified Successfully\n"
            << "=====================================\n\n"
            << "Reservation ID : "
            << reservation->getId()
            << "\n"
            << "Flight         : "
            << flight->getFlightNumber()
            << "\n"
            << "Old Seat       : "
            << oldSeatNumber
            << "\n"
            << "New Seat       : "
            << reservation->getSeatNumber()
            << "\n"
            << "Status         : Confirmed\n";
    }
    catch (const std::exception& exception)
    {
        std::cout
            << "\nModification failed: "
            << exception.what()
            << "\n";
    }

    pause();
}

void ConsoleUI::passengerCancelReservation()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "       Cancel My Reservation\n"
        << "=====================================\n\n";

    auto passenger =
        std::dynamic_pointer_cast<Passenger>(
            currentUser
        );

    if (passenger == nullptr)
    {
        std::cout
            << "Current user is not a passenger.\n";

        pause();
        return;
    }

    const auto& reservations =
        bookingService.getAllReservations();

    bool hasReservations = false;

    for (const auto& reservation : reservations)
    {
        if (reservation == nullptr ||
            reservation->getPassenger() == nullptr)
        {
            continue;
        }

        if (reservation->getPassenger()->getId() !=
            passenger->getId())
        {
            continue;
        }

        hasReservations = true;

        auto flight =
            reservation->getFlight();

        std::cout
            << "-------------------------------------\n"
            << "Reservation ID : "
            << reservation->getId()
            << "\n"
            << "Flight         : "
            << (flight != nullptr
                    ? flight->getFlightNumber()
                    : "Unknown")
            << "\n"
            << "Seat           : "
            << reservation->getSeatNumber()
            << "\n"
            << "Price          : "
            << reservation->getTotalPrice()
            << "\n"
            << "Status         : ";

        if (reservation->getStatus() ==
            ReservationStatus::Confirmed)
        {
            std::cout << "Confirmed";
        }
        else if (
            reservation->getStatus() ==
            ReservationStatus::Waitlisted)
        {
            std::cout << "Waitlisted";
        }
        else
        {
            std::cout << "Cancelled";
        }

        std::cout
            << "\n"
            << "-------------------------------------\n";
    }

    if (!hasReservations)
    {
        std::cout
            << "You do not have any reservations.\n";

        pause();
        return;
    }

    std::cout
        << "\nEnter Reservation ID: ";

    int reservationId;

    if (!(std::cin >> reservationId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid reservation ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto reservation =
        bookingService.getReservation(
            reservationId
        );

    if (reservation == nullptr ||
        reservation->getPassenger() == nullptr ||
        reservation->getPassenger()->getId() !=
            passenger->getId())
    {
        std::cout
            << "\nReservation not found in your account.\n";

        pause();
        return;
    }

    if (reservation->getStatus() ==
        ReservationStatus::Cancelled)
    {
        std::cout
            << "\nReservation is already cancelled.\n";

        pause();
        return;
    }

    auto flight =
        reservation->getFlight();

    std::cout
        << "\n=====================================\n"
        << "Reservation Details\n"
        << "=====================================\n"
        << "Reservation ID : "
        << reservation->getId()
        << "\n"
        << "Flight         : "
        << (flight != nullptr
                ? flight->getFlightNumber()
                : "Unknown")
        << "\n"
        << "Seat           : "
        << reservation->getSeatNumber()
        << "\n"
        << "Price          : "
        << reservation->getTotalPrice()
        << "\n";

    std::cout
        << "\nAre you sure you want to cancel "
        << "this reservation? (Y/N): ";

    char confirmation;

    std::cin >> confirmation;

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    if (confirmation != 'Y' &&
        confirmation != 'y')
    {
        std::cout
            << "\nCancellation aborted.\n";

        pause();
        return;
    }

    try
    {
        const bool cancelled =
            bookingService.cancelReservation(
                reservationId
            );

        if (!cancelled)
        {
            std::cout
                << "\nFailed to cancel reservation.\n";

            pause();
            return;
        }

        std::cout
            << "\n=====================================\n"
            << " Reservation Cancelled Successfully\n"
            << "=====================================\n\n"
            << "Reservation ID : "
            << reservationId
            << "\n"
            << "Status         : Cancelled\n"
            << "\n"
            << "Payment refund processed successfully.\n";
    }
    catch (const std::exception& exception)
    {
        std::cout
            << "\nCancellation failed: "
            << exception.what()
            << "\n";
    }

    pause();
}

void ConsoleUI::passengerOnlineCheckIn()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "          Online Check-In\n"
        << "=====================================\n\n";

    auto passenger =
        std::dynamic_pointer_cast<Passenger>(
            currentUser
        );

    if (passenger == nullptr)
    {
        std::cout
            << "Current user is not a passenger.\n";

        pause();
        return;
    }

    const auto& reservations =
        bookingService.getAllReservations();

    bool hasReservations = false;

    // ========================================================
    // 1. Display Passenger Reservations
    // ========================================================

    std::cout
        << "Your Reservations:\n\n";

    for (const auto& reservation : reservations)
    {
        if (reservation == nullptr ||
            reservation->getPassenger() == nullptr)
        {
            continue;
        }

        if (reservation->getPassenger()->getId() !=
            passenger->getId())
        {
            continue;
        }

        hasReservations = true;

        auto flight =
            reservation->getFlight();

        std::cout
            << "-------------------------------------\n"
            << "Reservation ID : "
            << reservation->getId()
            << "\n"
            << "Flight         : "
            << (flight != nullptr
                    ? flight->getFlightNumber()
                    : "Unknown")
            << "\n"
            << "Route          : "
            << (flight != nullptr
                    ? flight->getOrigin() +
                      " -> " +
                      flight->getDestination()
                    : "Unknown")
            << "\n"
            << "Seat           : "
            << reservation->getSeatNumber()
            << "\n"
            << "Status         : ";

        if (reservation->getStatus() ==
            ReservationStatus::Confirmed)
        {
            std::cout << "Confirmed";
        }
        else if (
            reservation->getStatus() ==
            ReservationStatus::Waitlisted)
        {
            std::cout << "Waitlisted";
        }
        else
        {
            std::cout << "Cancelled";
        }

        std::cout
            << "\n"
            << "-------------------------------------\n";
    }

    if (!hasReservations)
    {
        std::cout
            << "You do not have any reservations.\n";

        pause();
        return;
    }

    // ========================================================
    // 2. Select Reservation
    // ========================================================

    std::cout
        << "\nEnter Reservation ID: ";

    int reservationId;

    if (!(std::cin >> reservationId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid reservation ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto reservation =
        bookingService.getReservation(
            reservationId
        );

    // ========================================================
    // 3. Validate Ownership
    // ========================================================

    if (reservation == nullptr ||
        reservation->getPassenger() == nullptr ||
        reservation->getPassenger()->getId() !=
            passenger->getId())
    {
        std::cout
            << "\nReservation not found in your account.\n";

        pause();
        return;
    }

    // ========================================================
    // 4. Validate Reservation
    // ========================================================

    if (reservation->getStatus() ==
        ReservationStatus::Cancelled)
    {
        std::cout
            << "\nCannot check in a cancelled reservation.\n";

        pause();
        return;
    }

    if (reservation->getStatus() ==
        ReservationStatus::Waitlisted)
    {
        std::cout
            << "\nCannot check in a waitlisted reservation.\n";

        pause();
        return;
    }

    auto flight =
        reservation->getFlight();

    if (flight == nullptr)
    {
        std::cout
            << "\nFlight information is unavailable.\n";

        pause();
        return;
    }

    // ========================================================
    // 5. Check Existing Check-In
    // ========================================================

    auto existingCheckIn =
        checkInService.getCheckInByReservationId(
            reservationId
        );

    if (existingCheckIn != nullptr)
    {
        std::cout
            << "\nThis reservation is already checked in.\n";

        auto boardingPass =
            checkInService.getBoardingPassByReservationId(
                reservationId
            );

        if (boardingPass != nullptr)
        {
            std::cout
                << "Boarding Pass : "
                << boardingPass->getPassNumber()
                << "\n";
        }

        pause();
        return;
    }

    // ========================================================
    // 6. Display Flight Information
    // ========================================================

    std::cout
        << "\n=====================================\n"
        << "       Check-In Information\n"
        << "=====================================\n"
        << "Reservation ID : "
        << reservation->getId()
        << "\n"
        << "Passenger      : "
        << passenger->getFullName()
        << "\n"
        << "Flight         : "
        << flight->getFlightNumber()
        << "\n"
        << "Route          : "
        << flight->getOrigin()
        << " -> "
        << flight->getDestination()
        << "\n"
        << "Seat           : "
        << reservation->getSeatNumber()
        << "\n";

    // ========================================================
    // 7. Check-In Information
    // ========================================================

    std::string checkInDateTime;
    std::string gate;
    std::string boardingTime;

    std::cout
        << "\nCheck-In Date & Time: ";

    std::getline(
        std::cin,
        checkInDateTime
    );

    if (checkInDateTime.empty())
    {
        std::cout
            << "\nCheck-in date/time cannot be empty.\n";

        pause();
        return;
    }

    std::cout
        << "Gate: ";

    std::getline(
        std::cin,
        gate
    );

    if (gate.empty())
    {
        std::cout
            << "\nGate cannot be empty.\n";

        pause();
        return;
    }

    std::cout
        << "Boarding Time: ";

    std::getline(
        std::cin,
        boardingTime
    );

    if (boardingTime.empty())
    {
        std::cout
            << "\nBoarding time cannot be empty.\n";

        pause();
        return;
    }

    // ========================================================
    // 8. Perform Online Check-In
    // ========================================================

    try
    {
        auto checkIn =
            checkInService.onlineCheckIn(
                reservation,
                checkInDateTime,
                gate,
                boardingTime
            );

        if (checkIn == nullptr)
        {
            std::cout
                << "\nFailed to complete online check-in.\n";

            pause();
            return;
        }

        auto boardingPass =
            checkInService.getBoardingPassByReservationId(
                reservationId
            );

        std::cout
            << "\n=====================================\n"
            << " Online Check-In Successful\n"
            << "=====================================\n\n"
            << "Reservation ID : "
            << reservationId
            << "\n"
            << "Flight         : "
            << flight->getFlightNumber()
            << "\n"
            << "Seat           : "
            << reservation->getSeatNumber()
            << "\n"
            << "Check-In Type  : Online\n";

        if (boardingPass != nullptr)
        {
            std::cout
                << "Boarding Pass  : "
                << boardingPass->getPassNumber()
                << "\n"
                << "Gate           : "
                << gate
                << "\n"
                << "Boarding Time  : "
                << boardingTime
                << "\n";
        }

        std::cout
            << "\nOnline check-in completed successfully.\n";
    }
    catch (const std::exception& exception)
    {
        std::cout
            << "\nCheck-in failed: "
            << exception.what()
            << "\n";
    }

    pause();
}

void ConsoleUI::passengerViewBoardingPass()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "          View Boarding Pass\n"
        << "=====================================\n\n";

    auto passenger =
        std::dynamic_pointer_cast<Passenger>(
            currentUser
        );

    if (passenger == nullptr)
    {
        std::cout
            << "Current user is not a passenger.\n";

        pause();
        return;
    }

    const auto& reservations =
        bookingService.getAllReservations();

    bool hasCheckIns = false;

    // ========================================================
    // 1. Display Passenger Boarding Passes
    // ========================================================

    for (const auto& reservation : reservations)
    {
        if (reservation == nullptr ||
            reservation->getPassenger() == nullptr)
        {
            continue;
        }

        if (reservation->getPassenger()->getId() !=
            passenger->getId())
        {
            continue;
        }

        auto boardingPass =
            checkInService.getBoardingPassByReservationId(
                reservation->getId()
            );

        if (boardingPass == nullptr)
        {
            continue;
        }

        hasCheckIns = true;

        std::cout
            << "=====================================\n"
            << "           Boarding Pass\n"
            << "=====================================\n"
            << "Pass Number    : "
            << boardingPass->getPassNumber()
            << "\n"
            << "Reservation ID : "
            << boardingPass->getReservationId()
            << "\n"
            << "Passenger      : "
            << boardingPass->getPassengerName()
            << "\n"
            << "Passport       : "
            << boardingPass->getPassportNumber()
            << "\n"
            << "Flight         : "
            << boardingPass->getFlightNumber()
            << "\n"
            << "Seat           : "
            << boardingPass->getSeatNumber()
            << "\n"
            << "Gate           : "
            << boardingPass->getGate()
            << "\n"
            << "Boarding Time  : "
            << boardingPass->getBoardingTime()
            << "\n"
            << "=====================================\n\n";
    }

    // ========================================================
    // 2. No Boarding Pass
    // ========================================================

    if (!hasCheckIns)
    {
        std::cout
            << "No boarding pass is available.\n\n"
            << "You must complete online check-in "
            << "before viewing your boarding pass.\n";
    }

    pause();
}

void ConsoleUI::passengerViewProfile()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "            My Profile\n"
        << "=====================================\n\n";

    auto passenger =
        std::dynamic_pointer_cast<Passenger>(
            currentUser
        );

    if (passenger == nullptr)
    {
        std::cout
            << "Current user is not a passenger.\n";

        pause();
        return;
    }

    passengerService.displayProfile(
        passenger
    );

    pause();
}

void ConsoleUI::passengerLoyaltyProgram()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "          Loyalty Program\n"
        << "=====================================\n\n";

    auto passenger =
        std::dynamic_pointer_cast<Passenger>(
            currentUser
        );

    if (passenger == nullptr)
    {
        std::cout
            << "Current user is not a passenger.\n";

        pause();
        return;
    }

    // ========================================================
    // 1. Display Loyalty Points
    // ========================================================

    std::cout
        << "Passenger: "
        << passenger->getFullName()
        << "\n\n"
        << "Current Loyalty Points: "
        << passenger->getLoyaltyPoints()
        << "\n\n";

    // ========================================================
    // 2. Loyalty Information
    // ========================================================

    std::cout
        << "Loyalty Program Information\n"
        << "-------------------------------------\n"
        << "Points are earned based on reservation price.\n"
        << "Every 100 currency units = 1 loyalty point.\n"
        << "-------------------------------------\n\n";

    // ========================================================
    // 3. Loyalty Menu
    // ========================================================

    std::cout
        << "1. Redeem Loyalty Points\n"
        << "2. Back\n\n"
        << "Choose an option: ";

    int choice;

    if (!(std::cin >> choice))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid input.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    switch (choice)
    {
        case 1:
        {
            const int currentPoints =
                passenger->getLoyaltyPoints();

            if (currentPoints <= 0)
            {
                std::cout
                    << "\nYou do not have enough loyalty points "
                    << "to redeem.\n";

                pause();
                return;
            }

            std::cout
                << "\nCurrent Points: "
                << currentPoints
                << "\n\n"
                << "Enter points to redeem: ";

            int pointsToRedeem;

            if (!(std::cin >> pointsToRedeem))
            {
                std::cin.clear();

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );

                std::cout
                    << "\nInvalid points amount.\n";

                pause();
                return;
            }

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            if (pointsToRedeem <= 0)
            {
                std::cout
                    << "\nPoints to redeem must be positive.\n";

                pause();
                return;
            }

            if (pointsToRedeem > currentPoints)
            {
                std::cout
                    << "\nYou do not have enough loyalty points.\n"
                    << "Available Points: "
                    << currentPoints
                    << "\n";

                pause();
                return;
            }

            const bool redeemed =
                passenger->redeemLoyaltyPoints(
                    pointsToRedeem
                );

            if (!redeemed)
            {
                std::cout
                    << "\nFailed to redeem loyalty points.\n";

                pause();
                return;
            }

            std::cout
                << "\n=====================================\n"
                << "   Loyalty Points Redeemed\n"
                << "=====================================\n\n"
                << "Redeemed Points : "
                << pointsToRedeem
                << "\n"
                << "Remaining Points: "
                << passenger->getLoyaltyPoints()
                << "\n"
                << "\nPoints redeemed successfully.\n";

            pause();
            break;
        }

        case 2:
            return;

        default:
            std::cout
                << "\nInvalid option.\n";

            pause();
            break;
    }
}

void ConsoleUI::modifyReservation()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "        Modify Reservation\n"
        << "=====================================\n\n";

    const auto& reservations =
        bookingService.getAllReservations();

    if (reservations.empty())
    {
        std::cout
            << "No reservations available.\n";

        pause();
        return;
    }

    // ========================================================
    // 1. Display Reservations
    // ========================================================

    std::cout
        << "Existing Reservations:\n\n";

    for (const auto& reservation : reservations)
    {
        if (reservation == nullptr)
        {
            continue;
        }

        auto passenger =
            reservation->getPassenger();

        auto flight =
            reservation->getFlight();

        std::cout
            << "-------------------------------------\n"
            << "Reservation ID : "
            << reservation->getId()
            << "\n"
            << "Passenger      : "
            << (passenger != nullptr
                    ? passenger->getFullName()
                    : "Unknown")
            << "\n"
            << "Flight         : "
            << (flight != nullptr
                    ? flight->getFlightNumber()
                    : "Unknown")
            << "\n"
            << "Seat           : "
            << reservation->getSeatNumber()
            << "\n"
            << "Status         : ";

        if (reservation->getStatus() ==
            ReservationStatus::Confirmed)
        {
            std::cout << "Confirmed";
        }
        else if (
            reservation->getStatus() ==
            ReservationStatus::Waitlisted)
        {
            std::cout << "Waitlisted";
        }
        else
        {
            std::cout << "Cancelled";
        }

        std::cout
            << "\n"
            << "-------------------------------------\n";
    }

    // ========================================================
    // 2. Select Reservation
    // ========================================================

    std::cout
        << "\nEnter Reservation ID: ";

    int reservationId;

    if (!(std::cin >> reservationId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid reservation ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto reservation =
        bookingService.getReservation(
            reservationId
        );

    if (reservation == nullptr)
    {
        std::cout
            << "\nReservation not found.\n";

        pause();
        return;
    }

    // ========================================================
    // 3. Validate Reservation
    // ========================================================

    if (reservation->getStatus() ==
        ReservationStatus::Cancelled)
    {
        std::cout
            << "\nCannot modify a cancelled reservation.\n";

        pause();
        return;
    }

    if (reservation->getStatus() ==
        ReservationStatus::Waitlisted)
    {
        std::cout
            << "\nCannot modify a waitlisted reservation.\n";

        pause();
        return;
    }

    auto flight =
        reservation->getFlight();

    if (flight == nullptr)
    {
        std::cout
            << "\nFlight information is unavailable.\n";

        pause();
        return;
    }

    // ========================================================
    // 4. Display Current Reservation
    // ========================================================

    std::cout
        << "\n=====================================\n"
        << "Current Reservation\n"
        << "=====================================\n"
        << "Reservation ID : "
        << reservation->getId()
        << "\n"
        << "Flight         : "
        << flight->getFlightNumber()
        << "\n"
        << "Route          : "
        << flight->getOrigin()
        << " -> "
        << flight->getDestination()
        << "\n"
        << "Current Seat   : "
        << reservation->getSeatNumber()
        << "\n"
        << "Available Seats: "
        << flight->getAvailableSeats()
        << "\n";

    // ========================================================
    // 5. Select New Seat
    // ========================================================

    std::cout
        << "\nEnter New Seat Number "
        << "(example: 12B): ";

    std::string newSeatNumber;

    std::getline(
        std::cin,
        newSeatNumber
    );

    if (newSeatNumber.empty())
    {
        std::cout
            << "\nSeat number cannot be empty.\n";

        pause();
        return;
    }

    // ========================================================
    // 6. Modify Reservation
    // ========================================================
    const std::string oldSeatNumber =
        reservation->getSeatNumber();

    try
    {
        const bool modified =
            bookingService.modifyReservation(
                reservationId,
                newSeatNumber
            );

        if (!modified)
        {
            std::cout
                << "\nFailed to modify reservation.\n"
                << "The new seat may be unavailable "
                << "or the reservation may not be modifiable.\n";

            pause();
            return;
        }

        std::cout
            << "\n=====================================\n"
            << "    Reservation Modified Successfully\n"
            << "=====================================\n\n"
            << "Reservation ID : "
            << reservation->getId()
            << "\n"
            << "Flight         : "
            << flight->getFlightNumber()
            << "\n"
            << "Old Seat       : "
            << oldSeatNumber
            << "\n"
            << "New Seat       : "
            << reservation->getSeatNumber()
            << "\n"
            << "Status         : Confirmed\n";
    }
    catch (const std::exception& exception)
    {
        std::cout
            << "\nModification failed: "
            << exception.what()
            << "\n";
    }
        pause();
}

void ConsoleUI::cancelReservation()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "        Cancel Reservation\n"
        << "=====================================\n\n";

    const auto& reservations =
        bookingService.getAllReservations();

    if (reservations.empty())
    {
        std::cout
            << "No reservations available.\n";

        pause();
        return;
    }

    // ========================================================
    // 1. Display Reservations
    // ========================================================

    std::cout
        << "Existing Reservations:\n\n";

    for (const auto& reservation : reservations)
    {
        if (reservation == nullptr)
        {
            continue;
        }

        auto passenger =
            reservation->getPassenger();

        auto flight =
            reservation->getFlight();

        std::cout
            << "-------------------------------------\n"
            << "Reservation ID : "
            << reservation->getId()
            << "\n"
            << "Passenger      : "
            << (passenger != nullptr
                    ? passenger->getFullName()
                    : "Unknown")
            << "\n"
            << "Flight         : "
            << (flight != nullptr
                    ? flight->getFlightNumber()
                    : "Unknown")
            << "\n"
            << "Seat           : "
            << reservation->getSeatNumber()
            << "\n"
            << "Price          : "
            << reservation->getTotalPrice()
            << "\n"
            << "Status         : ";

        if (reservation->getStatus() ==
            ReservationStatus::Confirmed)
        {
            std::cout << "Confirmed";
        }
        else if (
            reservation->getStatus() ==
            ReservationStatus::Waitlisted)
        {
            std::cout << "Waitlisted";
        }
        else
        {
            std::cout << "Cancelled";
        }

        std::cout
            << "\n"
            << "-------------------------------------\n";
    }

    // ========================================================
    // 2. Select Reservation
    // ========================================================

    std::cout
        << "\nEnter Reservation ID: ";

    int reservationId;

    if (!(std::cin >> reservationId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid reservation ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto reservation =
        bookingService.getReservation(
            reservationId
        );

    if (reservation == nullptr)
    {
        std::cout
            << "\nReservation not found.\n";

        pause();
        return;
    }

    // ========================================================
    // 3. Validate
    // ========================================================

    if (reservation->getStatus() ==
        ReservationStatus::Cancelled)
    {
        std::cout
            << "\nReservation is already cancelled.\n";

        pause();
        return;
    }

    // ========================================================
    // 4. Confirmation
    // ========================================================

    std::cout
        << "\n=====================================\n"
        << "Reservation Details\n"
        << "=====================================\n"
        << "Reservation ID : "
        << reservation->getId()
        << "\n"
        << "Passenger      : "
        << reservation->getPassenger()->getFullName()
        << "\n"
        << "Flight         : "
        << reservation->getFlight()->getFlightNumber()
        << "\n"
        << "Seat           : "
        << reservation->getSeatNumber()
        << "\n"
        << "Price          : "
        << reservation->getTotalPrice()
        << "\n";

    std::cout
        << "\nAre you sure you want to cancel "
        << "this reservation? (Y/N): ";

    char confirmation;

    std::cin >> confirmation;

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    if (confirmation != 'Y' &&
        confirmation != 'y')
    {
        std::cout
            << "\nCancellation aborted.\n";

        pause();
        return;
    }

    // ========================================================
    // 5. Cancel Reservation
    // ========================================================

    try
    {
        const bool cancelled =
            bookingService.cancelReservation(
                reservationId
            );

        if (!cancelled)
        {
            std::cout
                << "\nFailed to cancel reservation.\n";

            pause();
            return;
        }

        std::cout
            << "\n=====================================\n"
            << " Reservation Cancelled Successfully\n"
            << "=====================================\n\n"
            << "Reservation ID : "
            << reservationId
            << "\n"
            << "Seat           : "
            << reservation->getSeatNumber()
            << "\n"
            << "Status         : Cancelled\n"
            << "\n";

        std::cout
            << "Payment refund processed successfully.\n";
    }
    catch (const std::exception& exception)
    {
        std::cout
            << "\nCancellation failed: "
            << exception.what()
            << "\n";
    }

    pause();
}

void ConsoleUI::airportCheckIn()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "          Airport Check-In\n"
        << "=====================================\n\n";

    const auto& reservations =
        bookingService.getAllReservations();

    if (reservations.empty())
    {
        std::cout
            << "No reservations available.\n";

        pause();
        return;
    }

    std::cout
        << "Existing Reservations:\n\n";

    for (const auto& reservation : reservations)
    {
        if (reservation == nullptr)
            continue;

        std::cout
            << "-------------------------------------\n"
            << "Reservation ID : "
            << reservation->getId()
            << "\n"
            << "Passenger      : "
            << reservation->getPassenger()->getFullName()
            << "\n"
            << "Flight         : "
            << reservation->getFlight()->getFlightNumber()
            << "\n"
            << "Seat           : "
            << reservation->getSeatNumber()
            << "\n"
            << "Status         : ";

        switch (reservation->getStatus())
        {
            case ReservationStatus::Confirmed:
                std::cout << "Confirmed";
                break;

            case ReservationStatus::Waitlisted:
                std::cout << "Waitlisted";
                break;

            case ReservationStatus::Cancelled:
                std::cout << "Cancelled";
                break;
        }

        std::cout << "\n";
    }

    std::cout
        << "-------------------------------------\n\n"
        << "Enter Reservation ID: ";

    int reservationId;

    if (!(std::cin >> reservationId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid reservation ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto reservation =
        bookingService.getReservation(reservationId);

    if (reservation == nullptr)
    {
        std::cout
            << "\nReservation not found.\n";

        pause();
        return;
    }

    std::string checkInDateTime;
    std::string gate;
    std::string boardingTime;

    std::cout
        << "\nCheck-In Date/Time: ";
    std::getline(std::cin, checkInDateTime);

    std::cout
        << "Gate: ";
    std::getline(std::cin, gate);

    std::cout
        << "Boarding Time: ";
    std::getline(std::cin, boardingTime);

    try
    {
        auto checkIn =
            checkInService.airportCheckIn(
                reservation,
                checkInDateTime,
                gate,
                boardingTime
            );

        if (checkIn == nullptr)
        {
            std::cout
                << "\nAirport check-in failed.\n";

            pause();
            return;
        }

        auto boardingPass =
            checkInService.getBoardingPassByReservationId(
                reservationId
            );

        std::cout
            << "\n=====================================\n"
            << "       Check-In Successful\n"
            << "=====================================\n\n";

        std::cout
            << "Check-In ID      : "
            << checkIn->getId()
            << "\n"
            << "Reservation ID   : "
            << checkIn->getReservationId()
            << "\n"
            << "Check-In Type    : Airport\n"
            << "Check-In Date    : "
            << checkIn->getCheckInDateTime()
            << "\n";

        if (boardingPass != nullptr)
        {
            std::cout
                << "\n=====================================\n"
                << "           Boarding Pass\n"
                << "=====================================\n\n"

                << "Boarding Pass ID : "
                << boardingPass->getId()
                << "\n"

                << "Pass Number      : "
                << boardingPass->getPassNumber()
                << "\n"

                << "Passenger        : "
                << boardingPass->getPassengerName()
                << "\n"

                << "Passport         : "
                << boardingPass->getPassportNumber()
                << "\n"

                << "Flight           : "
                << boardingPass->getFlightNumber()
                << "\n"

                << "Seat             : "
                << boardingPass->getSeatNumber()
                << "\n"

                << "Gate             : "
                << boardingPass->getGate()
                << "\n"

                << "Boarding Time    : "
                << boardingPass->getBoardingTime()
                << "\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cout
            << "\nCheck-in failed: "
            << e.what()
            << "\n";
    }

    pause();
}

void ConsoleUI::managePassengers()
{
    while (true)
    {
        clearScreen();

        std::cout
            << "=====================================\n"
            << "        Manage Passengers\n"
            << "=====================================\n\n"
            << "1. List Passengers\n"
            << "2. View Passenger Profile\n"
            << "3. Update Contact Information\n"
            << "4. Set Passenger Preferences\n"
            << "5. View Travel History\n"
            << "6. Back\n\n"
            << "Choose an option: ";

        int choice;

        if (!(std::cin >> choice))
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout
                << "\nInvalid input.\n";

            pause();
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        switch (choice)
        {
            // =====================================
            // 1. List Passengers
            // =====================================
            case 1:
            {
                clearScreen();

                std::cout
                    << "=====================================\n"
                    << "           List Passengers\n"
                    << "=====================================\n\n";

                bool foundPassenger = false;

                for (const auto& user : users)
                {
                    auto passenger =
                        std::dynamic_pointer_cast<Passenger>(user);

                    if (passenger == nullptr)
                    {
                        continue;
                    }

                    foundPassenger = true;

                    std::cout
                        << "-------------------------------------\n"
                        << "Passenger ID  : "
                        << passenger->getId()
                        << "\n"
                        << "Name          : "
                        << passenger->getFullName()
                        << "\n"
                        << "Username      : "
                        << passenger->getUsername()
                        << "\n"
                        << "Email         : "
                        << passenger->getEmail()
                        << "\n"
                        << "Phone         : "
                        << passenger->getPhone()
                        << "\n"
                        << "Passport      : "
                        << passenger->getPassportNumber()
                        << "\n"
                        << "Loyalty Points: "
                        << passenger->getLoyaltyPoints()
                        << "\n";
                }

                if (!foundPassenger)
                {
                    std::cout
                        << "No passengers found.\n";
                }
                else
                {
                    std::cout
                        << "-------------------------------------\n";
                }

                pause();
                break;
            }

            // =====================================
            // 2. View Passenger Profile
            // =====================================
            case 2:
            {
                clearScreen();

                std::cout
                    << "=====================================\n"
                    << "       View Passenger Profile\n"
                    << "=====================================\n\n";

                std::cout
                    << "Available Passengers:\n\n";

                bool foundPassenger = false;

                for (const auto& user : users)
                {
                    auto passenger =
                        std::dynamic_pointer_cast<Passenger>(user);

                    if (passenger == nullptr)
                    {
                        continue;
                    }

                    foundPassenger = true;

                    std::cout
                        << "ID: "
                        << passenger->getId()
                        << " | Name: "
                        << passenger->getFullName()
                        << "\n";
                }

                if (!foundPassenger)
                {
                    std::cout
                        << "\nNo passengers found.\n";

                    pause();
                    break;
                }

                int passengerId;

                std::cout
                    << "\nEnter Passenger ID: ";

                if (!(std::cin >> passengerId))
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout
                        << "\nInvalid passenger ID.\n";

                    pause();
                    break;
                }

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );

                std::shared_ptr<Passenger> selectedPassenger;

                for (const auto& user : users)
                {
                    auto passenger =
                        std::dynamic_pointer_cast<Passenger>(user);

                    if (passenger != nullptr &&
                        passenger->getId() == passengerId)
                    {
                        selectedPassenger = passenger;
                        break;
                    }
                }

                if (selectedPassenger == nullptr)
                {
                    std::cout
                        << "\nPassenger not found.\n";

                    pause();
                    break;
                }

                passengerService.displayProfile(
                    selectedPassenger
                );

                pause();
                break;
            }

            // =====================================
            // 3. Update Contact Information
            // =====================================
            case 3:
            {
                clearScreen();

                std::cout
                    << "=====================================\n"
                    << "    Update Contact Information\n"
                    << "=====================================\n\n";

                int passengerId;

                std::cout
                    << "Enter Passenger ID: ";

                if (!(std::cin >> passengerId))
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout
                        << "\nInvalid passenger ID.\n";

                    pause();
                    break;
                }

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );

                std::shared_ptr<Passenger> selectedPassenger;

                for (const auto& user : users)
                {
                    auto passenger =
                        std::dynamic_pointer_cast<Passenger>(user);

                    if (passenger != nullptr &&
                        passenger->getId() == passengerId)
                    {
                        selectedPassenger = passenger;
                        break;
                    }
                }

                if (selectedPassenger == nullptr)
                {
                    std::cout
                        << "\nPassenger not found.\n";

                    pause();
                    break;
                }

                std::string email;
                std::string phone;

                std::cout
                    << "Current Email: "
                    << selectedPassenger->getEmail()
                    << "\n";

                std::cout
                    << "New Email: ";

                std::getline(std::cin, email);

                std::cout
                    << "Current Phone: "
                    << selectedPassenger->getPhone()
                    << "\n";

                std::cout
                    << "New Phone: ";

                std::getline(std::cin, phone);

                try
                {
                    passengerService.updateContactInformation(
                        selectedPassenger,
                        email,
                        phone
                    );

                    std::cout
                        << "\nContact information updated successfully.\n";
                }
                catch (const std::exception& exception)
                {
                    std::cout
                        << "\nUpdate failed: "
                        << exception.what()
                        << "\n";
                }

                pause();
                break;
            }

            // =====================================
            // 4. Set Passenger Preferences
            // =====================================
            case 4:
            {
                clearScreen();

                std::cout
                    << "=====================================\n"
                    << "       Passenger Preferences\n"
                    << "=====================================\n\n";

                int passengerId;

                std::cout
                    << "Enter Passenger ID: ";

                if (!(std::cin >> passengerId))
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout
                        << "\nInvalid passenger ID.\n";

                    pause();
                    break;
                }

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );

                std::shared_ptr<Passenger> selectedPassenger;

                for (const auto& user : users)
                {
                    auto passenger =
                        std::dynamic_pointer_cast<Passenger>(user);

                    if (passenger != nullptr &&
                        passenger->getId() == passengerId)
                    {
                        selectedPassenger = passenger;
                        break;
                    }
                }

                if (selectedPassenger == nullptr)
                {
                    std::cout
                        << "\nPassenger not found.\n";

                    pause();
                    break;
                }

                std::string preferredSeat;
                std::string mealPreference;

                std::cout
                    << "Current Preferred Seat: ";

                if (selectedPassenger->getPreferredSeat().empty())
                {
                    std::cout << "Not set\n";
                }
                else
                {
                    std::cout
                        << selectedPassenger->getPreferredSeat()
                        << "\n";
                }

                std::cout
                    << "New Preferred Seat: ";

                std::getline(
                    std::cin,
                    preferredSeat
                );

                std::cout
                    << "Current Meal Preference: ";

                if (selectedPassenger->getMealPreference().empty())
                {
                    std::cout << "Not set\n";
                }
                else
                {
                    std::cout
                        << selectedPassenger->getMealPreference()
                        << "\n";
                }

                std::cout
                    << "New Meal Preference: ";

                std::getline(
                    std::cin,
                    mealPreference
                );

                try
                {
                    passengerService.setPreferences(
                        selectedPassenger,
                        preferredSeat,
                        mealPreference
                    );

                    std::cout
                        << "\nPassenger preferences updated successfully.\n";
                }
                catch (const std::exception& exception)
                {
                    std::cout
                        << "\nUpdate failed: "
                        << exception.what()
                        << "\n";
                }

                pause();
                break;
            }

            // =====================================
            // 5. View Travel History
            // =====================================
            case 5:
            {
                clearScreen();

                std::cout
                    << "=====================================\n"
                    << "         Travel History\n"
                    << "=====================================\n\n";

                int passengerId;

                std::cout
                    << "Enter Passenger ID: ";

                if (!(std::cin >> passengerId))
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    std::cout
                        << "\nInvalid passenger ID.\n";

                    pause();
                    break;
                }

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );

                std::shared_ptr<Passenger> selectedPassenger;

                for (const auto& user : users)
                {
                    auto passenger =
                        std::dynamic_pointer_cast<Passenger>(user);

                    if (passenger != nullptr &&
                        passenger->getId() == passengerId)
                    {
                        selectedPassenger = passenger;
                        break;
                    }
                }

                if (selectedPassenger == nullptr)
                {
                    std::cout
                        << "\nPassenger not found.\n";

                    pause();
                    break;
                }

                try
                {
                    const auto& history =
                        passengerService.getTravelHistory(
                            selectedPassenger
                        );

                    if (history.empty())
                    {
                        std::cout
                            << "\nNo travel history found.\n";
                    }
                    else
                    {
                        std::cout
                            << "\nPassenger: "
                            << selectedPassenger->getFullName()
                            << "\n\n";

                        for (const auto& flight : history)
                        {
                            if (flight == nullptr)
                            {
                                continue;
                            }

                            std::cout
                                << "-------------------------------------\n"
                                << "Flight Number : "
                                << flight->getFlightNumber()
                                << "\n"
                                << "Origin        : "
                                << flight->getOrigin()
                                << "\n"
                                << "Destination   : "
                                << flight->getDestination()
                                << "\n"
                                << "Departure     : "
                                << flight->getDepartureTime()
                                << "\n"
                                << "Arrival       : "
                                << flight->getArrivalTime()
                                << "\n"
                                << "-------------------------------------\n";
                        }
                    }
                }
                catch (const std::exception& exception)
                {
                    std::cout
                        << "\nFailed to retrieve travel history: "
                        << exception.what()
                        << "\n";
                }

                pause();
                break;
            }

            // =====================================
            // 6. Back
            // =====================================
            case 6:
                return;

            default:
                std::cout
                    << "\nInvalid option.\n";

                pause();
                break;
        }
    }
}

void ConsoleUI::processPayment()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "          Process Payment\n"
        << "=====================================\n\n";

    const auto& reservations =
        bookingService.getAllReservations();

    if (reservations.empty())
    {
        std::cout
            << "No reservations available.\n";

        pause();
        return;
    }

    // ========================================================
    // 1. Display Reservations
    // ========================================================

    std::cout
        << "Existing Reservations:\n\n";

    for (const auto& reservation : reservations)
    {
        if (reservation == nullptr)
        {
            continue;
        }

        auto passenger =
            reservation->getPassenger();

        auto flight =
            reservation->getFlight();

        std::cout
            << "-------------------------------------\n"
            << "Reservation ID : "
            << reservation->getId()
            << "\n"
            << "Passenger      : "
            << (passenger != nullptr
                    ? passenger->getFullName()
                    : "Unknown")
            << "\n"
            << "Flight         : "
            << (flight != nullptr
                    ? flight->getFlightNumber()
                    : "Unknown")
            << "\n"
            << "Seat           : "
            << reservation->getSeatNumber()
            << "\n"
            << "Amount         : "
            << reservation->getTotalPrice()
            << "\n"
            << "-------------------------------------\n";
    }

    // ========================================================
    // 2. Select Reservation
    // ========================================================

    std::cout
        << "\nEnter Reservation ID: ";

    int reservationId;

    if (!(std::cin >> reservationId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid reservation ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto reservation =
        bookingService.getReservation(
            reservationId
        );

    if (reservation == nullptr)
    {
        std::cout
            << "\nReservation not found.\n";

        pause();
        return;
    }

    // ========================================================
    // 3. Get Payment
    // ========================================================

    auto payment =
        bookingService.getPaymentByReservationId(
            reservationId
        );

            if (payment == nullptr)
    {
        std::cout
            << "\nNo payment found for this reservation.\n";

        pause();
        return;
    }

    // ========================================================
    // 4. Display Payment Information
    // ========================================================

    std::cout
        << "\n=====================================\n"
        << "          Payment Details\n"
        << "=====================================\n"
        << "Payment ID     : "
        << payment->getId()
        << "\n"
        << "Reservation ID : "
        << payment->getReservationId()
        << "\n"
        << "Amount         : "
        << payment->getAmount()
        << "\n"
        << "Transaction Date: "
        << payment->getTransactionDate()
        << "\n"
        << "Status         : ";

    switch (payment->getStatus())
    {
        case PaymentStatus::Pending:
            std::cout << "Pending";
            break;

        case PaymentStatus::Completed:
            std::cout << "Completed";
            break;

        case PaymentStatus::Failed:
            std::cout << "Failed";
            break;

        case PaymentStatus::Refunded:
            std::cout << "Refunded";
            break;
    }

    std::cout
        << "\n";

    // ========================================================
    // 5. Payment Status
    // ========================================================

    if (payment->getStatus() ==
        PaymentStatus::Completed)
    {
        std::cout
            << "\nPayment has already been completed.\n";
    }
    else if (
        payment->getStatus() ==
        PaymentStatus::Refunded)
    {
        std::cout
            << "\nPayment has already been refunded.\n";
    }
    else if (
        payment->getStatus() ==
        PaymentStatus::Pending)
    {
        std::cout
            << "\nPayment is currently pending.\n";
    }
    else
    {
        std::cout
            << "\nPayment failed.\n";
    }

    pause();
}

// ============================================================
// CHANGE FLIGHT STATUS
// ============================================================

void ConsoleUI::changeFlightStatus()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << "         Change Flight Status\n"
        << "=====================================\n\n";

    if (flights.empty())
    {
        std::cout
            << "No flights found.\n";

        pause();
        return;
    }

    std::string flightNumber;

    std::cout
        << "Flight Number: ";

    std::getline(
        std::cin,
        flightNumber
    );

    std::shared_ptr<Flight> flight;

    for (const auto& flightItem : flights)
    {
        if (flightItem &&
            flightItem->getFlightNumber()
                == flightNumber)
        {
            flight = flightItem;
            break;
        }
    }

    if (!flight)
    {
        std::cout
            << "\nFlight not found.\n";

        pause();
        return;
    }

    clearScreen();

    std::cout
        << "=====================================\n"
        << "         Change Flight Status\n"
        << "=====================================\n\n"
        << "Flight: "
        << flight->getFlightNumber()
        << "\n"
        << "Current Status: "
        << flightStatusToString(
            flight->getStatus()
        )
        << "\n\n"
        << "1. Start Boarding\n"
        << "2. Delay Flight\n"
        << "3. Cancel Flight\n"
        << "4. Depart Flight\n"
        << "5. Complete Flight\n"
        << "6. Back\n\n"
        << "Choose an option: ";

    int choice;

    if (!(std::cin >> choice))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid input.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    if (choice == 6)
    {
        return;
    }

    try
    {
        FlightOperationsService operationsService;

        switch (choice)
        {
            case 1:
                operationsService.startBoarding(
                    flight
                );
                break;

            case 2:
                operationsService.delayFlight(
                    flight
                );
                break;

            case 3:
                operationsService.cancelFlight(
                    flight
                );
                break;

            case 4:
                operationsService.departFlight(
                    flight
                );
                break;

            case 5:
                operationsService.completeFlight(
                    flight
                );
                break;

            default:
                std::cout
                    << "\nInvalid option.\n";

                pause();
                return;
        }

        FlightRepository flightRepository;

        flightRepository.save(flights);

        std::cout
            << "\nFlight status updated successfully.\n"
            << "New Status: "
            << flightStatusToString(
                flight->getStatus()
            )
            << "\n";
    }
    catch (const std::exception& e)
    {
        std::cout
            << "\nFailed to change flight status.\n"
            << e.what()
            << "\n";
    }

    pause();
}

// ============================================================
// FLIGHT STATUS TO STRING
// ============================================================

std::string ConsoleUI::flightStatusToString(
    FlightStatus status
) const
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

    return "Unknown";
}

// ============================================================
// AIRCRAFT STATUS TO STRING
// ============================================================

std::string aircraftStatusToString(
    AircraftStatus status
)
{
    switch (status)
    {
        case AircraftStatus::Available:
            return "Available";

        case AircraftStatus::InFlight:
            return "In Flight";

        case AircraftStatus::Maintenance:
            return "Maintenance";

        case AircraftStatus::Unavailable:
            return "Unavailable";
    }

    return "Unknown";
}

// ============================================================
// AIRCRAFT - LIST
// ============================================================

void ConsoleUI::listAircraft() const
{
    clearScreen();

    std::cout
        << "========================================\n"
        << "              ALL AIRCRAFT\n"
        << "========================================\n\n";

    if (aircraft.empty())
    {
        std::cout
            << "No aircraft found.\n";

        pause();
        return;
    }

    for (const auto& currentAircraft : aircraft)
    {
        if (!currentAircraft)
        {
            continue;
        }

        std::cout
            << "----------------------------------------\n"
            << "ID                    : "
            << currentAircraft->getId()
            << "\n"
            << "Registration Number   : "
            << currentAircraft->getRegistrationNumber()
            << "\n"
            << "Manufacturer          : "
            << currentAircraft->getManufacturer()
            << "\n"
            << "Model                 : "
            << currentAircraft->getModel()
            << "\n"
            << "Capacity              : "
            << currentAircraft->getCapacity()
            << " seats\n"
            << "Status                : "
            << aircraftStatusToString(
                   currentAircraft->getStatus()
               )
            << "\n"
            << "Next Maintenance Date : "
            << (
                   currentAircraft->getNextMaintenanceDate().empty()
                       ? "Not Scheduled"
                       : currentAircraft->getNextMaintenanceDate()
               )
            << "\n";
    }

    std::cout
        << "----------------------------------------\n";

    pause();
}

// ============================================================
// AIRCRAFT - CREATE
// ============================================================

void ConsoleUI::createAircraft()
{
    clearScreen();

    std::cout
        << "========================================\n"
        << "             ADD AIRCRAFT\n"
        << "========================================\n\n";

    try
    {
        std::string registrationNumber;
        std::string manufacturer;
        std::string model;
        std::string nextMaintenanceDate;
        int capacity;

        std::cout
            << "Registration Number: ";

        std::getline(
            std::cin,
            registrationNumber
        );

        if (registrationNumber.empty())
        {
            throw std::invalid_argument(
                "Registration number cannot be empty."
            );
        }

        auto duplicateIt =
            std::find_if(
                aircraft.begin(),
                aircraft.end(),
                [&registrationNumber](
                    const std::shared_ptr<Aircraft>& currentAircraft
                )
                {
                    return currentAircraft &&
                           currentAircraft->getRegistrationNumber()
                               == registrationNumber;
                }
            );

        if (duplicateIt != aircraft.end())
        {
            throw std::invalid_argument(
                "An aircraft with this registration number already exists."
            );
        }

        std::cout
            << "Manufacturer: ";

        std::getline(
            std::cin,
            manufacturer
        );

        std::cout
            << "Model: ";

        std::getline(
            std::cin,
            model
        );

        std::cout
            << "Capacity: ";

        std::cin >> capacity;

        if (std::cin.fail())
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            throw std::invalid_argument(
                "Capacity must be a valid number."
            );
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "Next Maintenance Date "
            << "(leave empty if not scheduled): ";

        std::getline(
            std::cin,
            nextMaintenanceDate
        );

        int nextId = 1;

        for (const auto& currentAircraft : aircraft)
        {
            if (currentAircraft)
            {
                nextId =
                    std::max(
                        nextId,
                        currentAircraft->getId() + 1
                    );
            }
        }

        auto newAircraft =
            std::make_shared<Aircraft>(
                nextId,
                registrationNumber,
                manufacturer,
                model,
                capacity,
                AircraftStatus::Available,
                nextMaintenanceDate
            );

        aircraft.push_back(
            newAircraft
        );

        AircraftRepository repository;

        repository.save(
            aircraft
        );

        std::cout
            << "\nAircraft created successfully.\n"
            << "Assigned ID: "
            << newAircraft->getId()
            << "\n";

        pause();
    }
    catch (const std::exception& ex)
    {
        std::cout
            << "\nFailed to create aircraft: "
            << ex.what()
            << "\n";

        pause();
    }
}

// ============================================================
// AIRCRAFT - UPDATE
// ============================================================

void ConsoleUI::updateAircraft()
{
    clearScreen();

    std::cout
        << "========================================\n"
        << "            UPDATE AIRCRAFT\n"
        << "========================================\n\n";

    if (aircraft.empty())
    {
        std::cout
            << "No aircraft found.\n";

        pause();
        return;
    }

    int aircraftId;

    std::cout
        << "Enter Aircraft ID: ";

    std::cin >> aircraftId;

    if (std::cin.fail())
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid Aircraft ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto aircraftIt =
        std::find_if(
            aircraft.begin(),
            aircraft.end(),
            [aircraftId](
                const std::shared_ptr<Aircraft>& currentAircraft
            )
            {
                return currentAircraft &&
                       currentAircraft->getId()
                           == aircraftId;
            }
        );

    if (aircraftIt == aircraft.end())
    {
        std::cout
            << "\nAircraft not found.\n";

        pause();
        return;
    }

    auto currentAircraft = *aircraftIt;

    while (true)
    {
        clearScreen();

        std::cout
            << "========================================\n"
            << "            UPDATE AIRCRAFT\n"
            << "========================================\n\n"
            << "Aircraft ID          : "
            << currentAircraft->getId()
            << "\n"
            << "Registration Number  : "
            << currentAircraft->getRegistrationNumber()
            << "\n"
            << "Manufacturer         : "
            << currentAircraft->getManufacturer()
            << "\n"
            << "Model                : "
            << currentAircraft->getModel()
            << "\n"
            << "Capacity             : "
            << currentAircraft->getCapacity()
            << "\n"
            << "Current Status       : "
            << aircraftStatusToString(
                   currentAircraft->getStatus()
               )
            << "\n"
            << "Maintenance Date     : "
            << (
                   currentAircraft->getNextMaintenanceDate().empty()
                       ? "Not Scheduled"
                       : currentAircraft->getNextMaintenanceDate()
               )
            << "\n\n"
            << "1. Update Status\n"
            << "2. Update Maintenance Date\n"
            << "3. Back\n"
            << "========================================\n";

        int choice;

        std::cout
            << "Enter your choice: ";

        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout
                << "\nInvalid choice.\n";

            pause();
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        if (choice == 1)
        {
            std::cout
                << "\nSelect New Status:\n"
                << "1. Available\n"
                << "2. In Flight\n"
                << "3. Maintenance\n"
                << "4. Unavailable\n";

            int statusChoice;

            std::cout
                << "Enter choice: ";

            std::cin >> statusChoice;

            if (std::cin.fail())
            {
                std::cin.clear();

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );

                std::cout
                    << "\nInvalid status.\n";

                pause();
                continue;
            }

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            AircraftStatus newStatus;

            switch (statusChoice)
            {
                case 1:
                    newStatus = AircraftStatus::Available;
                    break;

                case 2:
                    newStatus = AircraftStatus::InFlight;
                    break;

                case 3:
                    newStatus = AircraftStatus::Maintenance;
                    break;

                case 4:
                    newStatus = AircraftStatus::Unavailable;
                    break;

                default:
                    std::cout
                        << "\nInvalid status choice.\n";

                    pause();
                    continue;
            }

            currentAircraft->setStatus(
                newStatus
            );

            AircraftRepository repository;

            repository.save(
                aircraft
            );

            std::cout
                << "\nAircraft status updated successfully.\n";

            pause();
        }
        else if (choice == 2)
        {
            std::string newDate;

            std::cout
                << "\nEnter New Maintenance Date "
                << "(leave empty to clear): ";

            std::getline(
                std::cin,
                newDate
            );

            currentAircraft->setNextMaintenanceDate(
                newDate
            );

            AircraftRepository repository;

            repository.save(
                aircraft
            );

            std::cout
                << "\nMaintenance date updated successfully.\n";

            pause();
        }
        else if (choice == 3)
        {
            return;
        }
        else
        {
            std::cout
                << "\nInvalid choice.\n";

            pause();
        }
    }
}

// ============================================================
// AIRCRAFT - DELETE
// ============================================================

void ConsoleUI::deleteAircraft()
{
    clearScreen();

    std::cout
        << "========================================\n"
        << "            DELETE AIRCRAFT\n"
        << "========================================\n\n";

    if (aircraft.empty())
    {
        std::cout
            << "No aircraft found.\n";

        pause();
        return;
    }

    int aircraftId;

    std::cout
        << "Enter Aircraft ID: ";

    std::cin >> aircraftId;

    if (std::cin.fail())
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid Aircraft ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto aircraftIt =
        std::find_if(
            aircraft.begin(),
            aircraft.end(),
            [aircraftId](
                const std::shared_ptr<Aircraft>& currentAircraft
            )
            {
                return currentAircraft &&
                       currentAircraft->getId()
                           == aircraftId;
            }
        );

    if (aircraftIt == aircraft.end())
    {
        std::cout
            << "\nAircraft not found.\n";

        pause();
        return;
    }

    auto currentAircraft = *aircraftIt;

    auto flightIt =
        std::find_if(
            flights.begin(),
            flights.end(),
            [aircraftId](
                const std::shared_ptr<Flight>& currentFlight
            )
            {
                if (!currentFlight)
                {
                    return false;
                }

                auto assignedAircraft =
                    currentFlight->getAircraft();

                return assignedAircraft &&
                       assignedAircraft->getId()
                           == aircraftId;
            }
        );

    if (flightIt != flights.end())
    {
        std::cout
            << "\nCannot delete this aircraft.\n"
            << "It is currently assigned to a flight.\n"
            << "Flight Number: "
            << (*flightIt)->getFlightNumber()
            << "\n";

        pause();
        return;
    }

    std::cout
        << "\nAircraft:\n"
        << "ID                   : "
        << currentAircraft->getId()
        << "\n"
        << "Registration Number  : "
        << currentAircraft->getRegistrationNumber()
        << "\n"
        << "Manufacturer         : "
        << currentAircraft->getManufacturer()
        << "\n"
        << "Model                : "
        << currentAircraft->getModel()
        << "\n\n";

    std::cout
        << "Are you sure you want to delete this aircraft?\n"
        << "Enter Y to confirm or N to cancel: ";

    char confirmation;

    std::cin >> confirmation;

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    if (confirmation != 'Y' &&
        confirmation != 'y')
    {
        std::cout
            << "\nDeletion cancelled.\n";

        pause();
        return;
    }

    aircraft.erase(
        aircraftIt
    );

    AircraftRepository repository;

    repository.save(
        aircraft
    );

    std::cout
        << "\nAircraft deleted successfully.\n";

    pause();
}

// ============================================================
// AIRCRAFT - CHANGE STATUS
// ============================================================

void ConsoleUI::changeAircraftStatus()
{
    clearScreen();

    std::cout
        << "========================================\n"
        << "         CHANGE AIRCRAFT STATUS\n"
        << "========================================\n\n";

    if (aircraft.empty())
    {
        std::cout
            << "No aircraft found.\n";

        pause();
        return;
    }

    int aircraftId;

    std::cout
        << "Enter Aircraft ID: ";

    std::cin >> aircraftId;

    if (std::cin.fail())
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid Aircraft ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto aircraftIt =
        std::find_if(
            aircraft.begin(),
            aircraft.end(),
            [aircraftId](
                const std::shared_ptr<Aircraft>& currentAircraft
            )
            {
                return currentAircraft &&
                       currentAircraft->getId()
                           == aircraftId;
            }
        );

    if (aircraftIt == aircraft.end())
    {
        std::cout
            << "\nAircraft not found.\n";

        pause();
        return;
    }

    auto currentAircraft = *aircraftIt;

    std::cout
        << "\nAircraft: "
        << currentAircraft->getRegistrationNumber()
        << "\n"
        << "Current Status: "
        << aircraftStatusToString(
               currentAircraft->getStatus()
           )
        << "\n\n"
        << "1. Available\n"
        << "2. In Flight\n"
        << "3. Maintenance\n"
        << "4. Unavailable\n"
        << "5. Cancel\n"
        << "========================================\n";

    int statusChoice;

    std::cout
        << "Enter your choice: ";

    std::cin >> statusChoice;

    if (std::cin.fail())
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid choice.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    AircraftStatus newStatus;

    switch (statusChoice)
    {
        case 1:
            newStatus = AircraftStatus::Available;
            break;

        case 2:
            newStatus = AircraftStatus::InFlight;
            break;

        case 3:
            newStatus = AircraftStatus::Maintenance;
            break;

        case 4:
            newStatus = AircraftStatus::Unavailable;
            break;

        case 5:
            return;

        default:
            std::cout
                << "\nInvalid status choice.\n";

            pause();
            return;
    }

    currentAircraft->setStatus(
        newStatus
    );

    AircraftRepository repository;

    repository.save(
        aircraft
    );

    std::cout
        << "\nAircraft status changed successfully.\n"
        << "New Status: "
        << aircraftStatusToString(
               currentAircraft->getStatus()
           )
        << "\n";

    pause();
}

// ============================================================
// AIRCRAFT MANAGEMENT
// ============================================================

void ConsoleUI::manageAircraft()
{
    while (true)
    {
        clearScreen();

        std::cout
            << "========================================\n"
            << "          AIRCRAFT MANAGEMENT\n"
            << "========================================\n"
            << "1. List All Aircraft\n"
            << "2. Add Aircraft\n"
            << "3. Update Aircraft\n"
            << "4. Delete Aircraft\n"
            << "5. Change Aircraft Status\n"
            << "6. Back\n"
            << "========================================\n";

        int choice;

        std::cout
            << "Enter your choice: ";

        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout
                << "\nInvalid choice.\n";

            pause();
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        switch (choice)
        {
            case 1:
                listAircraft();
                break;

            case 2:
                createAircraft();
                break;

            case 3:
                updateAircraft();
                break;

            case 4:
                deleteAircraft();
                break;

            case 5:
                changeAircraftStatus();
                break;

            case 6:
                return;

            default:
                std::cout
                    << "\nInvalid choice.\n";

                pause();
                break;
        }
    }
}

// ============================================================
// CREW MANAGEMENT
// ============================================================

void ConsoleUI::manageCrew()
{
    while (true)
    {
        clearScreen();

        std::cout
            << "========================================\n"
            << "             CREW MANAGEMENT\n"
            << "========================================\n"
            << "1. List All Crew Members\n"
            << "2. Add Crew Member\n"
            << "3. Update Crew Member\n"
            << "4. Delete Crew Member\n"
            << "5. Change Crew Status\n"
            << "6. Assign Crew to Flight\n"
            << "7. Remove Crew from Flight\n"
            << "8. Back\n"
            << "========================================\n";

        int choice;

        std::cout
            << "Enter your choice: ";

        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout
                << "\nInvalid choice.\n";

            pause();
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        switch (choice)
        {
            case 1:
                listCrewMembers();
                break;

            case 2:
                createCrewMember();
                break;

            case 3:
                updateCrewMember();
                break;

            case 4:
                deleteCrewMember();
                break;

            case 5:
                changeCrewStatus();
                break;

            case 6:
                assignCrewToFlight();
                break;

            case 7:
                removeCrewFromFlight();
                break;

            case 8:
                return;

            default:
                std::cout
                    << "\nInvalid choice.\n";

                pause();
                break;
        }
    }
}

// ============================================================
// CREW - LIST
// ============================================================

void ConsoleUI::listCrewMembers() const
{
    clearScreen();

    std::cout
        << "========================================\n"
        << "          ALL CREW MEMBERS\n"
        << "========================================\n\n";

    if (crewMembers.empty())
    {
        std::cout
            << "No crew members found.\n";

        pause();
        return;
    }

    for (const auto& crewMember : crewMembers)
    {
        if (!crewMember)
        {
            continue;
        }

        std::cout
            << "----------------------------------------\n"
            << "ID                   : "
            << crewMember->getId()
            << "\n"
            << "Employee ID          : "
            << crewMember->getEmployeeId()
            << "\n"
            << "Full Name            : "
            << crewMember->getFullName()
            << "\n"
            << "Role                 : "
            << crewMember->getCrewRole()
            << "\n"
            << "Total Flight Hours   : "
            << crewMember->getTotalFlightHours()
            << "\n"
            << "Maximum Flight Hours : "
            << crewMember->getMaximumFlightHours()
            << "\n"
            << "Status               : "
            << (
                crewMember->getIsActive()
                    ? "Active"
                    : "Inactive"
            )
            << "\n";

        auto pilot =
            std::dynamic_pointer_cast<Pilot>(
                crewMember
            );

        if (pilot)
        {
            std::cout
                << "License Number       : "
                << pilot->getLicenseNumber()
                << "\n";
        }

        std::cout
            << "Assigned Flights     : ";

        bool hasAssignedFlight = false;

        for (const auto& flight : flights)
        {
            if (!flight)
            {
                continue;
            }

            const auto& assignedCrew =
                flight->getCrewMembers();

            auto crewIt =
                std::find_if(
                    assignedCrew.begin(),
                    assignedCrew.end(),
                    [&crewMember](
                        const std::shared_ptr<CrewMember>& member
                    )
                    {
                        return member &&
                               member->getId()
                                   == crewMember->getId();
                    }
                );

            if (crewIt != assignedCrew.end())
            {
                if (hasAssignedFlight)
                {
                    std::cout
                        << ", ";
                }

                std::cout
                    << flight->getFlightNumber();

                hasAssignedFlight = true;
            }
        }

        if (!hasAssignedFlight)
        {
            std::cout
                << "None";
        }

        std::cout
            << "\n";
    }

    std::cout
        << "----------------------------------------\n";

    pause();
}

// ============================================================
// CREW - CREATE
// ============================================================

void ConsoleUI::createCrewMember()
{
    clearScreen();

    std::cout
        << "========================================\n"
        << "           ADD CREW MEMBER\n"
        << "========================================\n\n";

    try
    {
        std::cout
            << "Select Crew Role:\n"
            << "1. Pilot\n"
            << "2. Flight Attendant\n"
            << "3. Cancel\n\n"
            << "Choose: ";

        int roleChoice;

        if (!(std::cin >> roleChoice))
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            throw std::invalid_argument(
                "Invalid role choice."
            );
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        if (roleChoice == 3)
        {
            return;
        }

        if (roleChoice != 1 &&
            roleChoice != 2)
        {
            throw std::invalid_argument(
                "Invalid role choice."
            );
        }

        std::string employeeId;
        std::string fullName;
        std::string licenseNumber;
        int maximumFlightHours;

        std::cout
            << "\nEmployee ID: ";

        std::getline(
            std::cin,
            employeeId
        );

        if (employeeId.empty())
        {
            throw std::invalid_argument(
                "Employee ID cannot be empty."
            );
        }

        auto duplicateEmployee =
            std::find_if(
                crewMembers.begin(),
                crewMembers.end(),
                [&employeeId](
                    const std::shared_ptr<CrewMember>& member
                )
                {
                    return member &&
                           member->getEmployeeId()
                               == employeeId;
                }
            );

        if (duplicateEmployee != crewMembers.end())
        {
            throw std::invalid_argument(
                "A crew member with this Employee ID already exists."
            );
        }

        std::cout
            << "Full Name: ";

        std::getline(
            std::cin,
            fullName
        );

        if (fullName.empty())
        {
            throw std::invalid_argument(
                "Full name cannot be empty."
            );
        }

        std::cout
            << "Maximum Flight Hours: ";

        if (!(std::cin >> maximumFlightHours))
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            throw std::invalid_argument(
                "Maximum flight hours must be a valid number."
            );
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        if (maximumFlightHours <= 0)
        {
            throw std::invalid_argument(
                "Maximum flight hours must be positive."
            );
        }

        if (roleChoice == 1)
        {
            std::cout
                << "License Number: ";

            std::getline(
                std::cin,
                licenseNumber
            );

            if (licenseNumber.empty())
            {
                throw std::invalid_argument(
                    "Pilot license number cannot be empty."
                );
            }

            int nextId = 1;

            for (const auto& member : crewMembers)
            {
                if (member)
                {
                    nextId =
                        std::max(
                            nextId,
                            member->getId() + 1
                        );
                }
            }

            auto pilot =
                std::make_shared<Pilot>(
                    nextId,
                    employeeId,
                    fullName,
                    maximumFlightHours,
                    licenseNumber
                );

            crewMembers.push_back(
                pilot
            );

            CrewRepository repository;

            repository.save(
                crewMembers
            );

            std::cout
                << "\nPilot created successfully.\n"
                << "Assigned ID: "
                << pilot->getId()
                << "\n";
        }
        else
        {
            int nextId = 1;

            for (const auto& member : crewMembers)
            {
                if (member)
                {
                    nextId =
                        std::max(
                            nextId,
                            member->getId() + 1
                        );
                }
            }

            auto flightAttendant =
                std::make_shared<FlightAttendant>(
                    nextId,
                    employeeId,
                    fullName,
                    maximumFlightHours
                );

            crewMembers.push_back(
                flightAttendant
            );

            CrewRepository repository;

            repository.save(
                crewMembers
            );

            std::cout
                << "\nFlight Attendant created successfully.\n"
                << "Assigned ID: "
                << flightAttendant->getId()
                << "\n";
        }

        pause();
    }
    catch (const std::exception& e)
    {
        std::cout
            << "\nFailed to create crew member.\n"
            << e.what()
            << "\n";

        pause();
    }
}

// ============================================================
// CREW - UPDATE
// ============================================================

void ConsoleUI::updateCrewMember()
{
    clearScreen();

    std::cout
        << "========================================\n"
        << "           UPDATE CREW MEMBER\n"
        << "========================================\n\n";

    if (crewMembers.empty())
    {
        std::cout
            << "No crew members found.\n";

        pause();
        return;
    }

    int crewId;

    std::cout
        << "Enter Crew Member ID: ";

    if (!(std::cin >> crewId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid Crew Member ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto crewIt =
        std::find_if(
            crewMembers.begin(),
            crewMembers.end(),
            [crewId](
                const std::shared_ptr<CrewMember>& member
            )
            {
                return member &&
                       member->getId() == crewId;
            }
        );

    if (crewIt == crewMembers.end())
    {
        std::cout
            << "\nCrew member not found.\n";

        pause();
        return;
    }

    auto crewMember = *crewIt;

    while (true)
    {
        clearScreen();

        std::cout
            << "========================================\n"
            << "           UPDATE CREW MEMBER\n"
            << "========================================\n\n"
            << "ID                   : "
            << crewMember->getId()
            << "\n"
            << "Employee ID          : "
            << crewMember->getEmployeeId()
            << "\n"
            << "Full Name            : "
            << crewMember->getFullName()
            << "\n"
            << "Role                 : "
            << crewMember->getCrewRole()
            << "\n"
            << "Total Flight Hours   : "
            << crewMember->getTotalFlightHours()
            << "\n"
            << "Maximum Flight Hours : "
            << crewMember->getMaximumFlightHours()
            << "\n"
            << "Status               : "
            << (
                crewMember->getIsActive()
                    ? "Active"
                    : "Inactive"
            )
            << "\n\n"
            << "1. Update Full Name\n"
            << "2. Update Maximum Flight Hours\n"
            << "3. Back\n\n"
            << "Choose an option: ";

        int choice;

        if (!(std::cin >> choice))
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout
                << "\nInvalid input.\n";

            pause();
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        try
        {
            if (choice == 1)
            {
                std::string newName;

                std::cout
                    << "\nNew Full Name: ";

                std::getline(
                    std::cin,
                    newName
                );

                crewMember->setFullName(
                    newName
                );

                CrewRepository repository;

                repository.save(
                    crewMembers
                );

                std::cout
                    << "\nFull name updated successfully.\n";

                pause();
            }
            else if (choice == 2)
            {
                int newMaximumHours;

                std::cout
                    << "\nCurrent Maximum Hours: "
                    << crewMember->getMaximumFlightHours()
                    << "\n"
                    << "Current Total Hours: "
                    << crewMember->getTotalFlightHours()
                    << "\n"
                    << "New Maximum Flight Hours: ";

                if (!(std::cin >> newMaximumHours))
                {
                    std::cin.clear();

                    std::cin.ignore(
                        std::numeric_limits<std::streamsize>::max(),
                        '\n'
                    );

                    throw std::invalid_argument(
                        "Maximum flight hours must be a valid number."
                    );
                }

                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(),
                    '\n'
                );

                crewMember->setMaximumFlightHours(
                    newMaximumHours
                );

                CrewRepository repository;

                repository.save(
                    crewMembers
                );

                std::cout
                    << "\nMaximum flight hours updated successfully.\n";

                pause();
            }
            else if (choice == 3)
            {
                return;
            }
            else
            {
                std::cout
                    << "\nInvalid option.\n";

                pause();
            }
        }
        catch (const std::exception& e)
        {
            std::cout
                << "\nFailed to update crew member.\n"
                << e.what()
                << "\n";

            pause();
        }
    }
}

// ============================================================
// CREW - DELETE
// ============================================================

void ConsoleUI::deleteCrewMember()
{
    clearScreen();

    std::cout
        << "========================================\n"
        << "           DELETE CREW MEMBER\n"
        << "========================================\n\n";

    if (crewMembers.empty())
    {
        std::cout
            << "No crew members found.\n";

        pause();
        return;
    }

    int crewId;

    std::cout
        << "Enter Crew Member ID: ";

    if (!(std::cin >> crewId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid Crew Member ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto crewIt =
        std::find_if(
            crewMembers.begin(),
            crewMembers.end(),
            [crewId](
                const std::shared_ptr<CrewMember>& member
            )
            {
                return member &&
                       member->getId() == crewId;
            }
        );

    if (crewIt == crewMembers.end())
    {
        std::cout
            << "\nCrew member not found.\n";

        pause();
        return;
    }

    auto crewMember = *crewIt;

    // Do not allow deletion while assigned to a flight.
    for (const auto& flight : flights)
    {
        if (!flight)
        {
            continue;
        }

        const auto& assignedCrew =
            flight->getCrewMembers();

        auto assignedIt =
            std::find_if(
                assignedCrew.begin(),
                assignedCrew.end(),
                [crewId](
                    const std::shared_ptr<CrewMember>& member
                )
                {
                    return member &&
                           member->getId() == crewId;
                }
            );

        if (assignedIt != assignedCrew.end())
        {
            std::cout
                << "\nCannot delete this crew member.\n"
                << "They are currently assigned to flight: "
                << flight->getFlightNumber()
                << "\n"
                << "Remove them from the flight first.\n";

            pause();
            return;
        }
    }

    std::cout
        << "\nCrew Member:\n"
        << "ID          : "
        << crewMember->getId()
        << "\n"
        << "Employee ID : "
        << crewMember->getEmployeeId()
        << "\n"
        << "Name        : "
        << crewMember->getFullName()
        << "\n"
        << "Role        : "
        << crewMember->getCrewRole()
        << "\n";

    std::cout
        << "\nAre you sure you want to delete this crew member?\n"
        << "Enter Y to confirm or N to cancel: ";

    char confirmation;

    std::cin >> confirmation;

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    if (confirmation != 'Y' &&
        confirmation != 'y')
    {
        std::cout
            << "\nDeletion cancelled.\n";

        pause();
        return;
    }

    crewMembers.erase(
        crewIt
    );

    try
    {
        CrewRepository repository;

        repository.save(
            crewMembers
        );

        std::cout
            << "\nCrew member deleted successfully.\n";
    }
    catch (const std::exception& e)
    {
        std::cout
            << "\nFailed to save crew data.\n"
            << e.what()
            << "\n";
    }

    pause();
}

// ============================================================
// CREW - CHANGE STATUS
// ============================================================

void ConsoleUI::changeCrewStatus()
{
    clearScreen();

    std::cout
        << "========================================\n"
        << "          CHANGE CREW STATUS\n"
        << "========================================\n\n";

    if (crewMembers.empty())
    {
        std::cout
            << "No crew members found.\n";

        pause();
        return;
    }

    int crewId;

    std::cout
        << "Enter Crew Member ID: ";

    if (!(std::cin >> crewId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid Crew Member ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto crewIt =
        std::find_if(
            crewMembers.begin(),
            crewMembers.end(),
            [crewId](
                const std::shared_ptr<CrewMember>& member
            )
            {
                return member &&
                       member->getId() == crewId;
            }
        );

    if (crewIt == crewMembers.end())
    {
        std::cout
            << "\nCrew member not found.\n";

        pause();
        return;
    }

    auto crewMember = *crewIt;

    std::cout
        << "\nCrew Member: "
        << crewMember->getFullName()
        << "\n"
        << "Role: "
        << crewMember->getCrewRole()
        << "\n"
        << "Current Status: "
        << (
            crewMember->getIsActive()
                ? "Active"
                : "Inactive"
        )
        << "\n\n"
        << "1. Activate\n"
        << "2. Deactivate\n"
        << "3. Cancel\n\n"
        << "Choose: ";

    int choice;

    if (!(std::cin >> choice))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid input.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    if (choice == 3)
    {
        return;
    }

    bool newStatus;

    switch (choice)
    {
        case 1:
            newStatus = true;
            break;

        case 2:
            newStatus = false;
            break;

        default:
            std::cout
                << "\nInvalid option.\n";

            pause();
            return;
    }

    try
    {
        crewMember->setIsActive(
            newStatus
        );

        CrewRepository repository;

        repository.save(
            crewMembers
        );

        std::cout
            << "\nCrew status changed successfully.\n"
            << "New Status: "
            << (
                crewMember->getIsActive()
                    ? "Active"
                    : "Inactive"
            )
            << "\n";
    }
    catch (const std::exception& e)
    {
        std::cout
            << "\nFailed to change crew status.\n"
            << e.what()
            << "\n";
    }

    pause();
}

// ============================================================
// CREW - ASSIGN TO FLIGHT
// ============================================================

void ConsoleUI::assignCrewToFlight()
{
    clearScreen();

    std::cout
        << "========================================\n"
        << "          ASSIGN CREW TO FLIGHT\n"
        << "========================================\n\n";

    if (crewMembers.empty())
    {
        std::cout
            << "No crew members found.\n";

        pause();
        return;
    }

    if (flights.empty())
    {
        std::cout
            << "No flights found.\n";

        pause();
        return;
    }

    std::cout
        << "Available Crew Members:\n\n";

    for (const auto& crewMember : crewMembers)
    {
        if (!crewMember)
        {
            continue;
        }

        std::cout
            << "ID: "
            << crewMember->getId()
            << " | "
            << crewMember->getFullName()
            << " | "
            << crewMember->getCrewRole()
            << " | Hours: "
            << crewMember->getTotalFlightHours()
            << "/"
            << crewMember->getMaximumFlightHours()
            << " | Status: "
            << (
                crewMember->getIsActive()
                    ? "Active"
                    : "Inactive"
            )
            << "\n";
    }

    std::cout
        << "\nEnter Crew Member ID: ";

    int crewId;

    if (!(std::cin >> crewId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid Crew Member ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto crewIt =
        std::find_if(
            crewMembers.begin(),
            crewMembers.end(),
            [crewId](
                const std::shared_ptr<CrewMember>& member
            )
            {
                return member &&
                       member->getId() == crewId;
            }
        );

    if (crewIt == crewMembers.end())
    {
        std::cout
            << "\nCrew member not found.\n";

        pause();
        return;
    }

    auto crewMember = *crewIt;

    if (!crewMember->getIsActive())
    {
        std::cout
            << "\nCannot assign an inactive crew member.\n";

        pause();
        return;
    }

    std::cout
        << "\nAvailable Flights:\n\n";

    for (const auto& flight : flights)
    {
        if (!flight)
        {
            continue;
        }

        std::cout
            << "Flight: "
            << flight->getFlightNumber()
            << " | "
            << flight->getOrigin()
            << " -> "
            << flight->getDestination()
            << " | Duration: "
            << flight->getFlightDurationHours()
            << " hours"
            << " | Status: "
            << flightStatusToString(
                   flight->getStatus()
               )
            << "\n";
    }

    std::string flightNumber;

    std::cout
        << "\nEnter Flight Number: ";

    std::getline(
        std::cin,
        flightNumber
    );

    auto flightIt =
        std::find_if(
            flights.begin(),
            flights.end(),
            [&flightNumber](
                const std::shared_ptr<Flight>& flight
            )
            {
                return flight &&
                       flight->getFlightNumber()
                           == flightNumber;
            }
        );

    if (flightIt == flights.end())
    {
        std::cout
            << "\nFlight not found.\n";

        pause();
        return;
    }

    auto flight = *flightIt;

    if (flight->getStatus() == FlightStatus::Cancelled)
    {
        std::cout
            << "\nCannot assign crew to a cancelled flight.\n";

        pause();
        return;
    }

    if (flight->getStatus() == FlightStatus::Completed)
    {
        std::cout
            << "\nCannot assign crew to a completed flight.\n";

        pause();
        return;
    }

    try
    {
        bool assigned =
            flight->assignCrewMember(
                crewMember
            );

        if (!assigned)
        {
            std::cout
                << "\nCrew member could not be assigned.\n"
                << "Possible reasons:\n"
                << "- Already assigned to this flight.\n"
                << "- Maximum flight hours would be exceeded.\n"
                << "- Crew member is inactive.\n";

            pause();
            return;
        }

        CrewRepository crewRepository;

        FlightRepository flightRepository;

        crewRepository.save(
            crewMembers
        );

        flightRepository.save(
            flights
        );

        std::cout
            << "\nCrew member assigned successfully.\n"
            << "Crew Member: "
            << crewMember->getFullName()
            << "\n"
            << "Flight: "
            << flight->getFlightNumber()
            << "\n"
            << "Total Flight Hours: "
            << crewMember->getTotalFlightHours()
            << "/"
            << crewMember->getMaximumFlightHours()
            << "\n";
    }
    catch (const std::exception& e)
    {
        std::cout
            << "\nFailed to assign crew member.\n"
            << e.what()
            << "\n";
    }

    pause();
}

// ============================================================
// CREW - REMOVE FROM FLIGHT
// ============================================================

void ConsoleUI::removeCrewFromFlight()
{
    clearScreen();

    std::cout
        << "========================================\n"
        << "         REMOVE CREW FROM FLIGHT\n"
        << "========================================\n\n";

    if (flights.empty())
    {
        std::cout
            << "No flights found.\n";

        pause();
        return;
    }

    bool hasAssignedCrew = false;

    for (const auto& flight : flights)
    {
        if (!flight)
        {
            continue;
        }

        if (!flight->getCrewMembers().empty())
        {
            hasAssignedCrew = true;

            std::cout
                << "Flight: "
                << flight->getFlightNumber()
                << "\n";

            for (const auto& crewMember :
                 flight->getCrewMembers())
            {
                if (!crewMember)
                {
                    continue;
                }

                std::cout
                    << "  Crew ID: "
                    << crewMember->getId()
                    << " | "
                    << crewMember->getFullName()
                    << " | "
                    << crewMember->getCrewRole()
                    << "\n";
            }

            std::cout << "\n";
        }
    }

    if (!hasAssignedCrew)
    {
        std::cout
            << "No crew members are currently assigned to flights.\n";

        pause();
        return;
    }

    std::string flightNumber;

    std::cout
        << "Enter Flight Number: ";

    std::getline(
        std::cin,
        flightNumber
    );

    auto flightIt =
        std::find_if(
            flights.begin(),
            flights.end(),
            [&flightNumber](
                const std::shared_ptr<Flight>& flight
            )
            {
                return flight &&
                       flight->getFlightNumber()
                           == flightNumber;
            }
        );

    if (flightIt == flights.end())
    {
        std::cout
            << "\nFlight not found.\n";

        pause();
        return;
    }

    auto flight = *flightIt;

    if (flight->getCrewMembers().empty())
    {
        std::cout
            << "\nNo crew members are assigned to this flight.\n";

        pause();
        return;
    }

    std::cout
        << "\nAssigned Crew:\n\n";

    for (const auto& crewMember :
         flight->getCrewMembers())
    {
        if (!crewMember)
        {
            continue;
        }

        std::cout
            << "ID: "
            << crewMember->getId()
            << " | "
            << crewMember->getFullName()
            << " | "
            << crewMember->getCrewRole()
            << "\n";
    }

    int crewId;

    std::cout
        << "\nEnter Crew Member ID to remove: ";

    if (!(std::cin >> crewId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid Crew Member ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto assignedCrewIt =
        std::find_if(
            flight->getCrewMembers().begin(),
            flight->getCrewMembers().end(),
            [crewId](
                const std::shared_ptr<CrewMember>& member
            )
            {
                return member &&
                       member->getId() == crewId;
            }
        );

    if (assignedCrewIt ==
        flight->getCrewMembers().end())
    {
        std::cout
            << "\nThis crew member is not assigned to this flight.\n";

        pause();
        return;
    }

    try
    {
        bool removed =
            flight->removeCrewMember(
                crewId
            );

        if (!removed)
        {
            std::cout
                << "\nCrew member could not be removed.\n";

            pause();
            return;
        }

        CrewRepository crewRepository;

        FlightRepository flightRepository;

        crewRepository.save(
            crewMembers
        );

        flightRepository.save(
            flights
        );

        std::cout
            << "\nCrew member removed successfully.\n";
    }
    catch (const std::exception& e)
    {
        std::cout
            << "\nFailed to remove crew member.\n"
            << e.what()
            << "\n";
    }

    pause();
}

// ============================================================
// MAINTENANCE MANAGEMENT
// ============================================================

void ConsoleUI::manageMaintenance()
{
    while (true)
    {
        clearScreen();

        std::cout
            << "=====================================\n"
            << " Maintenance Management\n"
            << "=====================================\n\n"

            << "1. List Maintenance Records\n"
            << "2. Schedule Maintenance\n"
            << "3. Add Replaced Part\n"
            << "4. Complete Maintenance\n"
            << "5. Back\n\n"

            << "Choose an option: ";

        int choice;

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout << "\nInvalid input.\n";
            pause();
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        switch (choice)
        {
        case 1:
            listMaintenanceRecords();
            break;

        case 2:
            scheduleMaintenance();
            break;

        case 3:
            addReplacedPart();
            break;

        case 4:
            completeMaintenance();
            break;

        case 5:
            return;

        default:
            std::cout << "\nInvalid option.\n";
            pause();
            break;
        }
    }
}

void ConsoleUI::listMaintenanceRecords() const
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << " Maintenance Records\n"
        << "=====================================\n\n";

    if (!maintenanceService)
    {
        std::cout
            << "Maintenance service is unavailable.\n";

        pause();
        return;
    }

    const auto& records =
        maintenanceService->getAllMaintenanceRecords();

    if (records.empty())
    {
        std::cout
            << "No maintenance records available.\n";

        pause();
        return;
    }

    for (const auto& maintenance : records)
    {
        if (!maintenance)
        {
            continue;
        }

        std::cout
            << "-------------------------------------\n"
            << "Maintenance ID: "
            << maintenance->getId()
            << "\n"

            << "Aircraft ID: "
            << maintenance->getAircraftId()
            << "\n"

            << "Date: "
            << maintenance->getMaintenanceDate()
            << "\n"

            << "Issue: "
            << maintenance->getIssueDescription()
            << "\n"

            << "Status: "
            << (
                maintenance->isCompleted()
                    ? "Completed"
                    : "In Progress"
            )
            << "\n"

            << "Replaced Parts:\n";

        const auto& parts =
            maintenance->getReplacedParts();

        if (parts.empty())
        {
            std::cout
                << "  None\n";
        }
        else
        {
            for (const auto& part : parts)
            {
                std::cout
                    << "  - "
                    << part
                    << "\n";
            }
        }
    }

    std::cout
        << "-------------------------------------\n";

    pause();
}

void ConsoleUI::scheduleMaintenance()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << " Schedule Maintenance\n"
        << "=====================================\n\n";

    if (aircraft.empty())
    {
        std::cout
            << "No aircraft available.\n";

        pause();
        return;
    }

    std::cout << "Available Aircraft:\n\n";

    for (const auto& currentAircraft : aircraft)
    {
        if (!currentAircraft)
        {
            continue;
        }

        std::cout
            << "ID: "
            << currentAircraft->getId()
            << " | Registration: "
            << currentAircraft->getRegistrationNumber()
            << " | Model: "
            << currentAircraft->getManufacturer()
            << " "
            << currentAircraft->getModel()
            << " | Status: "
            << aircraftStatusToString(
                   currentAircraft->getStatus()
               )
            << "\n";
    }

    int aircraftId;

    std::cout
        << "\nEnter Aircraft ID: ";

    if (!(std::cin >> aircraftId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid aircraft ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto aircraftIt =
        std::find_if(
            aircraft.begin(),
            aircraft.end(),
            [aircraftId](
                const std::shared_ptr<Aircraft>& currentAircraft
            )
            {
                return currentAircraft &&
                       currentAircraft->getId() == aircraftId;
            }
        );

    if (aircraftIt == aircraft.end())
    {
        std::cout
            << "\nAircraft not found.\n";

        pause();
        return;
    }

    std::string maintenanceDate;
    std::string issueDescription;

    std::cout
        << "Maintenance Date: ";

    std::getline(
        std::cin,
        maintenanceDate
    );

    std::cout
        << "Issue Description: ";

    std::getline(
        std::cin,
        issueDescription
    );

    try
    {
        auto maintenance =
            maintenanceService->scheduleMaintenance(
                *aircraftIt,
                maintenanceDate,
                issueDescription
            );

        AircraftRepository aircraftRepository;
        aircraftRepository.save(aircraft);

        MaintenanceRepository maintenanceRepository;
        maintenanceRepository.save(
            maintenanceService->getAllMaintenanceRecords()
        );

        std::cout
            << "\nMaintenance scheduled successfully!\n"
            << "Maintenance ID: "
            << maintenance->getId()
            << "\n";

        pause();
    }
    catch (const std::exception& e)
    {
        std::cout
            << "\nFailed to schedule maintenance.\n"
            << e.what()
            << "\n";

        pause();
    }
}

void ConsoleUI::addReplacedPart()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << " Add Replaced Part\n"
        << "=====================================\n\n";

    int maintenanceId;

    std::cout
        << "Enter Maintenance ID: ";

    if (!(std::cin >> maintenanceId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid maintenance ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto maintenance =
        maintenanceService->getMaintenance(
            maintenanceId
        );

    if (!maintenance)
    {
        std::cout
            << "\nMaintenance record not found.\n";

        pause();
        return;
    }

    if (maintenance->isCompleted())
    {
        std::cout
            << "\nMaintenance is already completed.\n";

        pause();
        return;
    }

    std::string part;

    std::cout
        << "Enter Replaced Part: ";

    std::getline(
        std::cin,
        part
    );

    try
    {
        if (
            maintenanceService->addReplacedPart(
                maintenanceId,
                part
            )
        )
        {
            MaintenanceRepository maintenanceRepository;

            maintenanceRepository.save(
                maintenanceService->getAllMaintenanceRecords()
            );

            std::cout
                << "\nReplaced part added successfully!\n";
        }
        else
        {
            std::cout
                << "\nFailed to add replaced part.\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cout
            << "\nFailed to add replaced part.\n"
            << e.what()
            << "\n";
    }

    pause();
}

void ConsoleUI::completeMaintenance()
{
    clearScreen();

    std::cout
        << "=====================================\n"
        << " Complete Maintenance\n"
        << "=====================================\n\n";

    int maintenanceId;

    std::cout
        << "Enter Maintenance ID: ";

    if (!(std::cin >> maintenanceId))
    {
        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout
            << "\nInvalid maintenance ID.\n";

        pause();
        return;
    }

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(),
        '\n'
    );

    auto maintenance =
        maintenanceService->getMaintenance(
            maintenanceId
        );

    if (!maintenance)
    {
        std::cout
            << "\nMaintenance record not found.\n";

        pause();
        return;
    }

    auto aircraftIt =
        std::find_if(
            aircraft.begin(),
            aircraft.end(),
            [maintenance](
                const std::shared_ptr<Aircraft>& currentAircraft
            )
            {
                return currentAircraft &&
                       currentAircraft->getId()
                           == maintenance->getAircraftId();
            }
        );

    if (aircraftIt == aircraft.end())
    {
        std::cout
            << "\nAircraft associated with this maintenance "
            << "record was not found.\n";

        pause();
        return;
    }

    try
    {
        if (
            maintenanceService->completeMaintenance(
                maintenanceId,
                *aircraftIt
            )
        )
        {
            AircraftRepository aircraftRepository;

            aircraftRepository.save(aircraft);

            MaintenanceRepository maintenanceRepository;

            maintenanceRepository.save(
                maintenanceService->getAllMaintenanceRecords()
            );

            std::cout
                << "\nMaintenance completed successfully!\n"
                << "Aircraft status changed to Available.\n";
        }
        else
        {
            std::cout
                << "\nFailed to complete maintenance.\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cout
            << "\nFailed to complete maintenance.\n"
            << e.what()
            << "\n";
    }

    pause();
}

void ConsoleUI::showReportsMenu()
{
    while (true)
    {
        clearScreen();

        std::cout
            << "=====================================\n"
            << " Reports & Analytics\n"
            << "=====================================\n\n"

            << "1. Flight Performance Report\n"
            << "2. Reservation Statistics Report\n"
            << "3. Financial Summary Report\n"
            << "4. Aircraft Utilization Report\n"
            << "5. Maintenance Report\n"
            << "6. Back\n\n"

            << "Choose an option: ";

        int choice;

        if (!(std::cin >> choice))
        {
            std::cin.clear();

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout
                << "\nInvalid input.\n";

            pause();
            continue;
        }

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        switch (choice)
        {
        case 1:
            generateFlightPerformanceReport();
            break;

        case 2:
            generateReservationStatisticsReport();
            break;

        case 3:
            generateFinancialSummaryReport();
            break;

        case 4:
            generateAircraftUtilizationReport();
            break;

        case 5:
            generateMaintenanceReport();
            break;

        case 6:
            return;

        default:
            std::cout
                << "\nInvalid option.\n";

            pause();
            break;
        }
    }
}

void ConsoleUI::generateFlightPerformanceReport()
{
    clearScreen();

    ReportService reportService;

    reportService.generateFlightPerformanceReport(
        flights
    );

    pause();
}

void ConsoleUI::generateReservationStatisticsReport()
{
    clearScreen();

    std::vector<std::shared_ptr<Passenger>>
        passengers;

    for (const auto& user : users)
    {
        auto passenger =
            std::dynamic_pointer_cast<Passenger>(
                user
            );

        if (passenger)
        {
            passengers.push_back(passenger);
        }
    }

    ReservationRepository reservationRepository;

    auto reservations =
        reservationRepository.load(
            passengers,
            flights
        );

    ReportService reportService;

    reportService.generateReservationStatisticsReport(
        reservations
    );

    pause();
}

void ConsoleUI::generateFinancialSummaryReport()
{
    clearScreen();

    PaymentRepository paymentRepository;

    auto payments =
        paymentRepository.load();

    ReportService reportService;

    reportService.generateFinancialSummaryReport(
        payments
    );

    pause();
}

void ConsoleUI::generateAircraftUtilizationReport()
{
    clearScreen();

    ReportService reportService;

    reportService.generateAircraftUtilizationReport(
        aircraft
    );

    pause();
}

void ConsoleUI::generateMaintenanceReport()
{
    clearScreen();

    if (!maintenanceService)
    {
        std::cout
            << "Maintenance service is unavailable.\n";

        pause();
        return;
    }

    ReportService reportService;

    reportService.generateMaintenanceReport(
        maintenanceService->getAllMaintenanceRecords()
    );

    pause();
}
// ============================================================
// LOGOUT
// ============================================================

void ConsoleUI::logout()
{
    currentUser = nullptr;

    std::cout
        << "\nLogged out successfully.\n";

    pause();
}

// ============================================================
// UTILITY
// ============================================================

void ConsoleUI::pause() const
{
    std::cout
        << "\nPress Enter to continue...";

    std::cin.get();
}

void ConsoleUI::clearScreen() const
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}