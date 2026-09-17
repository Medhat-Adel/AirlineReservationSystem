#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "models/Administrator.h"
#include "models/BookingAgent.h"
#include "models/Passenger.h"
#include "services/AuthenticationService.h"
#include "utils/PasswordHasher.h"

int main()
{
    const std::string adminPasswordHash =
        PasswordHasher::hashPassword("admin123");

    const std::string agentPasswordHash =
        PasswordHasher::hashPassword("agent123");

    const std::string passengerPasswordHash =
        PasswordHasher::hashPassword("pass123");

    const std::string inactivePasswordHash =
        PasswordHasher::hashPassword("inactive123");

    std::vector<std::shared_ptr<User>> users;

    users.push_back(
        std::make_shared<Administrator>(
            1,
            "admin",
            adminPasswordHash,
            "System Administrator",
            "admin@airline.com",
            "01000000000"
        )
    );

    users.push_back(
        std::make_shared<BookingAgent>(
            2,
            "agent",
            agentPasswordHash,
            "Booking Agent",
            "agent@airline.com",
            "01111111111"
        )
    );

    users.push_back(
        std::make_shared<Passenger>(
            3,
            "medhat",
            passengerPasswordHash,
            "Medhat Adel",
            "medhat@airline.com",
            "01222222222",
            "P1234567",
            0
        )
    );

    users.push_back(
        std::make_shared<Passenger>(
            4,
            "inactive",
            inactivePasswordHash,
            "Inactive User",
            "inactive@airline.com",
            "01555555555",
            "P7654321",
            0,
            false
        )
    );

    AuthenticationService authenticationService(users);

    // Correct administrator login
    auto admin = authenticationService.login(
        "admin",
        "admin123"
    );

    if (!admin)
    {
        throw std::runtime_error(
            "Administrator login failed."
        );
    }

    std::cout
        << "Correct administrator login test passed!\n";

    // Correct passenger login
    auto passenger = authenticationService.login(
        "medhat",
        "pass123"
    );

    if (!passenger)
    {
        throw std::runtime_error(
            "Passenger login failed."
        );
    }

    std::cout
        << "Correct passenger login test passed!\n";

    // Wrong password
    try
    {
        authenticationService.login(
            "admin",
            "wrongpassword"
        );

        throw std::runtime_error(
            "Wrong password test failed."
        );
    }
    catch (const std::runtime_error&)
    {
    }

    std::cout
        << "Wrong password test passed!\n";

    // Unknown username
    try
    {
        authenticationService.login(
            "unknown",
            "admin123"
        );

        throw std::runtime_error(
            "Unknown username test failed."
        );
    }
    catch (const std::runtime_error&)
    {
    }

    std::cout
        << "Unknown username test passed!\n";

    // Empty username
    try
    {
        authenticationService.login(
            "",
            "admin123"
        );

        throw std::runtime_error(
            "Empty username test failed."
        );
    }
    catch (const std::invalid_argument&)
    {
    }

    std::cout
        << "Empty username test passed!\n";

    // Empty password
    try
    {
        authenticationService.login(
            "admin",
            ""
        );

        throw std::runtime_error(
            "Empty password test failed."
        );
    }
    catch (const std::invalid_argument&)
    {
    }

    std::cout
        << "Empty password test passed!\n";

    // Inactive user
    try
    {
        authenticationService.login(
            "inactive",
            "inactive123"
        );

        throw std::runtime_error(
            "Inactive user test failed."
        );
    }
    catch (const std::runtime_error&)
    {
    }

    std::cout
        << "Inactive user test passed!\n";

    std::cout << "\n";
    std::cout
        << "Authentication service test completed successfully!\n";

    return 0;
}