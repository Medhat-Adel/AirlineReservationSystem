CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

TARGET = AirlineReservationSystem

SOURCES = main.cpp \
	src/models/User.cpp \
	src/models/Administrator.cpp \
	src/models/BookingAgent.cpp \
	src/models/Passenger.cpp \
	src/models/Aircraft.cpp \
	src/models/Flight.cpp \
	src/models/CrewMember.cpp \
	src/models/Pilot.cpp \
	src/models/FlightAttendant.cpp \
	src/models/Reservation.cpp \
	src/services/BookingService.cpp \
	src/models/Payment.cpp \
	src/services/PaymentService.cpp

build:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: build
	./$(TARGET)

clean:
	del /Q $(TARGET).exe 2>NUL || exit 0