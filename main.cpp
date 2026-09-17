#include <iostream>
#include <memory>
#include <vector>

#include "models/User.h"
#include "repositories/UserRepository.h"
#include "services/AuthenticationService.h"
#include "services/UserManagementService.h"
#include "ui/ConsoleUI.h"

int main()
{
    try
    {
        std::vector<std::shared_ptr<User>> users;

        UserRepository userRepository;

        users = userRepository.load();

        UserManagementService userManagementService(users);

        AuthenticationService authenticationService(users);

        ConsoleUI consoleUI(
            users,
            authenticationService,
            userManagementService
        );

        consoleUI.run();
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "Fatal error: "
            << e.what()
            << "\n";

        return 1;
    }

    return 0;
}