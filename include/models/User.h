#ifndef USER_H
#define USER_H

#include <string>

enum class Role
{
    Administrator,
    BookingAgent,
    Passenger
};

class User
{
protected:
    int id;
    std::string username;
    std::string passwordHash;
    std::string fullName;
    std::string email;
    std::string phone;
    Role role;
    bool isActive;

public:
    User(
        int id,
        const std::string& username,
        const std::string& passwordHash,
        const std::string& fullName,
        const std::string& email,
        const std::string& phone,
        Role role,
        bool isActive = true
    );

    virtual ~User() = default;

    // Getters
    int getId() const;
    const std::string& getUsername() const;
    const std::string& getPasswordHash() const;
    const std::string& getFullName() const;
    const std::string& getEmail() const;
    const std::string& getPhone() const;
    Role getRole() const;
    bool getIsActive() const;

    // Setters
    void setFullName(const std::string& fullName);
    void setEmail(const std::string& email);
    void setPhone(const std::string& phone);
    void setIsActive(bool isActive);

    // Polymorphic function
    virtual void displayMenu() const = 0;
};

#endif