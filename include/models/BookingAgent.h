#ifndef BOOKING_AGENT_H
#define BOOKING_AGENT_H

#include "models/User.h"

class BookingAgent : public User
{
public:
    BookingAgent(
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