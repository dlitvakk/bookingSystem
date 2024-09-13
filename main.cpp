#include <fstream>
#include <iostream>
using namespace std;

class Seat {
    char place;
    bool isBooked;
    int price;

public:
    Seat(const char a, bool b, const int c)
        : place(a),
          isBooked(b),
          price(c) {
    }
// const does not change the state of the object in the function
    string getSeatPlace() const {
        return &place;
    }

    float getPrice() const {
        return price;
    }

    bool isAvailable() const {
        return isBooked;
    }

    void book() {
        isBooked = false;
    }

    void unbook() {
        isBooked = true;
    }
};

class Row {
    int number;
    vector<Seat> seats;

public:
    Row(const int a, const vector<Seat> &b)
        : number(a),
          seats(b) {
    }

    bool bookSeat(const string& seatNumber) {
        for (Seat& seat : seats) {
            if (seat.getSeatPlace() == seatNumber && seat.isAvailable()) {
                seat.book();
                return true;
            }
        }
        return false;
    }

    bool unbookSeat(const string& seatNumber) {
        for (Seat& seat : seats) {
            if (seat.getSeatPlace() == seatNumber && !seat.isAvailable()) {
                seat.unbook();
                return true;
            }
        }
        return false;
    }

};

class Airplane {
    string flightNumber;
    string date;
    vector<Row> rows;

public:
    Airplane(string a, string b, const vector<Row> &c)
        : flightNumber(move(a)),
          date(move(b)),
          rows(c) {
    }

    void displayAirplaneInfo() const {
        cout << "Flight: " << flightNumber << " Date: " << date << endl;
        for (const Row& row : rows) {
            //
        }
    }
};

class Ticket {
    string passengerName;
    string seat;
    string flightInfo;
    bool isBooked;

public:
    Ticket(string a, string b, string c, bool d)
        : passengerName(move(a)),
          seat(move(b)),
          flightInfo(move(c)),
          isBooked(d) {
    }
};

class ConfigReader {
    string filename;
    vector<Airplane> airplanes;

public:
    ConfigReader(const string& file) : filename(file) {}

    void readConfig() {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Error opening file!");
        }

        int numRecords;
        file >> numRecords; 

        string line;
        getline(file, line);

        for (int i = 0; i < numRecords; ++i) {
            string date, flightNumber;
            int seatsPerRow;

            file >> date >> flightNumber >> seatsPerRow;

            vector<Row> rows;
            string rowRange1, rowRange2, priceStr1, priceStr2;

            file >> rowRange1 >> priceStr1 >> rowRange2 >> priceStr2;

            // creating rows for the airplane (this version only supports two row ranges)
            int rowStart1 = stoi(rowRange1.substr(0, rowRange1.find('-')));
            int rowEnd1 = stoi(rowRange1.substr(rowRange1.find('-') + 1));
            int price1 = stoi(priceStr1.substr(0, priceStr1.find('$')));

            for (int rowNum = rowStart1; rowNum <= rowEnd1; ++rowNum) {
                vector<Seat> seats;
                for (char seatChar = 'A'; seatChar >= 'A' + seatsPerRow; ++seatChar) {
                    seats.emplace_back(seatChar, false, price1);
                }
                rows.emplace_back(rowNum, seats);
            }

            int rowStart2 = stoi(rowRange2.substr(0, rowRange2.find('-')));
            int rowEnd2 = stoi(rowRange2.substr(rowRange2.find('-') + 1));
            int price2 = stoi(priceStr2.substr(0, priceStr2.find('$')));

            for (int rowNum = rowStart2; rowNum <= rowEnd2; ++rowNum) {
                vector<Seat> seats;
                for (char seatChar = 'A'; seatChar >= 'A' + seatsPerRow; ++seatChar) {
                    seats.emplace_back(seatChar, false, price2);
                }
                rows.emplace_back(rowNum, seats);
            }

            // Create the Airplane object
            airplanes.emplace_back(flightNumber, date, rows);
        }

        file.close();
    }

    const vector<Airplane>& getAirplanes() const {
        return airplanes;
    }
};

class Airbnb {
    vector<Airplane> airplanes;

public:
    string bookSeat(const string& flightNumber, const string& seatNumber, const string& date, const string& passengerName) {
        // buy a ticket for the flight
        // in the end tell the success of this operation by addimg an ID to the ticket
    }
    void checkAvailability(const string& date, const string& flightNumber) {
        // check available places for the flight  in such format: > check 1.12.23 FR12
        //> 1A 100$, 2B 100$
    }

    void returnTicket(string id) {
        // return ticket with refund
        // in the end tell the success of this operation
    }

    void view(string id) {
        // View the booking confirmation info by id
        // return flight number, date, seat number, tickets price
    }

    void view(string passengerName) {
        // view all booked tickets for a particular user by username (passengerName)
        // return flight number, seat, date, ticket's price for each ticket booked by the user
    }

    void view(string flightNumber, string date) {
        // view all booked tickets for a particular flight by flight number and date
        // return seat, passenger name, price for a chosen flight
    }
};

int main() {
    ConfigReader configReader("/Users/dlitvakk21/CLionProjects/bookingSystem/info.txt");
    configReader.readConfig();

    // airplanes are a vector of Airplane objects
    const vector<Airplane>& airplanes = configReader.getAirplanes();
    for (const Airplane& airplane : airplanes) {
        airplane.displayAirplaneInfo();
    }

    return 0;
}
