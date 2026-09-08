#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "models/User.h"

class Administrator : public User
{
public:
    Administrator(
        int id,
        const std::string& username,
        const std::string& passwordHash,
        const std::string& fullName,
        const std::string& email,
        const std::string& phone,
        bool isActive = true
    );

    void displayMenu() const override;
};

#endif