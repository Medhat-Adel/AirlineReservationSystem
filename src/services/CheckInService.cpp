#include "services/CheckInService.h"

#include <stdexcept>

CheckInService::CheckInService()
    : nextCheckInId(1),
      nextBoardingPassId(1)
{
}

std::shared_ptr<CheckIn>
CheckInService::onlineCheckIn(
    const std::shared_ptr<Reservation>& reservation,
    const std::string& checkInDateTime,
    const std::string& gate,
    const std::string& boardingTime
)
{
    if (reservation == nullptr)
    {
        throw std::invalid_argument(
            "Reservation cannot be null."
        );
    }

    if (reservation->getStatus() ==
        ReservationStatus::Cancelled)
    {
        throw std::invalid_argument(
            "Cannot check in a cancelled reservation."
        );
    }

    if (getCheckInByReservationId(
            reservation->getId()) != nullptr)
    {
        throw std::invalid_argument(
            "Reservation is already checked in."
        );
    }

    if (reservation->getFlight() == nullptr)
    {
        throw std::invalid_argument(
            "Reservation flight cannot be null."
        );
    }

    auto checkIn = std::make_shared<CheckIn>(
        nextCheckInId++,
        reservation->getId(),
        CheckInType::Online,
        checkInDateTime
    );

    const auto& passenger =
        reservation->getPassenger();

    const auto& flight =
        reservation->getFlight();

    const int boardingPassId =
        nextBoardingPassId++;

    auto boardingPass =
        std::make_shared<BoardingPass>(
            boardingPassId,
            "BP" + std::to_string(boardingPassId),
            reservation->getId(),
            passenger->getFullName(),
            passenger->getPassportNumber(),
            flight->getFlightNumber(),
            reservation->getSeatNumber(),
            gate,
            boardingTime
        );

    checkIns.push_back(checkIn);
    boardingPasses.push_back(boardingPass);

    return checkIn;
}

std::shared_ptr<CheckIn>
CheckInService::airportCheckIn(
    const std::shared_ptr<Reservation>& reservation,
    const std::string& checkInDateTime,
    const std::string& gate,
    const std::string& boardingTime
)
{
    if (reservation == nullptr)
    {
        throw std::invalid_argument(
            "Reservation cannot be null."
        );
    }

    if (reservation->getStatus() ==
        ReservationStatus::Cancelled)
    {
        throw std::invalid_argument(
            "Cannot check in a cancelled reservation."
        );
    }

    if (getCheckInByReservationId(
            reservation->getId()) != nullptr)
    {
        throw std::invalid_argument(
            "Reservation is already checked in."
        );
    }

    if (reservation->getFlight() == nullptr)
    {
        throw std::invalid_argument(
            "Reservation flight cannot be null."
        );
    }

    auto checkIn = std::make_shared<CheckIn>(
        nextCheckInId++,
        reservation->getId(),
        CheckInType::Airport,
        checkInDateTime
    );

    const auto& passenger =
        reservation->getPassenger();

    const auto& flight =
        reservation->getFlight();

    const int boardingPassId =
        nextBoardingPassId++;

    auto boardingPass =
        std::make_shared<BoardingPass>(
            boardingPassId,
            "BP" + std::to_string(boardingPassId),
            reservation->getId(),
            passenger->getFullName(),
            passenger->getPassportNumber(),
            flight->getFlightNumber(),
            reservation->getSeatNumber(),
            gate,
            boardingTime
        );

    checkIns.push_back(checkIn);
    boardingPasses.push_back(boardingPass);

    return checkIn;
}

std::shared_ptr<CheckIn>
CheckInService::getCheckInByReservationId(
    int reservationId
) const
{
    for (const auto& checkIn : checkIns)
    {
        if (checkIn->getReservationId() ==
            reservationId)
        {
            return checkIn;
        }
    }

    return nullptr;
}

std::shared_ptr<BoardingPass>
CheckInService::getBoardingPassByReservationId(
    int reservationId
) const
{
    for (const auto& boardingPass : boardingPasses)
    {
        if (boardingPass->getReservationId() ==
            reservationId)
        {
            return boardingPass;
        }
    }

    return nullptr;
}

const std::vector<std::shared_ptr<CheckIn>>&
CheckInService::getAllCheckIns() const
{
    return checkIns;
}

const std::vector<std::shared_ptr<BoardingPass>>&
CheckInService::getAllBoardingPasses() const
{
    return boardingPasses;
}