#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

#include <memory>
#include <string>
#include <vector>

#include "models/User.h"

class UserRepository
{
private:
    std::string filePath;

public:
    explicit UserRepository(
        const std::string& filePath = "data/users.json"
    );

    void save(
        const std::vector<std::shared_ptr<User>>& users
    ) const;

    std::vector<std::shared_ptr<User>> load() const;
};

#endif