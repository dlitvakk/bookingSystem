#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
using namespace std;

class Seat {
    string place;
    bool isBooked;
    int price;

public:
    Seat(const string a, bool b, const int c)
        : place(a),
          isBooked(b),
          price(c) {
    }
// const does not change the state of the object in the function
    string getSeatPlace() const {
        return place;
    }

    float getPrice() const {
        return price;
    }

    bool isAvailable() const {
        return !isBooked;
    }

    void book() {
        isBooked = true;
    }

    void unbook() {
        isBooked = false;
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

    const vector<Seat>& getSeats() const {
        return seats;
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

    string getFlightNumber() {
        return flightNumber;
    }

    string getDate() {
        return date;
    }

    int getPrice(const string& seatNumber) {
        for (Row& row : rows) {
            for (const Seat& seat : row.getSeats()) {
                if (seat.getSeatPlace() == seatNumber) {
                    return seat.getPrice();
                }
            }
        }
        return -1;
    }

    bool bookSeat(const string& seatNumber) {
        for (Row& row : rows) {
            if (row.bookSeat(seatNumber)) {
                return true;
            }
        }
        return false;  // Seat not available
    }

    bool unbookSeat(const string& seatNumber) {
        for (Row& row : rows) {
            if (row.unbookSeat(seatNumber)) {
                return true;
            }
        }
        return false;  // Seat not found
    }

    const vector<Row>& getRows() const {
        return rows;
    }
};

class Ticket {
    string passengerName;
    string seat;
    string flightInfo;
    bool isBooked;
    int price;
    string ticketID;

public:
    Ticket(const string& a, const string& b, const string& c, bool d, int e, string f)
        : passengerName(a), seat(b), flightInfo(c), isBooked(d), price(e), ticketID(f) {}

    string getPassengerName() const {
        return passengerName;
    }

    string getSeat() const {
        return seat;
    }

    string getFlightInfo() const {
        return flightInfo;
    }

    bool isTicketBooked() const {
        return isBooked;
    }

    int getPrice() const {
        return price;
    }

    void setPrice(int p) {
        price = p;
    }

    void setID(string id) {
        ticketID = id;
    }

    void unbookSeat(const string& seatNumber) {
        isBooked = false;
    }

    void displayTicket() const {
        cout << "Passenger: " << passengerName
             << "\nFlight Info: " << flightInfo
             << "\nSeat: " << seat
             << "\nPrice: " << price << "$" << endl;
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
                for (char seatChar = 'A'; seatChar < 'A' + seatsPerRow; ++seatChar) {
                    string seat;
                    seat = to_string(rowNum) + seatChar;
                    seats.emplace_back(seat, false, price1);
                }
                rows.emplace_back(rowNum, seats);
            }

            int rowStart2 = stoi(rowRange2.substr(0, rowRange2.find('-')));
            int rowEnd2 = stoi(rowRange2.substr(rowRange2.find('-') + 1));
            int price2 = stoi(priceStr2.substr(0, priceStr2.find('$')));

            for (int rowNum = rowStart2; rowNum <= rowEnd2; ++rowNum) {
                vector<Seat> seats;
                for (char seatChar = 'A'; seatChar < 'A' + seatsPerRow; ++seatChar) {
                    string seat;
                    seat = to_string(rowNum) + seatChar;
                    seats.emplace_back(seat, false, price2);
                }
                rows.emplace_back(rowNum, seats);
            }

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
    map<string, Ticket> tickets;
    int ticketCounter = 10000;

public:
    Airbnb(const vector<Airplane>& airplanes) : airplanes(airplanes) {}

    static string generateID() {
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        string id;
        id.reserve(10);

        for (int i = 0; i < 10; ++i) {
            id += alphanum[rand() % (sizeof(alphanum) - 1)];
        }
        return id;
    }

    string bookSeat(const string& flightNumber, const string& seatNumber, const string& date, const string& passengerName) {
        // buy a ticket for the flight
        // in the end tell the success of this operation by addimg an ID to the ticket
        // if the seat is already booked, return an error message

        for (Airplane& airplane : airplanes) {
            if (airplane.getFlightNumber() == flightNumber && airplane.getDate() == date) {
                if (airplane.bookSeat(seatNumber)) {
                    string ticketID = generateID();
                    int ticketPrice;
                    ticketPrice = airplane.getPrice(seatNumber);

                    tickets.emplace(ticketID, Ticket(passengerName, seatNumber, flightNumber + " " + date, true, ticketPrice, ticketID));

                    cout << "Booking successful for flight " << flightNumber << " by " << passengerName << ". Ticket ID: " << ticketID << endl;
                    return ticketID;
                } else {
                    cout << "Seat " << seatNumber << " is not available." << endl;
                    return "";
                }
            }
        }
        cout << "Flight not found." << endl;
        return "";

    }
    void checkAvailability(const string& date, const string& flightNumber) {
        // check available places for the flight  in such format: > check 1.12.23 FR12
        //> 1A 100$, 2B 100$
        for (Airplane& airplane : airplanes) {
            if (airplane.getFlightNumber() == flightNumber && airplane.getDate() == date) {
                cout << "Flight: " << flightNumber << " Date: " << date << endl;
                for (const Row& row : airplane.getRows()) {
                    for (const Seat& seat : row.getSeats()) {
                        if (seat.isAvailable()) {
                            cout << seat.getSeatPlace() << " " << seat.getPrice() << "$" << endl;
                        }
                    }
                }
                return;
            }
            cout << "Flight not found." << endl;
        }
    }

    void returnTicket(const string& id) {
        // return ticket with refund
        // in the end tell the success of this operation
        if (auto it = tickets.find(id); it != tickets.end()) {
            Ticket& ticket = it->second;
            string seatNumber = ticket.getSeat();
            string flightInfo = ticket.getFlightInfo();

            for (Airplane& airplane : airplanes) {
                if (flightInfo.find(airplane.getFlightNumber()) != string::npos && flightInfo.find(airplane.getDate()) != string::npos) {
                    if (airplane.unbookSeat(seatNumber)) {
                        ticket.setPrice(0);
                        ticket.unbookSeat(seatNumber);
                        ticket.setID("");
                        cout << "Ticket " << id << " returned successfully. Seat " << seatNumber << " is now available." << endl;
                    } else {
                        cout << "Error: Seat " << seatNumber << " could not be unbooked." << endl;
                    }
                    return;
                }
            }
            cout << "Error: Flight not found." << endl;
        } else {
            cout << "Ticket not found." << endl;
        }
    }

    void view(const string& arg) {
        // View the booking confirmation info by id
        // return flight number, date, seat number, tickets price

        if (const auto it = tickets.find(arg); it != tickets.end()) {
            const Ticket& ticket = it->second;
            ticket.displayTicket();
            return;
        }

        cout << "Tickets for " << arg << ":\n";
        bool found = false;
        for (const auto& [id, ticket] : tickets) {
            if (ticket.getPassengerName() == arg) {
                cout << "Ticket ID: " << id << " Flight: " << ticket.getFlightInfo() << " Seat: " << ticket.getSeat() << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "No tickets found for " << arg << "." << endl;
        }
    }
    void viewUser(const string& passengerName) {
        // view all booked tickets for a particular user by username (passengerName)
        // return flight number, seat, date, ticket's price for each ticket booked by the use

        cout << "Tickets for " << passengerName << ":\n";
        for (const auto& [id, ticket] : tickets) {
            if (ticket.getPassengerName() == passengerName) {
                cout << "Ticket ID: " << id << " Flight: " << ticket.getFlightInfo() << " Seat: " << ticket.getSeat() << endl;
            }
        }
    }

    void view(const string &flightNumber, const string &date) {
        // view all booked tickets for a particular flight by flight number and date
        // return seat, passenger name, price for a chosen flight
        cout << "Tickets for flight " << flightNumber << " on " << date << ":\n";
        for (const auto& [id, ticket] : tickets) {
            if (ticket.getFlightInfo().find(flightNumber) != string::npos && ticket.getFlightInfo().find(date) != string::npos) {
                cout << "Seat: " << ticket.getSeat() << " Passenger: " << ticket.getPassengerName() << endl;
            }
        }
    }
};

int main() {
    ConfigReader configReader("/Users/dlitvakk21/CLionProjects/bookingSystem/info.txt");
    configReader.readConfig();

    Airbnb airbnb(configReader.getAirplanes());
/*
    string ticketID = airbnb.bookSeat("FQ12", "2B", "11.12.2022", "AdamSmith");
    string ticketID2 = airbnb.bookSeat("FQ12", "1F", "11.12.2022", "DanaLitvak");
    airbnb.view(ticketID);
    string ticketID3 = airbnb.bookSeat("FQ12", "2B", "11.12.2022", "AdamSandler");
    string ticketID4 = airbnb.bookSeat("HJ114", "12C", "11.12.2022", "DanaLitvak");
    airbnb.view(ticketID4);
    airbnb.view(ticketID3);
    airbnb.checkAvailability("11.12.2022", "FQ12");
    airbnb.returnTicket(ticketID);
    airbnb.checkAvailability("11.12.2022", "FQ12");
    airbnb.viewUser("AdamSmith");
    airbnb.view("HJ114", "11.12.2022");
*/
    string input;
    cout << "Welcome to the booking system! Type 'help' for a list of commands." << endl;
    while (true) {
        cout << "> ";
        getline(cin, input);

        if (input == "help") {
            cout << "Commands:\n"
                 << "book <flight number> <seat number> <date> <passenger name>\n"
                 << "check <date> <flight number>\n"
                 << "return <ticket ID>\n"
                 << "view <ticket ID>\n"
                 << "view <passenger name>\n"
                 << "view <flight number> <date>\n"
                 << "exit\n";
        } else if (input == "exit") {
            break;
        } else {
            string command;
            string arg1, arg2, arg3, arg4;
            istringstream iss(input);
            iss >> command >> arg1 >> arg2 >> arg3 >> arg4;

            if (command == "book") {
                airbnb.bookSeat(arg1, arg2, arg3, arg4);
            } else if (command == "check") {
                airbnb.checkAvailability(arg1, arg2);
            } else if (command == "return") {
                airbnb.returnTicket(arg1);
            } else if (command == "view" && arg2.empty()) {
                airbnb.view(arg1);
            } else if (command == "view") {
                airbnb.view(arg1, arg2);
            }
        }
    }

    return 0;
}

