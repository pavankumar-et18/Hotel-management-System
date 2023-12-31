#include <iostream>
#include <vector>
#include <fstream>

const int TABLE_SIZE = 20;
const float SINGLE_BEDROOM_COST = 50.0;
const float DOUBLE_BEDROOM_COST = 75.0;

struct Booking {
    int bookingId;
    std::string customerName;
    std::string mobileNumber;
    int numSingleBedRooms;
    int numDoubleBedRooms;
    
    std::string status;
};

struct Food {
    int bookingId;
    float foodCosts;
    bool isValid;  // Flag indicating if the food order is valid or deleted
};

class HotelManagementSystem {
private:
    std::vector<Booking> bookings;
    Food foodOrders[20];

    int primaryHash(int bookingId) {
        return bookingId % TABLE_SIZE;
    }

    int secondaryHash(int bookingId) {
        return 7 - (bookingId % 7);
    }

    void loadBookings() {
        std::ifstream file("bookings.txt");
        if (!file) {
            std::cerr << "Error opening bookings.txt file.\n";
            return;
        }

        Booking booking;
        while (file >> booking.bookingId >> booking.customerName >> booking.mobileNumber
               >> booking.numSingleBedRooms >> booking.numDoubleBedRooms >> booking.status) {
            bookings.push_back(booking);
        }

        file.close();
    }

    void saveBookings() {
        std::ofstream file("bookings.txt");
        if (!file) {
            std::cerr << "Error opening bookings.txt file.\n";
            return;
        }

        for (size_t i = 0; i < bookings.size(); ++i) {
            const Booking& booking = bookings[i];
            file << booking.bookingId << " " << booking.customerName << " " << booking.mobileNumber << " "
                 << booking.numSingleBedRooms << " " << booking.numDoubleBedRooms << " " << booking.status << "\n";
        }

        file.close();
    }

    void loadFoodOrders() {
        std::ifstream file("foodOrders.txt");
        int i;
        if (!file) {
            std::cerr << "Error opening foodOrders.txt file.\n";
            return;
        }

        Food foodOrder;
        while (file >> i >> foodOrder.bookingId >> foodOrder.foodCosts >> foodOrder.isValid) {
            foodOrders[i]=foodOrder;
        }

        file.close();
    }

    void saveFoodOrders() {
        std::ofstream file("foodOrders.txt");
        if (!file) {
            std::cerr << "Error opening foodOrders.txt file.\n";
            return;
        }

        for (int i = 0; i < TABLE_SIZE; i++) {
            const Food& foodOrder = foodOrders[i];
            file <<i <<" " <<foodOrder.bookingId << " " << foodOrder.foodCosts << " " << foodOrder.isValid << "\n";
        }

        file.close();
    }

public:
    HotelManagementSystem() {
        for(int i=0;i<TABLE_SIZE;i++)
        {
        	foodOrders[i].bookingId=0;
        	foodOrders[i].foodCosts=0;
        	foodOrders[i].isValid=false;
		}
        loadBookings();
        loadFoodOrders();
    }

    ~HotelManagementSystem() {
        saveBookings();
        saveFoodOrders();
    }

    void run() {
        int choice;
        do {
            std::cout << "-----------------------------\n";
            std::cout << "Hotel Management System Menu:\n";
            std::cout << "-----------------------------\n";
            std::cout << "1. Add Booking\n";
            std::cout << "2. Delete Booking\n";
            std::cout << "3. Display Bookings\n";
            std::cout << "4. Order Food\n";
            std::cout << "5. Checkout\n";
            std::cout << "6. Exit\n";
            std::cout << "-----------------------------\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
                case 1:
                    addBooking();
                    break;
                case 2:
                    deleteBooking();
                    break;
                case 3:
                    displayBookings();
                    break;
                case 4:
                    orderFood();
                    break;
                case 5:
                    checkout();
                    break;
                case 6:
                    std::cout << "Exiting the system.\n";
                    return;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 6);
    }

    void addBooking() {
        Booking booking;
        std::cout << "Enter Booking Details:\n";
        std::cout << "Booking ID: ";
        std::cin >> booking.bookingId;
        std::cout << "Customer Name: ";
        std::cin.ignore();
        std::getline(std::cin, booking.customerName);
        std::cout << "Mobile Number: ";
        std::cin >> booking.mobileNumber;
        std::cout << "Number of Single Bed Rooms ($50 per day): ";
        std::cin >> booking.numSingleBedRooms;
        std::cout << "Number of Double Bed Rooms ($75 per day): ";
        std::cin >> booking.numDoubleBedRooms;
        booking.status = "in";
        bookings.push_back(booking);
        std::cout << "Booking added successfully!\n";
    }

