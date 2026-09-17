cmake_minimum_required(VERSION 3.20)

project(AirlineReservationSystem
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(AirlineReservationSystem
    main.cpp

    src/models/User.cpp
    src/models/Administrator.cpp
    src/models/BookingAgent.cpp
    src/models/Passenger.cpp

    src/models/Aircraft.cpp
    src/models/Flight.cpp

    src/models/CrewMember.cpp
    src/models/Pilot.cpp
    src/models/FlightAttendant.cpp

    src/models/Reservation.cpp
    src/services/BookingService.cpp
    src/models/Payment.cpp
    src/services/PaymentService.cpp
    src/services/FlightSearchService.cpp
    src/services/PassengerService.cpp

    src/models/CheckIn.cpp
    src/models/BoardingPass.cpp
    src/services/CheckInService.cpp
    src/services/FlightOperationsService.cpp
    src/models/Maintenance.cpp
    src/services/MaintenanceService.cpp
    src/services/ReportService.cpp

    src/repositories/AircraftRepository.cpp
    src/repositories/FlightRepository.cpp
    src/repositories/CrewRepository.cpp
    src/repositories/UserRepository.cpp
    src/repositories/PaymentRepository.cpp
    src/repositories/ReservationRepository.cpp
    src/repositories/MaintenanceRepository.cpp
)

target_include_directories(AirlineReservationSystem
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)