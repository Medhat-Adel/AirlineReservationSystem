#include "repositories/PaymentRepository.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace
{
    std::string paymentMethodToString(
        PaymentMethod method
    )
    {
        switch (method)
        {
            case PaymentMethod::Cash:
                return "Cash";

            case PaymentMethod::Card:
                return "Card";

            case PaymentMethod::BankTransfer:
                return "BankTransfer";
        }

        throw std::runtime_error(
            "Unknown payment method."
        );
    }

    PaymentMethod stringToPaymentMethod(
        const std::string& method
    )
    {
        if (method == "Cash")
        {
            return PaymentMethod::Cash;
        }

        if (method == "Card")
        {
            return PaymentMethod::Card;
        }

        if (method == "BankTransfer")
        {
            return PaymentMethod::BankTransfer;
        }

        throw std::runtime_error(
            "Unknown payment method: " + method
        );
    }

    std::string paymentStatusToString(
        PaymentStatus status
    )
    {
        switch (status)
        {
            case PaymentStatus::Pending:
                return "Pending";

            case PaymentStatus::Completed:
                return "Completed";

            case PaymentStatus::Failed:
                return "Failed";

            case PaymentStatus::Refunded:
                return "Refunded";
        }

        throw std::runtime_error(
            "Unknown payment status."
        );
    }

    PaymentStatus stringToPaymentStatus(
        const std::string& status
    )
    {
        if (status == "Pending")
        {
            return PaymentStatus::Pending;
        }

        if (status == "Completed")
        {
            return PaymentStatus::Completed;
        }

        if (status == "Failed")
        {
            return PaymentStatus::Failed;
        }

        if (status == "Refunded")
        {
            return PaymentStatus::Refunded;
        }

        throw std::runtime_error(
            "Unknown payment status: " + status
        );
    }
}

PaymentRepository::PaymentRepository(
    const std::string& filePath
)
    : filePath(filePath)
{
}

void PaymentRepository::save(
    const std::vector<std::shared_ptr<Payment>>& payments
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

    for (const auto& payment : payments)
    {
        if (!payment)
        {
            continue;
        }

        json item = {
            {"id", payment->getId()},
            {"reservationId", payment->getReservationId()},
            {"amount", payment->getAmount()},
            {"method",
             paymentMethodToString(
                 payment->getMethod()
             )},
            {"status",
             paymentStatusToString(
                 payment->getStatus()
             )},
            {"transactionDate",
             payment->getTransactionDate()}
        };

        data.push_back(item);
    }

    std::ofstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open payment file for writing: "
            + filePath
        );
    }

    file << data.dump(4);

    if (!file)
    {
        throw std::runtime_error(
            "Failed to write payment data."
        );
    }
}

std::vector<std::shared_ptr<Payment>>
PaymentRepository::load() const
{
    std::vector<std::shared_ptr<Payment>> payments;

    if (!std::filesystem::exists(filePath))
    {
        return payments;
    }

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Failed to open payment file for reading: "
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
            std::string("Invalid payment JSON file: ")
            + e.what()
        );
    }

    if (!data.is_array())
    {
        throw std::runtime_error(
            "Payment JSON root must be an array."
        );
    }

    for (const auto& item : data)
    {
        try
        {
            int id =
                item.at("id").get<int>();

            int reservationId =
                item.at("reservationId").get<int>();

            double amount =
                item.at("amount").get<double>();

            PaymentMethod method =
                stringToPaymentMethod(
                    item.at("method")
                        .get<std::string>()
                );

            PaymentStatus status =
                stringToPaymentStatus(
                    item.at("status")
                        .get<std::string>()
                );

            std::string transactionDate =
                item.at("transactionDate")
                    .get<std::string>();

            auto payment =
                std::make_shared<Payment>(
                    id,
                    reservationId,
                    amount,
                    method,
                    transactionDate,
                    status
                );

            payments.push_back(payment);
        }
        catch (const json::exception& e)
        {
            throw std::runtime_error(
                std::string("Invalid payment data: ")
                + e.what()
            );
        }
    }

    return payments;
}