CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

TARGET = AirlineReservationSystem.exe

TEST_DIR = build/tests

CORE_SOURCES = src/models/User.cpp src/models/Administrator.cpp src/models/BookingAgent.cpp src/models/Passenger.cpp src/models/Aircraft.cpp src/models/CrewMember.cpp src/models/Pilot.cpp src/models/FlightAttendant.cpp src/models/Flight.cpp src/models/Reservation.cpp src/models/Payment.cpp src/models/CheckIn.cpp src/models/BoardingPass.cpp src/models/Maintenance.cpp src/services/PaymentService.cpp src/services/BookingService.cpp src/services/LoyaltyService.cpp src/services/FlightSearchService.cpp src/services/PassengerService.cpp src/services/CheckInService.cpp src/services/FlightOperationsService.cpp src/services/MaintenanceService.cpp src/services/ReportService.cpp src/services/AuthenticationService.cpp src/services/UserManagementService.cpp src/repositories/AircraftRepository.cpp src/repositories/FlightRepository.cpp src/repositories/CrewRepository.cpp src/repositories/UserRepository.cpp src/repositories/PaymentRepository.cpp src/repositories/ReservationRepository.cpp src/repositories/MaintenanceRepository.cpp src/utils/PasswordHasher.cpp src/ui/ConsoleUI.cpp


AIRCRAFT_TEST = $(TEST_DIR)/AircraftRepositoryTest.exe

FLIGHT_TEST = $(TEST_DIR)/FlightRepositoryTest.exe

CREW_TEST = $(TEST_DIR)/CrewRepositoryTest.exe

USER_TEST = $(TEST_DIR)/UserRepositoryTest.exe

PAYMENT_TEST = $(TEST_DIR)/PaymentRepositoryTest.exe

RESERVATION_TEST = $(TEST_DIR)/ReservationRepositoryTest.exe

MAINTENANCE_TEST = $(TEST_DIR)/MaintenanceRepositoryTest.exe

AUTHENTICATION_TEST = $(TEST_DIR)/AuthenticationServiceTest.exe

PASSWORD_HASHER_TEST = $(TEST_DIR)/PasswordHasherTest.exe

USER_MANAGEMENT_TEST = $(TEST_DIR)/UserManagementServiceTest.exe

USER_PERSISTENCE_INTEGRATION_TEST = $(TEST_DIR)/UserPersistenceIntegrationTest.exe

BOOKING_LIFECYCLE_INTEGRATION_TEST = $(TEST_DIR)/BookingLifecycleIntegrationTest.exe

build:
	$(CXX) $(CXXFLAGS) main.cpp $(CORE_SOURCES) -o $(TARGET)


run: build
	./$(TARGET)


$(AIRCRAFT_TEST):
	mkdir -p $(TEST_DIR)
	$(CXX) $(CXXFLAGS) tests/repositories/AircraftRepositoryTest.cpp $(CORE_SOURCES) -o $(AIRCRAFT_TEST)


$(FLIGHT_TEST):
	mkdir -p $(TEST_DIR)
	$(CXX) $(CXXFLAGS) tests/repositories/FlightRepositoryTest.cpp $(CORE_SOURCES) -o $(FLIGHT_TEST)


$(CREW_TEST):
	mkdir -p $(TEST_DIR)
	$(CXX) $(CXXFLAGS) tests/repositories/CrewRepositoryTest.cpp $(CORE_SOURCES) -o $(CREW_TEST)


$(USER_TEST):
	mkdir -p $(TEST_DIR)
	$(CXX) $(CXXFLAGS) tests/repositories/UserRepositoryTest.cpp $(CORE_SOURCES) -o $(USER_TEST)


$(PAYMENT_TEST):
	mkdir -p $(TEST_DIR)
	$(CXX) $(CXXFLAGS) tests/repositories/PaymentRepositoryTest.cpp $(CORE_SOURCES) -o $(PAYMENT_TEST)


$(RESERVATION_TEST):
	mkdir -p $(TEST_DIR)
	$(CXX) $(CXXFLAGS) tests/repositories/ReservationRepositoryTest.cpp $(CORE_SOURCES) -o $(RESERVATION_TEST)


$(MAINTENANCE_TEST):
	mkdir -p $(TEST_DIR)
	$(CXX) $(CXXFLAGS) tests/repositories/MaintenanceRepositoryTest.cpp $(CORE_SOURCES) -o $(MAINTENANCE_TEST)


$(AUTHENTICATION_TEST):
	mkdir -p $(TEST_DIR)
	$(CXX) $(CXXFLAGS) tests/services/AuthenticationServiceTest.cpp $(CORE_SOURCES) -o $(AUTHENTICATION_TEST)


$(PASSWORD_HASHER_TEST):
	mkdir -p $(TEST_DIR)
	$(CXX) $(CXXFLAGS) tests/services/PasswordHasherTest.cpp $(CORE_SOURCES) -o $(PASSWORD_HASHER_TEST)


$(USER_MANAGEMENT_TEST):
	mkdir -p $(TEST_DIR)
	$(CXX) $(CXXFLAGS) tests/services/UserManagementServiceTest.cpp $(CORE_SOURCES) -o $(USER_MANAGEMENT_TEST)

$(USER_PERSISTENCE_INTEGRATION_TEST):
	mkdir -p $(TEST_DIR)
	$(CXX) $(CXXFLAGS) tests/integration/UserPersistenceIntegrationTest.cpp $(CORE_SOURCES) -o $(USER_PERSISTENCE_INTEGRATION_TEST)

$(BOOKING_LIFECYCLE_INTEGRATION_TEST):
	mkdir -p $(TEST_DIR)
	$(CXX) $(CXXFLAGS) tests/integration/BookingLifecycleIntegrationTest.cpp $(CORE_SOURCES) -o $(BOOKING_LIFECYCLE_INTEGRATION_TEST)
	
test: $(AIRCRAFT_TEST) $(FLIGHT_TEST) $(CREW_TEST) $(USER_TEST) $(PAYMENT_TEST) $(RESERVATION_TEST) $(MAINTENANCE_TEST) $(AUTHENTICATION_TEST) $(PASSWORD_HASHER_TEST) $(USER_MANAGEMENT_TEST) $(USER_PERSISTENCE_INTEGRATION_TEST) $(BOOKING_LIFECYCLE_INTEGRATION_TEST)
	./$(AIRCRAFT_TEST)
	./$(FLIGHT_TEST)
	./$(CREW_TEST)
	./$(USER_TEST)
	./$(PAYMENT_TEST)
	./$(RESERVATION_TEST)
	./$(MAINTENANCE_TEST)
	./$(AUTHENTICATION_TEST)
	./$(PASSWORD_HASHER_TEST)
	./$(USER_MANAGEMENT_TEST)
	./$(USER_PERSISTENCE_INTEGRATION_TEST)
	./$(BOOKING_LIFECYCLE_INTEGRATION_TEST)
	echo ALL TESTS COMPLETED SUCCESSFULLY


clean:
	rm -f $(TARGET)
	rm -rf build