#ifndef AUTHENTICATION_SERVICE_H
#define AUTHENTICATION_SERVICE_H

#include <memory>
#include <string>
#include <vector>

#include "models/User.h"

class AuthenticationService
{
private:
    const std::vector<std::shared_ptr<User>>& users;

public:
    explicit AuthenticationService(
        const std::vector<std::shared_ptr<User>>& users
    );

    std::shared_ptr<User> login(
        const std::string& username,
        const std::string& password
    ) const;
};

#endif