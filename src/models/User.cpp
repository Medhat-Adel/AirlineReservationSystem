#include "models/User.h"

User::User(
    int id,
    const std::string& username,
    const std::string& passwordHash,
    const std::string& fullName,
    const std::string& email,
    const std::string& phone,
    Role role,
    bool isActive
)
    : id(id),
      username(username),
      passwordHash(passwordHash),
      fullName(fullName),
      email(email),
      phone(phone),
      role(role),
      isActive(isActive)
{
}

// Getters

int User::getId() const
{
    return id;
}

const std::string& User::getUsername() const
{
    return username;
}

const std::string& User::getPasswordHash() const
{
    return passwordHash;
}

const std::string& User::getFullName() const
{
    return fullName;
}

const std::string& User::getEmail() const
{
    return email;
}

const std::string& User::getPhone() const
{
    return phone;
}

Role User::getRole() const
{
    return role;
}

bool User::getIsActive() const
{
    return isActive;
}

// Setters

void User::setFullName(const std::string& fullName)
{
    this->fullName = fullName;
}

void User::setEmail(const std::string& email)
{
    this->email = email;
}

void User::setPhone(const std::string& phone)
{
    this->phone = phone;
}

void User::setIsActive(bool isActive)
{
    this->isActive = isActive;
}