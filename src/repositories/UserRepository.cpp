#include "repositories/UserRepository.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "models/Administrator.h"
#include "models/BookingAgent.h"
#include "models/Passenger.h"

using json = nlohmann::json;

namespace
{
    std::string roleToString(
        const std::shared_ptr<User>& user
    )
    {
        if (!user)
        {
            throw std::runtime_error(
                "Cannot determine role of null user."
            );
        }

        switch (user->getRole())
        {
            case Role::Administrator:
                return "Administrator";

            case Role::BookingAgent:
                return "BookingAgent";

            case Role::Passenger:
                return "Passenger";
        }

        throw std::runtime_error(
            "Unknown user role."
        );
    }
}

UserRepository::UserRepository(
    const std::string& filePath
)
    : filePath(filePath)
{
}

void UserRepository::save(
    const std::vector<std::shared_ptr<User>>& users
) const
{
    std::filesystem::path path(filePath);

    if (!path.parent_path().empty())
    {
        std::filesystem::create_directories(
            path.parent_path()
        );
    }

    json data = json::array();

    for (const auto& user : users)
    {
        if (!user)
        {
            continue;
        }

        json item = {
            {"id", user->getId()},
            {"username", user->getUsername()},
            {"passwordHash", user->getPasswordHash()},
            {"fullName", user->getFullName()},
            {"email", user->getEmail()},
            {"phone", user->getPhone()},
            {"role", roleToString(user)},
            {"isActive", user->getIsActive()}
        };

        auto passenger =
            std::dynamic_pointer_cast<Passenger>(user);

        if (passenger)
        {
            item["passportNumber"] =
                passenger->getPassportNumber();

            item["loyaltyPoints"] =
                passenger->getLoyaltyPoints();

            item["preferredSeat"] =
                passenger->getPreferredSeat();

            item["mealPreference"] =
                passenger->getMealPreference();
        }

        data.push_back(item);
    }

    std::ofstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open user file for writing: "
            + filePath
        );
    }

    file << data.dump(4);

    if (!file)
    {
        throw std::runtime_error(
            "Failed to write user data."
        );
    }
}

std::vector<std::shared_ptr<User>>
UserRepository::load() const
{
    std::vector<std::shared_ptr<User>> users;

    if (!std::filesystem::exists(filePath))
    {
        return users;
    }

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open user file for reading: "
            + filePath
        );
    }

    json data;

    try
    {
        file >> data;
    }
    catch (const json::parse_error& e)
    {
        throw std::runtime_error(
            std::string("Invalid user JSON file: ")
            + e.what()
        );
    }

    if (!data.is_array())
    {
        throw std::runtime_error(
            "User JSON root must be an array."
        );
    }

    for (const auto& item : data)
    {
        try
        {
            int id =
                item.at("id").get<int>();

            std::string username =
                item.at("username").get<std::string>();

            std::string passwordHash =
                item.at("passwordHash").get<std::string>();

            std::string fullName =
                item.at("fullName").get<std::string>();

            std::string email =
                item.at("email").get<std::string>();

            std::string phone =
                item.at("phone").get<std::string>();

            std::string role =
                item.at("role").get<std::string>();

            bool isActive =
                item.at("isActive").get<bool>();

            std::shared_ptr<User> user;

            if (role == "Administrator")
            {
                user = std::make_shared<Administrator>(
                    id,
                    username,
                    passwordHash,
                    fullName,
                    email,
                    phone,
                    isActive
                );
            }
            else if (role == "BookingAgent")
            {
                user = std::make_shared<BookingAgent>(
                    id,
                    username,
                    passwordHash,
                    fullName,
                    email,
                    phone,
                    isActive
                );
            }
            else if (role == "Passenger")
            {
                std::string passportNumber =
                    item.at("passportNumber")
                        .get<std::string>();

                int loyaltyPoints =
                    item.at("loyaltyPoints")
                        .get<int>();

                auto passenger =
                    std::make_shared<Passenger>(
                        id,
                        username,
                        passwordHash,
                        fullName,
                        email,
                        phone,
                        passportNumber,
                        loyaltyPoints,
                        isActive
                    );

                    if (item.contains("preferredSeat"))
                    {
                        const std::string preferredSeat =
                            item.at("preferredSeat").get<std::string>();

                        if (!preferredSeat.empty())
                        {
                            passenger->setPreferredSeat(
                                preferredSeat
                            );
                        }
                    }

                    if (item.contains("mealPreference"))
                    {
                        const std::string mealPreference =
                            item.at("mealPreference").get<std::string>();

                        if (!mealPreference.empty())
                        {
                            passenger->setMealPreference(
                                mealPreference
                            );
                        }
                    }

                user = passenger;
            }
            else
            {
                throw std::runtime_error(
                    "Unknown user role: " + role
                );
            }

            users.push_back(user);
        }
        catch (const json::exception& e)
        {
            throw std::runtime_error(
                std::string("Invalid user data: ")
                + e.what()
            );
        }
    }

    return users;
}