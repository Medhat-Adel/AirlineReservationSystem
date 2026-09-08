#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <set>
#include <memory>
#include <vector>

#include "models/Aircraft.h"
#include "models/CrewMember.h"

enum class FlightStatus
{
    Scheduled,
    Boarding,
    Departed,
    Delayed,
    Cancelled,
    Completed
};

class Flight
{
private:
    std::string flightNumber;
    std::string origin;
    std::string destination;
    std::string departureTime;
    std::string arrivalTime;
    double price;
    FlightStatus status;

    std::shared_ptr<Aircraft> aircraft;

    std::set<std::string> occupiedSeats;
    std::vector<std::shared_ptr<CrewMember>> crewMembers;
    int flightDurationHours;    

public:
    Flight(
        const std::string& flightNumber,
        const std::string& origin,
        const std::string& destination,
        const std::string& departureTime,
        const std::string& arrivalTime,
        double price,
        const std::shared_ptr<Aircraft>& aircraft,
        int flightDurationHours,
        FlightStatus status = FlightStatus::Scheduled
    );

    // Getters
    const std::string& getFlightNumber() const;
    const std::string& getOrigin() const;
    const std::string& getDestination() const;
    const std::string& getDepartureTime() const;
    const std::string& getArrivalTime() const;
    double getPrice() const;
    FlightStatus getStatus() const;
    std::shared_ptr<Aircraft> getAircraft() const;

    // Setters
    void setPrice(double price);
    void setStatus(FlightStatus status);
    void setAircraft(const std::shared_ptr<Aircraft>& aircraft);

    // Seat management
    bool reserveSeat(const std::string& seatNumber);
    bool releaseSeat(const std::string& seatNumber);
    bool isSeatAvailable(const std::string& seatNumber) const;

    const std::set<std::string>& getOccupiedSeats() const;

    int getAvailableSeats() const;

    bool assignCrewMember(
        const std::shared_ptr<CrewMember>& crewMember
    );

    bool removeCrewMember(int crewMemberId);

    const std::vector<std::shared_ptr<CrewMember>>&
    getCrewMembers() const;

    int getFlightDurationHours() const;
};

#endif