#ifndef FLIGHT_TICKET_H
#define FLIGHT_TICKET_H

#include <iostream>
using namespace std;

//Declaring the Flight Ticket class
class Flight_Ticket
{
private: //data members
	string companyName;
	int flightNumber;
	string country_of_origin;
	string country_of_destination;
	string stopOver;
	string price;
	string time_of_departure;
	string time_of_arrival;
	string date;

public:
	Flight_Ticket() {} //default constructor
	Flight_Ticket(string companyName, int flightNumber, string country_of_origin, string country_of_destination, string stopOver, string price, string time_of_departure, string time_of_arrival, string date)
	{ //non-default constructor
		this->companyName = companyName;
		this->flightNumber = flightNumber;
		this->country_of_origin = country_of_origin;
		this->country_of_destination = country_of_destination;
		this->stopOver = stopOver;
		this->price = price;
		this->time_of_departure = time_of_departure;
		this->time_of_arrival = time_of_arrival;
		this->date = date;
	}

	//member function
	void printTicketInfo();

	//friend class
	friend class FlightHASHTABLE;

	//overloading operator ==
	bool operator==(Flight_Ticket *data);
};

void Flight_Ticket::printTicketInfo() //prints the flight ticket info
{
	cout << "Company Name: " << companyName << endl;
	cout << "FLight Number: " << flightNumber << endl;
	cout << "Country of Origin: " << country_of_origin << endl;
	cout << "Country of Destination: " << country_of_destination << endl;
	cout << "Stopover: " << stopOver << endl;
	cout << "Price: " << price << endl;
	cout << "Time of Departure: " << time_of_departure << endl;
	cout << "Time of Arrival: " << time_of_arrival << endl;
	cout << "Date: " << date << endl;
	cout << endl;
}

bool Flight_Ticket::operator==(Flight_Ticket *data) //checks whether two flight records are exactly similar
{
	if (companyName == data->companyName && flightNumber == data->flightNumber && country_of_origin == data->country_of_origin && country_of_destination == data->country_of_destination && stopOver == data->stopOver && price == data->price && time_of_departure == data->time_of_departure && time_of_arrival == data->time_of_arrival && date == data->date)
	{
		return true;
	}
	return false;
}
#endif