    void deleteBooking() {
        int bookingId;
        std::cout << "Enter Booking ID to delete: ";
        std::cin >> bookingId;

        // Mark the corresponding food orders as deleted
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (foodOrders[i].bookingId == bookingId) {
                foodOrders[i].isValid = false;
            }
        }

        for (int i = 0; i < bookings.size(); ++i) {
            if (bookings[i].bookingId == bookingId) {
                bookings.erase(bookings.begin() + i);
                std::cout << "Booking deleted successfully!\n";
                return;
            }
        }

        std::cout << "Booking not found.\n";
    }

    void displayBookings() {
        if (bookings.empty()) {
            std::cout << "No bookings to display.\n";
        } else {
            std::cout << "Booking Details:\n";
            std::cout << "--------------------------------------------------------------------------------------------------\n";
            std::cout << "Booking ID\tCustomer Name\tMobile Number\tSingle Bed Rooms\tDouble Bed Rooms\tStatus\n";
            std::cout << "--------------------------------------------------------------------------------------------------\n";

            for (int i = 0; i < bookings.size(); ++i) {
                std::cout << bookings[i].bookingId << "\t\t" << bookings[i].customerName << "\t\t"
                          << bookings[i].mobileNumber << "\t\t" << bookings[i].numSingleBedRooms << "\t\t\t"
                          << bookings[i].numDoubleBedRooms << "\t\t\t" << bookings[i].status << "\n";
            }

            std::cout << "--------------------------------------------------------------------------------------------------\n";
        }
    }

void orderFood() {
    int bookingId;
    float foodCosts;
    std::cout << "Enter Booking ID: ";
    std::cin >> bookingId;

    for (int i = 0; i < bookings.size(); ++i) {
        if (bookings[i].bookingId == bookingId) {
            if (bookings[i].status == "out") {
                std::cout << "Booking is already checked out. Cannot place a food order.\n";
                return;
            }

            std::cout << "Enter Food Costs: $";
            std::cin >> foodCosts;

            // Insert food order using double hashing
            int index = primaryHash(bookingId);
            int stepSize = secondaryHash(bookingId);
            int collisions = 0;

            while (foodOrders[index].isValid && foodOrders[index].bookingId != bookingId) {
                index = (index + stepSize) % TABLE_SIZE;
                ++collisions;

                // If all slots are filled, break to avoid infinite loop
                if (collisions >= TABLE_SIZE) {
                    std::cout << "No available slot to place the food order.\n";
                    return;
                }
            }
            if(foodOrders[index].isValid==false){
            	foodOrders[index].foodCosts=0;
			}
            foodOrders[index].bookingId = bookingId;
            foodOrders[index].foodCosts += foodCosts;
            foodOrders[index].isValid = true;
            std::cout << "Food order placed successfully!\n";
            return; // Break the loop once the food order is placed
        }
    }

    std::cout << "Booking not found.\n";
    return;
}



   void checkout() {
        int bookingId,sdays,ddays;
        char choice[5];
        std::cout << "Enter Booking ID to checkout: ";
        std::cin >> bookingId;
        std::cout << "Number of Stay Days in Single bedrooms: ";
        std::cin >> sdays;
        std::cout << "Number of Stay Days in Double bedrooms: ";
        std::cin >> ddays;

        float totalCost = 0.0;

        for (size_t i = 0; i < bookings.size(); ++i) {
            if (bookings[i].bookingId == bookingId) {
                std::cout << "single bedrooms ($50 per day): 50X" << bookings[i].numSingleBedRooms <<"X"<<sdays <<"\n";
                std::cout << "double bedrooms ($75 per day): 75X" << bookings[i].numDoubleBedRooms <<"X"<<ddays <<"\n";
                std::cout << "Total food costs: " << getFoodCosts(bookingId) << "\n";
                totalCost = (bookings[i].numSingleBedRooms * SINGLE_BEDROOM_COST*sdays) +
                             (bookings[i].numDoubleBedRooms * DOUBLE_BEDROOM_COST*ddays) +
                            getFoodCosts(bookingId);
                std::cout << "Total Cost: $" << totalCost << "\n";
                std::cout<< "\nConfirm checkout (yes/no)? ";
                std::cin>>choice;
                if(choice!="yes"){
                	return;
				}
				bookings[i].status = "out";
				return;
            }
        }
        std::cout << "Booking not found.\n";
        return;
    }

    float getFoodCosts(int bookingId) {
        float foodCosts = 0.0;

        for (int i = 0; i<TABLE_SIZE; i++) {
            if (foodOrders[i].bookingId == bookingId) {
                foodCosts += foodOrders[i].foodCosts;
            }
        }

        return foodCosts;
    }
};

int main() {
    HotelManagementSystem hotel;
    hotel.run();
    return 0;
}
