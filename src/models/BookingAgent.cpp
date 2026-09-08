#include "models/BookingAgent.h"
#include <iostream>

BookingAgent::BookingAgent(
    int id,
    const std::string& username,
    const std::string& passwordHash,
    const std::string& fullName,
    const std::string& email,
    const std::string& phone,
    bool isActive
)
    : User(
        id,
        username,
        passwordHash,
        fullName,
        email,
        phone,
        Role::BookingAgent,
        isActive
    )
{
}

void BookingAgent::displayMenu() const
{
    std::cout << "\n";
    std::cout << "===== Booking Agent Menu =====\n";
    std::cout << "1. Search Flights\n";
    std::cout << "2. Create Reservation\n";
    std::cout << "3. Modify Reservation\n";
    std::cout << "4. Cancel Reservation\n";
    std::cout << "5. Airport Check-In\n";
    std::cout << "6. Process Payment\n";
    std::cout << "7. Manage Passengers\n";
    std::cout << "0. Logout\n";
}