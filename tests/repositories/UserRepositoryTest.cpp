#include <iostream>
#include <memory>
#include <vector>

#include "models/Administrator.h"
#include "models/BookingAgent.h"
#include "models/Passenger.h"
#include "repositories/UserRepository.h"
#include "utils/PasswordHasher.h"

int main()
{
    try
    {
        std::vector<std::shared_ptr<User>> users;

        const std::string adminPasswordHash =
            PasswordHasher::hashPassword("admin123");

        const std::string agentPasswordHash =
            PasswordHasher::hashPassword("agent123");

        const std::string passengerPasswordHash =
            PasswordHasher::hashPassword("pass123");

        users.push_back(
            std::make_shared<Administrator>(
                1,
                "admin",
                adminPasswordHash,
                "System Admin",
                "admin@example.com",
                "01000000001"
            )
        );

        users.push_back(
            std::make_shared<BookingAgent>(
                2,
                "agent",
                agentPasswordHash,
                "Booking Agent",
                "agent@example.com",
                "01000000002"
            )
        );

        auto passenger =
            std::make_shared<Passenger>(
                3,
                "passenger",
                passengerPasswordHash,
                "Passenger One",
                "passenger@example.com",
                "01000000003",
                "P12345678",
                150
            );

        passenger->setPreferredSeat("12A");
        passenger->setMealPreference("Vegetarian");

        users.push_back(passenger);

        UserRepository repository;

        std::cout
            << "Saving users...\n";

        repository.save(users);

        std::cout
            << "Users saved successfully.\n\n";

        users.clear();

        std::cout
            << "Memory cleared.\n"
            << "Users in memory: "
            << users.size()
            << "\n\n";

        users = repository.load();

        std::cout
            << "Users loaded: "
            << users.size()
            << "\n\n";

        for (const auto& user : users)
        {
            std::cout
                << "-----------------------------\n";

            std::cout
                << "ID: "
                << user->getId()
                << "\n";

            std::cout
                << "Username: "
                << user->getUsername()
                << "\n";

            std::cout
                << "Name: "
                << user->getFullName()
                << "\n";

            std::cout
                << "Email: "
                << user->getEmail()
                << "\n";

            std::cout
                << "-----------------------------\n\n";
        }

        if (users.size() != 3)
        {
            throw std::runtime_error(
                "User persistence failed."
            );
        }

        if (
            users[0]->getRole()
            != Role::Administrator
        )
        {
            throw std::runtime_error(
                "Administrator role persistence failed."
            );
        }

        if (
            users[1]->getRole()
            != Role::BookingAgent
        )
        {
            throw std::runtime_error(
                "Booking Agent role persistence failed."
            );
        }

        if (
            users[2]->getRole()
            != Role::Passenger
        )
        {
            throw std::runtime_error(
                "Passenger role persistence failed."
            );
        }

        auto loadedPassenger =
            std::dynamic_pointer_cast<Passenger>(
                users[2]
            );

        if (!loadedPassenger)
        {
            throw std::runtime_error(
                "Passenger polymorphism failed."
            );
        }

        if (
            loadedPassenger->getPassportNumber()
            != "P12345678"
        )
        {
            throw std::runtime_error(
                "Passport persistence failed."
            );
        }

        if (
            loadedPassenger->getLoyaltyPoints()
            != 150
        )
        {
            throw std::runtime_error(
                "Loyalty points persistence failed."
            );
        }

        if (
            loadedPassenger->getPreferredSeat()
            != "12A"
        )
        {
            throw std::runtime_error(
                "Preferred seat persistence failed."
            );
        }

        if (
            loadedPassenger->getMealPreference()
            != "Vegetarian"
        )
        {
            throw std::runtime_error(
                "Meal preference persistence failed."
            );
        }

        std::cout
            << "User persistence test "
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