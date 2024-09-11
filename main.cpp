#include <iostream>
#include <utility>
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

int main() {
    cout << "Hello, World!" << endl;
    return 0;
}
