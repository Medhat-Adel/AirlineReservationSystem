#include "services/UserManagementService.h"

#include <stdexcept>

#include "models/Administrator.h"
#include "models/BookingAgent.h"
#include "models/Passenger.h"
#include "utils/PasswordHasher.h"

UserManagementService::UserManagementService(
    std::vector<std::shared_ptr<User>>& users
)
    : users(users),
      nextUserId(1)
{
    for (const auto& user : users)
    {
        if (user && user->getId() >= nextUserId)
        {
            nextUserId = user->getId() + 1;
        }
    }
}

std::shared_ptr<User> UserManagementService::createAdministrator(
    const std::string& username,
    const std::string& password,
    const std::string& fullName,
    const std::string& email,
    const std::string& phone
)
{
    if (username.empty())
    {
        throw std::invalid_argument(
            "Username cannot be empty."
        );
    }

    if (findUserByUsername(username))
    {
        throw std::invalid_argument(
            "Username already exists."
        );
    }

    const std::string passwordHash =
        PasswordHasher::hashPassword(password);

    auto administrator =
        std::make_shared<Administrator>(
            nextUserId++,
            username,
            passwordHash,
            fullName,
            email,
            phone
        );

    users.push_back(administrator);

    return administrator;
}

std::shared_ptr<User> UserManagementService::createBookingAgent(
    const std::string& username,
    const std::string& password,
    const std::string& fullName,
    const std::string& email,
    const std::string& phone
)
{
    if (username.empty())
    {
        throw std::invalid_argument(
            "Username cannot be empty."
        );
    }

    if (findUserByUsername(username))
    {
        throw std::invalid_argument(
            "Username already exists."
        );
    }

    const std::string passwordHash =
        PasswordHasher::hashPassword(password);

    auto bookingAgent =
        std::make_shared<BookingAgent>(
            nextUserId++,
            username,
            passwordHash,
            fullName,
            email,
            phone
        );

    users.push_back(bookingAgent);

    return bookingAgent;
}

std::shared_ptr<User> UserManagementService::createPassenger(
    const std::string& username,
    const std::string& password,
    const std::string& fullName,
    const std::string& email,
    const std::string& phone,
    const std::string& passportNumber
)
{
    if (username.empty())
    {
        throw std::invalid_argument(
            "Username cannot be empty."
        );
    }

    if (findUserByUsername(username))
    {
        throw std::invalid_argument(
            "Username already exists."
        );
    }

    const std::string passwordHash =
        PasswordHasher::hashPassword(password);

    auto passenger =
        std::make_shared<Passenger>(
            nextUserId++,
            username,
            passwordHash,
            fullName,
            email,
            phone,
            passportNumber,
            0
        );

    users.push_back(passenger);

    return passenger;
}

void UserManagementService::deactivateUser(int userId)
{
    auto user = findUserById(userId);

    if (!user)
    {
        throw std::runtime_error(
            "User not found."
        );
    }

    user->setIsActive(false);
}

void UserManagementService::activateUser(int userId)
{
    auto user = findUserById(userId);

    if (!user)
    {
        throw std::runtime_error(
            "User not found."
        );
    }

    user->setIsActive(true);
}

std::shared_ptr<User> UserManagementService::findUserById(
    int userId
) const
{
    if (userId <= 0)
    {
        throw std::invalid_argument(
            "User ID must be positive."
        );
    }

    for (const auto& user : users)
    {
        if (user && user->getId() == userId)
        {
            return user;
        }
    }

    return nullptr;
}

std::shared_ptr<User> UserManagementService::findUserByUsername(
    const std::string& username
) const
{
    if (username.empty())
    {
        return nullptr;
    }

    for (const auto& user : users)
    {
        if (
            user &&
            user->getUsername() == username
        )
        {
            return user;
        }
    }

    return nullptr;
}

const std::vector<std::shared_ptr<User>>&
UserManagementService::getAllUsers() const
{
    return users;
}