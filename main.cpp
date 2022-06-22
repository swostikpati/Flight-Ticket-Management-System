
//=================================================================================
// Name         : DS Assignment#4
// Author       : Swostik Pati
// Version      : 1.0
// Date Created : Apr-17-2022
// Date Modified: May-08-2022
// Description  : Starter Code for Flight-Ticket Management System using HASH TABLE in C++
//=================================================================================
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <ctime> //to use clock() function

// including header files
#include "flightticket.h"
#include "flighthashtable.h"

//==========================================================
void listCommands()
{
	cout << "\n----------------------------------" << endl;
	cout << "import <path>      	:Import flight-tickets from a CSV file" << endl
		 << "export <path>      	:Export flight-tickets to a CSV file" << endl
		 << "count_collisions   	:Print number of collisions" << endl
		 << "add                	:Add a new flight-ticket" << endl
		 << "delete <key>       	:Delete a flight-ticket" << endl
		 << "find <key>         	:Find a flight-ticket's details" << endl
		 << "allinday <date>    	:Display all flight-tickets in a day" << endl
		 << "printASC <key>     	:Print flight-tickets in ascending order" << endl
		 << "help                    :Display Menu" << endl
		 << "exit               	:Exit the program" << endl;
}
//==========================================================

// to be completed with all the necessary method definitions

//==========================================================
int main(void)
{
	FlightHASHTABLE myFlightHASHTABLE(100003); // making a FlightHASHTABLE object - 100003 is the lowest prime number above 100k
	string user_input, command, key;		   // to take input from

	// reference assignment-3
	while (true)
	{
		listCommands();
		cout << ">";
		getline(cin, user_input);

		// parse userinput into command and key
		stringstream sstr(user_input);
		getline(sstr, command, ' ');
		getline(sstr, key);
		cout << "----------------------------------" << endl;

		if (command == "import") // checks if import function has to be called
		{
			int temp = myFlightHASHTABLE.importCSV(key); // calls the importCSV function to take input from the files
			if (temp == -1)
			{
				cout << "No records could be imported" << endl;
			}
			else
			{
				cout << "The number of records imported are: " << temp << endl; // prints the total number of records imported
			}
		}

		else if (command == "export") // checks if the export function has to be called
		{
			int temp = myFlightHASHTABLE.exportCSV(key); // calls the exportCSV function to save the data into the files
			if (temp == -1)
			{
				cout << "No records could be imported" << endl;
			}
			else
			{
				cout << "The number of records exported are: " << temp << endl; // prints the total number of records exported
			}
		}

		if (command == "add") // checks if a record has to be added
		{
			cout << "Please enter the details of the flight-ticket:" << endl; // takes the details as input from the user
			string cn, co, cd, so, p, td, ta, d;
			int fn;
			cout << "Company Name: ";
			cin >> cn;
			cout << "\nFlight Number: ";
			cin >> fn;
			cout << "\nCountry of Origin: ";
			cin >> co;
			cout << "\nCoundty of Destination: ";
			cin >> cd;
			cout << "\nStopover: ";
			cin >> so;
			cout << "\nPrice: ";
			cin >> p;
			cout << "\nTime of Departure: ";
			cin >> td;
			cout << "\nTime of Arrival: ";
			cin >> ta;
			cout << "\nDate: ";
			cin >> d;

			Flight_Ticket *temp = new Flight_Ticket(cn, fn, co, cd, so, p, td, ta, d); // creating a new flight ticket object
			int s = myFlightHASHTABLE.add(temp);
			if (s == -1)
			{
				cout << "Record already exists" << endl;
			}
			else
			{
				cout << "Flight details added successfully!" << endl;
			}
		}

		else if (command == "count_collisions") // prints the total number of collisions
		{
			cout << "The total number of collisions are: " << myFlightHASHTABLE.count_collisions() << endl;
		}

		else if (command == "delete") // checks if the user wants to delete a flight ticket
		{
			// splitting the key into companyName and flightNumber
			bool flag = true;
			string companyName(""), flightNumber("");
			for (int i = 0; i < key.length(); i++)
			{
				if (key[i] == ',')
				{
					flag = false;
					continue;
				}

				if (flag)
				{
					companyName += key[i];
				}
				else
				{
					flightNumber += key[i];
				}
			}

			myFlightHASHTABLE.removeRecord(companyName, stoi(flightNumber)); /// calling the removeRecord Function
		}

		else if (command == "find") // checks if the user wants to search a flight ticket
		{
			// splitting the key into companyName and flightNumber
			bool flag = true;
			string companyName(""), flightNumber("");
			for (int i = 0; i < key.length(); i++)
			{
				if (key[i] == ',')
				{
					flag = false;
					continue;
				}

				if (flag)
				{
					companyName += key[i];
				}
				else
				{
					flightNumber += key[i];
				}
			}

			int start = clock();									 // calculating time taken for the find function to search through the hashtable - reference lab 13 starter code
			myFlightHASHTABLE.find(companyName, stoi(flightNumber)); // calling the find function
			int stop = clock();
			cout << "Time Taken: " << float(stop - start) / CLOCKS_PER_SEC << " seconds" << endl;
		}
		else if (command == "allinday") // checks if the user wants to print all the records in a specific date
		{
			myFlightHASHTABLE.allinday(key);
		}
		else if (command == "printASC") // checks if the user wants print the flight ticket details in a ascending order
		{
			// splitting the key into companyName and flightNumber
			bool flag = true;
			string companyName(""), flightNumber("");
			for (int i = 0; i < key.length(); i++)
			{
				if (key[i] == ',')
				{
					flag = false;
					continue;
				}

				if (flag)
				{
					companyName += key[i];
				}
				else
				{
					flightNumber += key[i];
				}
			}
			myFlightHASHTABLE.printASC(companyName, stoi(flightNumber)); // calling the printASC function
		}

		else if (command == "help") // checks if the user wants to view the menu again
		{
			listCommands(); // calls the function to list all the commands again
		}
		else if (command == "exit") // checks if the user wants to exit from this function
		{
			cout << "Thank you Exited!" << endl;
			return 0;
		}
		else
		{
			cout << "Error! Please enter a valid input" << endl;
		}
	}

	cout << "Okay" << endl;

	return 0;
}
