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
};

class Airplane {
    string flightNumber;
    string destination;
    vector<Row> rows;

public:
    Airplane(string a, string b, const vector<Row> &c)
        : flightNumber(move(a)),
          destination(move(b)),
          rows(c) {
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
    vector<Ticket> tickets;
}

int main() {
    cout << "Hello, World!" << endl;
    return 0;
}
