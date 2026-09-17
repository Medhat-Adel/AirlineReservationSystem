#include <iostream>
#include <stdexcept>
#include <string>

#include "utils/PasswordHasher.h"

int main()
{
    const std::string password =
        "MySecurePassword123";

    const std::string storedHash =
        PasswordHasher::hashPassword(password);

    if (storedHash.empty())
    {
        throw std::runtime_error(
            "Password hash should not be empty."
        );
    }

    if (storedHash.find(':') == std::string::npos)
    {
        throw std::runtime_error(
            "Password hash must contain a salt separator."
        );
    }

    std::cout
        << "Password hashing test passed!\n";

    if (!PasswordHasher::verifyPassword(
            password,
            storedHash))
    {
        throw std::runtime_error(
            "Correct password verification failed."
        );
    }

    std::cout
        << "Correct password verification test passed!\n";

    if (PasswordHasher::verifyPassword(
            "WrongPassword",
            storedHash))
    {
        throw std::runtime_error(
            "Wrong password verification failed."
        );
    }

    std::cout
        << "Wrong password verification test passed!\n";

    const std::string secondHash =
        PasswordHasher::hashPassword(password);

    if (storedHash == secondHash)
    {
        throw std::runtime_error(
            "Two password hashes should use different salts."
        );
    }

    std::cout
        << "Unique salt test passed!\n";

    try
    {
        PasswordHasher::hashPassword("");

        throw std::runtime_error(
            "Empty password test failed."
        );
    }
    catch (const std::invalid_argument&)
    {
    }

    std::cout
        << "Empty password test passed!\n";

    std::cout << "\n";
    std::cout
        << "Password hasher test completed successfully!\n";

    return 0;
}