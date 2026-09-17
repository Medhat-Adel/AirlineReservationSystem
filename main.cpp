#include <iostream>
#include <memory>
#include <vector>
#include <utility>

#include "models/User.h"
#include "repositories/UserRepository.h"
#include "ui/ConsoleUI.h"
#include "services/UserManagementService.h"
#include "services/AuthenticationService.h"
#include "services/MaintenanceService.h"
#include "services/PaymentService.h"
#include "services/LoyaltyService.h"
#include "services/BookingService.h"

int main()
{
    try
    {
        std::vector<std::shared_ptr<User>> users;

        UserRepository userRepository;

        users = userRepository.load();

        UserManagementService userManagementService(users);

        AuthenticationService authenticationService(users);

        auto maintenanceService =
            std::make_unique<MaintenanceService>();

        PaymentService paymentService;
        LoyaltyService loyaltyService;

        BookingService bookingService(
            paymentService,
            loyaltyService
        );

    ConsoleUI consoleUI(
        users,
        authenticationService,
        userManagementService,
        std::move(maintenanceService),
        bookingService
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