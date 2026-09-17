#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "models/Administrator.h"
#include "models/BookingAgent.h"
#include "models/Passenger.h"
#include "services/UserManagementService.h"
#include "utils/PasswordHasher.h"

int main()
{
    try
    {
        std::vector<std::shared_ptr<User>> users;

        UserManagementService userManagementService(users);

        // Create Administrator
        auto administrator =
            userManagementService.createAdministrator(
                "admin",
                "admin123",
                "System Administrator",
                "admin@airline.com",
                "01000000000"
            );

        if (!administrator)
        {
            throw std::runtime_error(
                "Administrator creation failed."
            );
        }

        if (administrator->getId() != 1)
        {
            throw std::runtime_error(
                "Administrator ID generation failed."
            );
        }

        if (administrator->getRole()
            != Role::Administrator)
        {
            throw std::runtime_error(
                "Administrator role failed."
            );
        }

        if (!PasswordHasher::verifyPassword(
                "admin123",
                administrator->getPasswordHash()))
        {
            throw std::runtime_error(
                "Administrator password hashing failed."
            );
        }

        std::cout
            << "Administrator creation test passed!\n";


        // Create Booking Agent
        auto bookingAgent =
            userManagementService.createBookingAgent(
                "agent",
                "agent123",
                "Booking Agent",
                "agent@airline.com",
                "01111111111"
            );

        if (!bookingAgent)
        {
            throw std::runtime_error(
                "Booking Agent creation failed."
            );
        }

        if (bookingAgent->getId() != 2)
        {
            throw std::runtime_error(
                "Booking Agent ID generation failed."
            );
        }

        if (bookingAgent->getRole()
            != Role::BookingAgent)
        {
            throw std::runtime_error(
                "Booking Agent role failed."
            );
        }

        if (!PasswordHasher::verifyPassword(
                "agent123",
                bookingAgent->getPasswordHash()))
        {
            throw std::runtime_error(
                "Booking Agent password hashing failed."
            );
        }

        std::cout
            << "Booking Agent creation test passed!\n";


        // Create Passenger
        auto passenger =
            userManagementService.createPassenger(
                "medhat",
                "pass123",
                "Medhat Adel",
                "medhat@airline.com",
                "01222222222",
                "P1234567"
            );

        if (!passenger)
        {
            throw std::runtime_error(
                "Passenger creation failed."
            );
        }

        if (passenger->getId() != 3)
        {
            throw std::runtime_error(
                "Passenger ID generation failed."
            );
        }

        if (passenger->getRole()
            != Role::Passenger)
        {
            throw std::runtime_error(
                "Passenger role failed."
            );
        }

        if (!PasswordHasher::verifyPassword(
                "pass123",
                passenger->getPasswordHash()))
        {
            throw std::runtime_error(
                "Passenger password hashing failed."
            );
        }

        auto loadedPassenger =
            std::dynamic_pointer_cast<Passenger>(
                passenger
            );

        if (!loadedPassenger)
        {
            throw std::runtime_error(
                "Passenger polymorphism failed."
            );
        }

        if (loadedPassenger->getPassportNumber()
            != "P1234567")
        {
            throw std::runtime_error(
                "Passenger passport failed."
            );
        }

        if (loadedPassenger->getLoyaltyPoints() != 0)
        {
            throw std::runtime_error(
                "Passenger initial loyalty points failed."
            );
        }

        std::cout
            << "Passenger creation test passed!\n";


        // Username search
        auto foundUser =
            userManagementService.findUserByUsername(
                "medhat"
            );

        if (!foundUser)
        {
            throw std::runtime_error(
                "Username search failed."
            );
        }

        if (foundUser->getId() != 3)
        {
            throw std::runtime_error(
                "Username search returned wrong user."
            );
        }

        std::cout
            << "Username search test passed!\n";


        // ID search
        auto foundById =
            userManagementService.findUserById(2);

        if (!foundById)
        {
            throw std::runtime_error(
                "ID search failed."
            );
        }

        if (foundById->getUsername() != "agent")
        {
            throw std::runtime_error(
                "ID search returned wrong user."
            );
        }

        std::cout
            << "ID search test passed!\n";


        // User count
        if (userManagementService.getAllUsers().size() != 3)
        {
            throw std::runtime_error(
                "User count test failed."
            );
        }

        std::cout
            << "User count test passed!\n";


        // Duplicate username
        try
        {
            userManagementService.createPassenger(
                "medhat",
                "another123",
                "Another Passenger",
                "another@airline.com",
                "01333333333",
                "P9999999"
            );

            throw std::runtime_error(
                "Duplicate username test failed."
            );
        }
        catch (const std::invalid_argument&)
        {
        }

        std::cout
            << "Duplicate username test passed!\n";


        // Deactivate user
        userManagementService.deactivateUser(3);

        if (passenger->getIsActive())
        {
            throw std::runtime_error(
                "User deactivation failed."
            );
        }

        std::cout
            << "User deactivation test passed!\n";


        // Activate user
        userManagementService.activateUser(3);

        if (!passenger->getIsActive())
        {
            throw std::runtime_error(
                "User activation failed."
            );
        }

        std::cout
            << "User activation test passed!\n";


        // Non-existing user
        if (
            userManagementService.findUserByUsername(
                "unknown"
            )
        )
        {
            throw std::runtime_error(
                "Unknown username search failed."
            );
        }

        std::cout
            << "Unknown username test passed!\n";


        std::cout
            << "\n"
            << "User management service test "
            << "completed successfully!\n";
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "Test failed: "
            << e.what()
            << "\n";

        return 1;
    }

    return 0;
}