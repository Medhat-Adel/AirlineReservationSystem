#include "models/Administrator.h"
#include <iostream>

Administrator::Administrator(
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
        Role::Administrator,
        isActive
    )
{
}

void Administrator::displayMenu() const
{
    std::cout << "\n";
    std::cout << "===== Administrator Menu =====\n";
    std::cout << "1. Manage Users\n";
    std::cout << "2. Manage Flights\n";
    std::cout << "3. Manage Aircraft\n";
    std::cout << "4. Manage Crew\n";
    std::cout << "5. Maintenance Management\n";
    std::cout << "6. Reports & Analytics\n";
    std::cout << "0. Logout\n";
}