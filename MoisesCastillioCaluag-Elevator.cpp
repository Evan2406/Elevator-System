#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>
#include <chrono>
#include <cmath>
using namespace std;

mutex m1, m2;

struct Passenger {
    int id;
    int currentFloor;
    int destinationFloor;
};

void clearScreen() {
    cout << string(100, '\n');
}

void displayRealTimeStatus(const vector<Passenger>& passengers, int currentFloor) {
    cout << "\nPassengers status:\n";
    for (const auto& passenger : passengers) {
        if (passenger.currentFloor == currentFloor) {
            cout << "Passenger " << passenger.id << ": Boarding at floor " << passenger.currentFloor << "\n";
            this_thread::sleep_for(chrono::seconds(5));
        } else {
            cout << "Passenger " << passenger.id << ": Waiting at floor " << passenger.currentFloor << "\n";
        }
    }
    this_thread::sleep_for(chrono::seconds(2));
}

void displayFinalSequence(const vector<Passenger>& passengers) {
    cout << "\nFinal Sequence:\n";
    cout << "Sequence of boarding:\n";
    for (size_t i = 0; i < passengers.size(); i++) {
        cout << i + 1 << ". Passenger " << passengers[i].id << " boarded at floor " << passengers[i].currentFloor << "\n";
        this_thread::sleep_for(chrono::seconds(2));
    }

    cout << "\nSequence of exiting:\n";
    for (size_t i = 0; i < passengers.size(); i++) {
        cout << i + 1 << ". Passenger " << passengers[i].id << " exited at floor " << passengers[i].destinationFloor << "\n";
        this_thread::sleep_for(chrono::seconds(2));
    }
}

void analyzeElevator(int currentFloor, vector<Passenger>& passengers) {
    m1.lock();
    sort(passengers.begin(), passengers.end(), [currentFloor](Passenger a, Passenger b) {
        return abs(currentFloor - a.currentFloor) < abs(currentFloor - b.currentFloor);
    });

    clearScreen();
    displayRealTimeStatus(passengers, currentFloor);

    m2.lock();
    sort(passengers.begin(), passengers.end(), [](Passenger a, Passenger b) {
        return a.destinationFloor < b.destinationFloor;
    });

    clearScreen();
    displayFinalSequence(passengers);

    m2.unlock();
    m1.unlock();
}

int getValidFloor(const string& prompt) {
    int floor;
    while (true) {
        cout << prompt;
        if (cin >> floor && floor >= 1 && floor <= 9) {
            break;
        } else {
            cout << "Invalid input! Please enter a number between 1 and 9.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return floor;
}

int getPassengerCount() {
    int count;
    while (true) {
        cout << "Enter the number of passengers (1-7): ";
        if (cin >> count && count >= 1 && count <= 7) {
            break;
        } else {
            cout << "Invalid input! Please enter a number between 1 and 7.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return count;
}

int main() {
    int currentFloor = getValidFloor("What floor is the elevator on now (1-9)? ");

    int passengerCount = getPassengerCount();
    vector<Passenger> passengers(passengerCount);

    for (int i = 0; i < passengerCount; i++) {
        passengers[i].id = i + 1;
        cout << "\nPassenger " << passengers[i].id << ":\n";
        passengers[i].currentFloor = getValidFloor("What floor are you on now (1-9)? ");
        passengers[i].destinationFloor = getValidFloor("What floor do you want to go to (1-9)? ");
    }

    thread t1(analyzeElevator, currentFloor, ref(passengers));
    t1.join();

    cout << "Elevator system finished.\n";
    return 0;
}
