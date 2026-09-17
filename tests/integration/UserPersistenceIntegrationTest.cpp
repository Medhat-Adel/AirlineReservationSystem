#include <cassert>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "models/User.h"
#include "models/Passenger.h"

#include "repositories/UserRepository.h"

#include "services/UserManagementService.h"
#include "services/AuthenticationService.h"

#include "utils/PasswordHasher.h"

int main()
{
    const std::string testFile =
        "data/integration_users_test.json";

    try
    {
        // =================================================
        // CLEANUP OLD TEST FILE
        // =================================================

        std::filesystem::remove(testFile);

        // =================================================
        // STEP 1: CREATE USERS
        // =================================================

        std::cout
            << "\n[1] Creating users...\n";

        std::vector<std::shared_ptr<User>> users;

        UserManagementService userManagementService(users);

        auto administrator =
            userManagementService.createAdministrator(
                "integration_admin",
                "Admin@123",
                "Integration Administrator",
                "admin@test.com",
                "01000000000"
            );

        auto passenger =
            userManagementService.createPassenger(
                "integration_passenger",
                "Passenger@123",
                "Integration Passenger",
                "passenger@test.com",
                "01111111111",
                "P123456"
            );

        assert(administrator != nullptr);
        assert(passenger != nullptr);

        assert(users.size() == 2);

        std::cout
            << "Users created successfully.\n";

        // =================================================
        // STEP 2: SET PASSENGER PREFERENCES
        // =================================================

        std::cout
            << "\n[2] Setting passenger preferences...\n";

        auto passengerModel =
            std::dynamic_pointer_cast<Passenger>(
                passenger
            );

        assert(passengerModel != nullptr);

        passengerModel->setPreferredSeat("12A");
        passengerModel->setMealPreference("Vegetarian");

        std::cout
            << "Passenger preferences set successfully.\n";

        // =================================================
        // STEP 3: VERIFY ORIGINAL PASSWORDS
        // =================================================

        std::cout
            << "\n[3] Verifying original password hashes...\n";

        assert(
            PasswordHasher::verifyPassword(
                "Admin@123",
                administrator->getPasswordHash()
            )
        );

        assert(
            PasswordHasher::verifyPassword(
                "Passenger@123",
                passenger->getPasswordHash()
            )
        );

        std::cout
            << "Original password verification PASSED.\n";

        // =================================================
        // STEP 4: SAVE USERS
        // =================================================

        std::cout
            << "\n[4] Saving users...\n";

        UserRepository repository(testFile);

        repository.save(users);

        assert(
            std::filesystem::exists(testFile)
        );

        std::cout
            << "Users saved successfully.\n";

        // =================================================
        // STEP 5: CLEAR MEMORY
        // =================================================

        std::cout
            << "\n[5] Clearing memory...\n";

        users.clear();

        assert(users.empty());

        std::cout
            << "Memory cleared successfully.\n";

        // =================================================
        // STEP 6: LOAD USERS
        // =================================================

        std::cout
            << "\n[6] Loading users from JSON...\n";

        users = repository.load();

        assert(users.size() == 2);

        std::cout
            << "Users loaded: "
            << users.size()
            << "\n";

        // =================================================
        // STEP 7: FIND LOADED ADMINISTRATOR
        // =================================================

        std::shared_ptr<User> loadedAdministrator;

        std::shared_ptr<User> loadedPassengerUser;

        for (const auto& user : users)
        {
            if (!user)
            {
                continue;
            }

            std::cout
                << "\nLoaded User:"
                << "\nUsername: "
                << user->getUsername()
                << "\nFull Name: "
                << user->getFullName()
                << "\nRole: "
                << static_cast<int>(user->getRole())
                << "\nPassword Hash: "
                << user->getPasswordHash()
                << "\n";

            if (user->getUsername()
                == "integration_admin")
            {
                loadedAdministrator = user;
            }

            if (user->getUsername()
                == "integration_passenger")
            {
                loadedPassengerUser = user;
            }
        }

        assert(
            loadedAdministrator != nullptr
        );

        assert(
            loadedPassengerUser != nullptr
        );

        std::cout
            << "\nAdministrator loaded successfully.\n";

        std::cout
            << "Passenger loaded successfully.\n";

        // =================================================
        // STEP 8: VERIFY LOADED ADMIN DATA
        // =================================================

        std::cout
            << "\n[7] Verifying administrator data...\n";

        assert(
            loadedAdministrator->getUsername()
            == "integration_admin"
        );

        assert(
            loadedAdministrator->getFullName()
            == "Integration Administrator"
        );

        assert(
            loadedAdministrator->getEmail()
            == "admin@test.com"
        );

        assert(
            loadedAdministrator->getPhone()
            == "01000000000"
        );

        assert(
            loadedAdministrator->getRole()
            == Role::Administrator
        );

        assert(
            loadedAdministrator->getIsActive()
        );

        std::cout
            << "Administrator data verification PASSED.\n";

        // =================================================
        // STEP 9: VERIFY LOADED PASSENGER DATA
        // =================================================

        std::cout
            << "\n[8] Verifying passenger data...\n";

        auto loadedPassenger =
            std::dynamic_pointer_cast<Passenger>(
                loadedPassengerUser
            );

        assert(loadedPassenger != nullptr);

        assert(
            loadedPassenger->getUsername()
            == "integration_passenger"
        );

        assert(
            loadedPassenger->getFullName()
            == "Integration Passenger"
        );

        assert(
            loadedPassenger->getEmail()
            == "passenger@test.com"
        );

        assert(
            loadedPassenger->getPhone()
            == "01111111111"
        );

        assert(
            loadedPassenger->getPassportNumber()
            == "P123456"
        );

        assert(
            loadedPassenger->getRole()
            == Role::Passenger
        );

        assert(
            loadedPassenger->getIsActive()
        );

        assert(
            loadedPassenger->getPreferredSeat()
            == "12A"
        );

        assert(
            loadedPassenger->getMealPreference()
            == "Vegetarian"
        );

        std::cout
            << "Passenger data verification PASSED.\n";

        // =================================================
        // STEP 10: VERIFY LOADED PASSWORD HASHES
        // =================================================

        std::cout
            << "\n[9] Verifying loaded password hashes...\n";

        bool adminPasswordCorrect =
            PasswordHasher::verifyPassword(
                "Admin@123",
                loadedAdministrator->getPasswordHash()
            );

        bool passengerPasswordCorrect =
            PasswordHasher::verifyPassword(
                "Passenger@123",
                loadedPassenger->getPasswordHash()
            );

        std::cout
            << "Admin password verification result: "
            << (adminPasswordCorrect ? "TRUE" : "FALSE")
            << "\n";

        std::cout
            << "Passenger password verification result: "
            << (passengerPasswordCorrect ? "TRUE" : "FALSE")
            << "\n";

        assert(adminPasswordCorrect);
        assert(passengerPasswordCorrect);

        std::cout
            << "Loaded password verification PASSED.\n";

        // =================================================
        // STEP 11: CREATE AUTHENTICATION SERVICE
        // =================================================

        std::cout
            << "\n[10] Creating AuthenticationService...\n";

        AuthenticationService authenticationService(users);

        std::cout
            << "AuthenticationService created successfully.\n";

        // =================================================
        // STEP 12: ADMIN LOGIN
        // =================================================

        std::cout
            << "\n[11] Testing administrator login...\n";

        std::shared_ptr<User> loggedInAdministrator;

        try
        {
            loggedInAdministrator =
                authenticationService.login(
                    "integration_admin",
                    "Admin@123"
                );

            std::cout
                << "Administrator login returned successfully.\n";
        }
        catch (const std::exception& e)
        {
            std::cerr
                << "Administrator login THREW EXCEPTION:\n"
                << e.what()
                << "\n";

            throw;
        }

        assert(
            loggedInAdministrator != nullptr
        );

        assert(
            loggedInAdministrator->getUsername()
            == "integration_admin"
        );

        assert(
            loggedInAdministrator->getRole()
            == Role::Administrator
        );

        std::cout
            << "Administrator login PASSED.\n";

        // =================================================
        // STEP 13: PASSENGER LOGIN
        // =================================================

        std::cout
            << "\n[12] Testing passenger login...\n";

        std::shared_ptr<User> loggedInPassenger;

        try
        {
            loggedInPassenger =
                authenticationService.login(
                    "integration_passenger",
                    "Passenger@123"
                );

            std::cout
                << "Passenger login returned successfully.\n";
        }
        catch (const std::exception& e)
        {
            std::cerr
                << "Passenger login THREW EXCEPTION:\n"
                << e.what()
                << "\n";

            throw;
        }

        assert(
            loggedInPassenger != nullptr
        );

        assert(
            loggedInPassenger->getUsername()
            == "integration_passenger"
        );

        assert(
            loggedInPassenger->getRole()
            == Role::Passenger
        );

        std::cout
            << "Passenger login PASSED.\n";

        // =================================================
        // STEP 14: WRONG PASSWORD MUST FAIL
        // =================================================

        std::cout
            << "\n[13] Testing wrong password...\n";

        try
        {
            authenticationService.login(
                "integration_admin",
                "WrongPassword"
            );

            std::cerr
                << "ERROR: Wrong password was accepted!\n";

            return 1;
        }
        catch (const std::exception&)
        {
            std::cout
                << "Wrong password correctly rejected.\n";
        }

        // =================================================
        // CLEANUP
        // =================================================

        std::filesystem::remove(testFile);

        std::cout
            << "\n========================================\n"
            << "USER PERSISTENCE INTEGRATION TEST PASSED\n"
            << "========================================\n";

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "\n========================================\n"
            << "INTEGRATION TEST FAILED\n"
            << "========================================\n"
            << e.what()
            << "\n";

        std::filesystem::remove(testFile);

        return 1;
    }
}