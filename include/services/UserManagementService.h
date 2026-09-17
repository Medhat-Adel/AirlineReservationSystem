#ifndef USER_MANAGEMENT_SERVICE_H
#define USER_MANAGEMENT_SERVICE_H

#include <memory>
#include <string>
#include <vector>

#include "models/User.h"

class UserManagementService
{
private:
    std::vector<std::shared_ptr<User>>& users;
    int nextUserId;

public:
    explicit UserManagementService(
        std::vector<std::shared_ptr<User>>& users
    );

    std::shared_ptr<User> createAdministrator(
        const std::string& username,
        const std::string& password,
        const std::string& fullName,
        const std::string& email,
        const std::string& phone
    );

    std::shared_ptr<User> createBookingAgent(
        const std::string& username,
        const std::string& password,
        const std::string& fullName,
        const std::string& email,
        const std::string& phone
    );

    std::shared_ptr<User> createPassenger(
        const std::string& username,
        const std::string& password,
        const std::string& fullName,
        const std::string& email,
        const std::string& phone,
        const std::string& passportNumber
    );

    void deactivateUser(int userId);
    void activateUser(int userId);

    std::shared_ptr<User> findUserById(int userId) const;

    std::shared_ptr<User> findUserByUsername(
        const std::string& username
    ) const;

    const std::vector<std::shared_ptr<User>>& getAllUsers() const;
};

#endif