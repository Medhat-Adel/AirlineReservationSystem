#ifndef CHECK_IN_H
#define CHECK_IN_H

#include <string>

enum class CheckInType
{
    Online,
    Airport
};

enum class CheckInStatus
{
    Completed,
    Cancelled
};

class CheckIn
{
private:
    int id;
    int reservationId;
    CheckInType type;
    CheckInStatus status;
    std::string checkInDateTime;

public:
    CheckIn(
        int id,
        int reservationId,
        CheckInType type,
        const std::string& checkInDateTime,
        CheckInStatus status = CheckInStatus::Completed
    );

    int getId() const;
    int getReservationId() const;
    CheckInType getType() const;
    CheckInStatus getStatus() const;
    const std::string& getCheckInDateTime() const;

    void setStatus(CheckInStatus status);
};

#endif