#include "services/PassengerService.h"

#include <iostream>
#include <stdexcept>

void PassengerService::updateContactInformation(
    const std::shared_ptr<Passenger>& passenger,
    const std::string& email,
    const std::string& phone
) const
{
    if (passenger == nullptr)
    {
        throw std::invalid_argument(
            "Passenger cannot be null."
        );
    }

    if (email.empty())
    {
        throw std::invalid_argument(
            "Email cannot be empty."
        );
    }

    if (phone.empty())
    {
        throw std::invalid_argument(
            "Phone cannot be empty."
        );
    }

    passenger->setEmail(email);
    passenger->setPhone(phone);
}

void PassengerService::setPreferences(
    const std::shared_ptr<Passenger>& passenger,
    const std::string& preferredSeat,
    const std::string& mealPreference
) const
{
    if (passenger == nullptr)
    {
        throw std::invalid_argument(
            "Passenger cannot be null."
        );
    }

    passenger->setPreferredSeat(
        preferredSeat
    );

    passenger->setMealPreference(
        mealPreference
    );
}

void PassengerService::addTravelHistory(
    const std::shared_ptr<Passenger>& passenger,
    const std::shared_ptr<Flight>& flight
) const
{
    if (passenger == nullptr)
    {
        throw std::invalid_argument(
            "Passenger cannot be null."
        );
    }

    if (flight == nullptr)
    {
        throw std::invalid_argument(
            "Flight cannot be null."
        );
    }

    passenger->addTravelHistory(flight);
}

const std::vector<std::shared_ptr<Flight>>&
PassengerService::getTravelHistory(
    const std::shared_ptr<Passenger>& passenger
) const
{
    if (passenger == nullptr)
    {
        throw std::invalid_argument(
            "Passenger cannot be null."
        );
    }

    return passenger->getTravelHistory();
}

void PassengerService::displayProfile(
    const std::shared_ptr<Passenger>& passenger
) const
{
    if (passenger == nullptr)
    {
        throw std::invalid_argument(
            "Passenger cannot be null."
        );
    }

    std::cout << "\n========================================\n";
    std::cout << "          PASSENGER PROFILE\n";
    std::cout << "========================================\n";

    std::cout << "Name: "
              << passenger->getFullName()
              << '\n';

    std::cout << "Username: "
              << passenger->getUsername()
              << '\n';

    std::cout << "Email: "
              << passenger->getEmail()
              << '\n';

    std::cout << "Phone: "
              << passenger->getPhone()
              << '\n';

    std::cout << "Passport Number: "
              << passenger->getPassportNumber()
              << '\n';

    std::cout << "Preferred Seat: "
              << passenger->getPreferredSeat()
              << '\n';

    std::cout << "Meal Preference: "
              << passenger->getMealPreference()
              << '\n';

    std::cout << "Loyalty Points: "
              << passenger->getLoyaltyPoints()
              << '\n';

    std::cout << "Travel History: "
              << passenger->getTravelHistory().size()
              << " flight(s)\n";

    std::cout << "========================================\n";
}