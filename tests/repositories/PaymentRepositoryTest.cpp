#include <iostream>
#include <memory>
#include <vector>

#include "models/Payment.h"
#include "repositories/PaymentRepository.h"

int main()
{
    try
    {
        std::vector<std::shared_ptr<Payment>> payments;

        payments.push_back(
            std::make_shared<Payment>(
                1,
                101,
                5000.0,
                PaymentMethod::Card,
                "2026-09-10 09:30",
                PaymentStatus::Completed
            )
        );

        payments.push_back(
            std::make_shared<Payment>(
                2,
                102,
                3500.0,
                PaymentMethod::Cash,
                "2026-09-10 10:00",
                PaymentStatus::Refunded
            )
        );

        payments.push_back(
            std::make_shared<Payment>(
                3,
                103,
                7000.0,
                PaymentMethod::BankTransfer,
                "2026-09-10 11:00",
                PaymentStatus::Pending
            )
        );

        PaymentRepository repository;

        std::cout << "Saving payments...\n";

        repository.save(payments);

        std::cout
            << "Payments saved successfully.\n\n";

        payments.clear();

        std::cout
            << "Memory cleared.\n"
            << "Payments in memory: "
            << payments.size()
            << "\n\n";

        payments = repository.load();

        std::cout
            << "Payments loaded: "
            << payments.size()
            << "\n\n";

        for (const auto& payment : payments)
        {
            std::cout
                << "-----------------------------\n";

            std::cout
                << "ID: "
                << payment->getId()
                << "\n";

            std::cout
                << "Reservation ID: "
                << payment->getReservationId()
                << "\n";

            std::cout
                << "Amount: "
                << payment->getAmount()
                << "\n";

            std::cout << "Method: ";

            switch (payment->getMethod())
            {
                case PaymentMethod::Cash:
                    std::cout << "Cash\n";
                    break;

                case PaymentMethod::Card:
                    std::cout << "Card\n";
                    break;

                case PaymentMethod::BankTransfer:
                    std::cout
                        << "Bank Transfer\n";
                    break;
            }

            std::cout << "Status: ";

            switch (payment->getStatus())
            {
                case PaymentStatus::Pending:
                    std::cout << "Pending\n";
                    break;

                case PaymentStatus::Completed:
                    std::cout << "Completed\n";
                    break;

                case PaymentStatus::Failed:
                    std::cout << "Failed\n";
                    break;

                case PaymentStatus::Refunded:
                    std::cout << "Refunded\n";
                    break;
            }

            std::cout
                << "Transaction Date: "
                << payment->getTransactionDate()
                << "\n";

            std::cout
                << "-----------------------------\n\n";
        }

        if (payments.size() != 3)
        {
            throw std::runtime_error(
                "Payment persistence failed."
            );
        }

        std::cout
            << "Payment persistence test "
            << "completed successfully!\n";
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "Test failed: "
            << e.what()
            << "\n";

        return 1;
    }

    return 0;
}