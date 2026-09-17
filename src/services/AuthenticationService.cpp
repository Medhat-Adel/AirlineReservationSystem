#include "services/AuthenticationService.h"

#include <stdexcept>

#include "utils/PasswordHasher.h"

AuthenticationService::AuthenticationService(
    const std::vector<std::shared_ptr<User>>& users
)
    : users(users)
{
}

std::shared_ptr<User> AuthenticationService::login(
    const std::string& username,
    const std::string& password
) const
{
    if (username.empty())
    {
        throw std::invalid_argument(
            "Username cannot be empty."
        );
    }

    if (password.empty())
    {
        throw std::invalid_argument(
            "Password cannot be empty."
        );
    }

    for (const auto& user : users)
    {
        if (!user)
        {
            continue;
        }

        if (user->getUsername() == username)
        {
            if (!user->getIsActive())
            {
                throw std::runtime_error(
                    "User account is inactive."
                );
            }

            if (!PasswordHasher::verifyPassword(
                    password,
                    user->getPasswordHash()))
            {
                throw std::runtime_error(
                    "Invalid username or password."
                );
            }

            return user;
        }
    }

    throw std::runtime_error(
        "Invalid username or password."
    );
}