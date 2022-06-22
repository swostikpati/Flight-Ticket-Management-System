#ifndef FlightHASHTABLE_H
#define FlightHASHTABLE_H

#include <iostream>
#include "flightticket.h"
#include <fstream>
// include additional header files as needed

#define hashArrLength 100000 // defining the length of the hashcodes array (big array) which stores all hashcodes

using namespace std;

//some functions and classes are constructed taking reference from lab 11 task 3 startercode and solutions
// HashNode Class Declaration
class HashNode
{
private:
	string key;			 // stores the key - companyName + flightNumber
	Flight_Ticket value; // stores the flight ticket object

public:
	HashNode() {}							  // default constructor
	HashNode(string key, Flight_Ticket value) // non-default constructor
	{
		this->key = key;
		this->value = value;
	}

	// friend classes
	friend class FlightHASHTABLE;
	friend struct Node;
};

// NodeList Class Declaration
typedef HashNode Elem; // defining Elem to represent HashNode datatype
class NodeList
{
public:
	// Node Structure
	struct Node
	{
		Elem elem;
		Node *prev;
		Node *next;
	};

	// Iterator Class
	class Iterator
	{
	public:
		// overloading operators
		Elem &operator*();
		bool operator==(const Iterator &p) const;
		bool operator!=(const Iterator &p) const;
		Iterator &operator++();
		Iterator &operator--();

		// friend classes
		friend class NodeList;
		friend class FlightHASHTABLE;

	private:
		Node *v;					 // pointer to the node
		Iterator(Node *u) { v = u; } // constructor
	};

public:
	NodeList();									 // default constructor
	~NodeList();								 // destructor
	int size() const;							 // returns list size
	bool empty() const;							 // checks if list is empty
	Iterator begin() const;						 // beginning position
	Iterator end() const;						 // (just beyond) last position
	void push_back(const Elem &e);				 // inserts at the back of the list
	void push(const Iterator &p, const Elem &e); // insert e before p
	void erase(const Iterator &p);				 // remove p from the list

	// overloading operator
	Elem &operator[](int n); // gives the element at index n

private:		   // data members
	int n;		   // number of items
	Node *header;  // head-of-list sentinel
	Node *trailer; // tail-of-list sentinel
};
//=============================================================================
// FlightHASHTABLE Class
class FlightHASHTABLE
{
private:
	int size;					  // Current Size of HashTable
	int capacity;				  // Total Capacity of HashTable
	NodeList *buckets;			  // List to store the HashNodes
	int probes;					  // Stores the number of probes
	int collisions;				  // Stores the number of collisions
	int hashCodes[hashArrLength]; // Stores all the hashCode indexes generated

public:
	FlightHASHTABLE() {}		   // default constructor
	FlightHASHTABLE(int capacity); // constructor
	~FlightHASHTABLE();			   // destructor

	unsigned long hashCode(string key);						 // function that returns a hashCode
	void insert(string key, Flight_Ticket *value);			 // function to insert into the hashCode
	bool searchInternal(Flight_Ticket *data);				 // function that searches through the hashCode for same records
	int getSize();											 // returns the size of the  hashtable
	int getprobes();										 // returns the number of probes
	int importCSV(string path);								 // Load all the flight-tickets from the CSV file to the HashTable
	int exportCSV(string path);								 // Export all the flight-tickets from the HashTable to a CSV file in an ascending order
	int count_collisions();									 // return the number of collisions on the HashTable
	int add(Flight_Ticket *data);							 // add new flight to the HashTable
	void removeRecord(string companyName, int flightNumber); // Delete a record with key from the hashtable
	void find(string companyName, int flightNumber);		 // Find and Display records with same key entered
	void allinday(string date);								 // Find and Display records with same day entered
	void printASC(string companyName, int flightNumber);	 // display the collisions for the entered key in an ascending order

	// friend class
	friend class Flight_Ticket;
};

NodeList::NodeList() // default constructor for NodeList Class
{
	n = 0;
	header = new Node;
	trailer = new Node;
	header->next = trailer;
	trailer->prev = header;
}

NodeList::~NodeList() // destructor
{
	Node *curr = header;
	while (curr->next != trailer)
	{
		Node *temp = curr;
		curr = curr->next;
		delete temp;
	}
}

int NodeList::size() const // returns the size of the NodeList
{
	return n;
}

bool NodeList::empty() const // returns if list is empty
{
	return (n == 0);
}

NodeList::Iterator NodeList::begin() const
{
	return Iterator(header->next);
}

NodeList::Iterator NodeList::end() const
{
	return Iterator(trailer);
}

// function to remove a record
void NodeList::erase(const Iterator &p)
{
	Node *v = p.v;
	Node *w = v->next;
	Node *u = v->prev;
	u->next = w;
	w->prev = u;
	delete v;
	n--; // update the number of elements
}

// inserts element before p
void NodeList::push(const Iterator &p, const Elem &e)
{
	Node *w = p.v; // w points to the node associated with p
	Node *u = w->prev;
	Node *v = new Node;
	v->elem = e;
	v->next = w;
	w->prev = v;
	v->prev = u;
	u->next = v;
	n++; // update the number of elements
}

// inserts element at the end
void NodeList::push_back(const Elem &e)
{
	Node *w = trailer;
	Node *u = w->prev;
	Node *v = new Node;
	v->elem = e;
	v->next = w;
	w->prev = v;
	v->prev = u;
	u->next = v;
	n++; // updates number of elements
}

// overloading operator *
Elem &NodeList::Iterator::operator*()
{
	return v->elem;
}

// overloading operator ==
bool NodeList::Iterator::operator==(const Iterator &p) const
{
	return v == p.v;
}

// overloading operator !=
bool NodeList::Iterator::operator!=(const Iterator &p) const
{
	return v != p.v;
}

// overloading operator ++
NodeList::Iterator &NodeList::Iterator::operator++()
{
	v = v->next;
	return *this;
}

// overloading operator --
NodeList::Iterator &NodeList::Iterator::operator--()
{
	v = v->prev;
	return *this;
}

// overloading operator [] - returns element at index n
Elem &NodeList::operator[](int n)
{
	Node *curr = header;
	for (int i = 0; i < n; i++)
	{
		curr = curr->next;
	}
	return curr->elem;
}

// FlightHASHTABLE constructor
FlightHASHTABLE::FlightHASHTABLE(int capacity)
{
	buckets = new NodeList[capacity]; // Array of a list/chain
	this->capacity = capacity;
	this->size = 0;
	this->probes = 0;
	this->collisions = 0;
	for (int i = 0; i < hashArrLength; i++)
	{
		hashCodes[i] = -1; // initializing the hashCodes array with -1s
	}
}

// destructor
FlightHASHTABLE::~FlightHASHTABLE()
{
	delete[] buckets;
}

// function that returns hashCodes
unsigned long FlightHASHTABLE::hashCode(string key)
{
	//reference for hashcodes - slides and previous implementations

	// Hash Code 1 - Cycle Shift HashCode
	int m = key.length();
	unsigned int h = 0;
	for (int i = 0; i < m; i++)
	{
		h = (h << 5) | (h >> 27); //cycle shifting the 32 bits
		h += (unsigned int)key[i];
	}

	return h % this->capacity; //hashcode stays in the range 0 - N-1;

	// HashCode 2 - Polynomial HashCode
	/*
	unsigned int sum = 0;
	unsigned short int a = 2;

	//implementing the horner's method of calculating higher powers - reference https://youtu.be/bwiYI3S_iHs
	for (int i = key.length() - 1; i >= 0; i--)
	{
		sum = (sum * a) + (unsigned int)key[i]; 
	}
	int32_t result = sum % this->capacity; //preventing overflow

	return result;*/

	// Hash Code 3 - Summation HashCode
	/*unsigned int sum = 0;
	for (int i = 0; i < key.length(); i++)
	{
		sum += (unsigned int)key[i]; //summing up the ascii values of characters
	}
	int32_t result = sum % this->capacity; //preventing overflow
	
	return result;*/
}
//================================================
void FlightHASHTABLE::insert(string key, Flight_Ticket *value) // helper function for adding elements
{
	int index = hashCode(key); // Find the bucket index where the new node will fall, make sure that the index is within the bounds of array

	// saving hashcodes
	bool f1 = true; // to check if the index given by the hashcode is already present in the hashcode array

	int c(0); // to find the next empty index in the array
	for (int i = 0; i < hashArrLength; i++)
	{
		if (hashCodes[i] == -1) // prematurely stop the iteration if no more values are present in the array
		{
			c = i; // gives the next empty index
			break;
		}

		if (hashCodes[i] == index) // if the index generated is already present
		{
			f1 = false; // flag becomes false
			break;
		}
	}
	if (f1) // if flag is still true then the index is added to the array of hashcode indexes
	{
		hashCodes[c] = index;
	}

	if (buckets[index].size() >= 1) // whenever the size of the current bucket is at least 1, collisions occur
	{
		collisions++;
	}

	buckets[index].push_back(HashNode(key, *value)); // new element is added to the bucket list
	this->size++;									 // size of the hashtable is incremented
}
//===================================================
void FlightHASHTABLE::find(string companyName, int flightNumber) // function to find the entry given a key
{
	string key = companyName + to_string(flightNumber); // generating the key
	int index = hashCode(key);							// Get the index of the bucket
	int counter = 0;									// counter used to display serial number and display total records

	for (NodeList::Iterator it = buckets[index].begin(); it != buckets[index].end(); ++it)
	{
		if (it.v->elem.key == key) // whenever key is equal the details are presented
		{
			counter++;
			cout << counter << ". ";
			it.v->elem.value.printTicketInfo();
		}
	}
	if (counter == 0) // if no records are found
	{
		cout << "No Records Found." << endl;
	}
	else // if records are found the total number is displayed
	{
		cout << "Total no. of Records Found: " << counter << endl;
	}
}
//===================================================
int FlightHASHTABLE::getSize() // returns the size of the hashtable
{
	return this->size;
}
//===================================================
int FlightHASHTABLE::getprobes() // returns the total number of probes
{
	return this->probes;
}

bool FlightHASHTABLE::searchInternal(Flight_Ticket *data) // checks in the hashtable to find exactly identical entries
{
	string key = data->companyName + to_string(data->flightNumber);
	int index = hashCode(key); // Get the index of the bucket

	for (NodeList::Iterator it = buckets[index].begin(); it != buckets[index].end(); ++it)
	{
		if (it.v->elem.value == data) // overloaded == operator is called to check two Flight ticket entries
		{
			return true;
		}
	}
	return false;
}

int FlightHASHTABLE::add(Flight_Ticket *data) // adds a new entry
{
	string key = data->companyName + to_string(data->flightNumber);

	if (searchInternal(data)) // checks if identical entry is present
	{
		return -1;
	}

	insert(key, data); // calls the insert function to insert the entry into hashtable
	return 1;
}

void FlightHASHTABLE::removeRecord(string companyName, int flightNumber) // removes entry
{
	string key = companyName + to_string(flightNumber);
	int index = hashCode(key); // Get the index of the bucket
	int counter = 0;		   // to display serial number

	for (NodeList::Iterator it = buckets[index].begin(); it != buckets[index].end(); ++it) // displaying all records under the key
	{
		if (it.v->elem.key == key)
		{
			counter++;
			cout << counter << ". ";
			it.v->elem.value.printTicketInfo();
		}
	}
	if (counter == 0) // if no records match the key then the program simply returns
	{
		cout << "No Records Found" << endl;
		return;
	}

	// asking user to input which record they want to delete
	int n;
	do
	{
		cout << "\nPlease select the record you want to delete: " << endl;
		cin >> n;
	} while (n < 1 or n > counter);

	int i(1);
	for (NodeList::Iterator it = buckets[index].begin(); it != buckets[index].end(); ++it)
	{
		if (it.v->elem.key == key)
		{
			if (i == n)
			{
				buckets[index].erase(it);
				cout << "Record Deleted" << endl;
				return;
			}
			i++;
		}
	}
}

void FlightHASHTABLE::allinday(string date) // displays all the records present in a specific day
{
	int counter(0);							// to diplay serial number
	for (int i = 0; i < hashArrLength; i++) // goes through the hashCodes array to find each index
	{
		if (hashCodes[i] == -1) // pre-maturely breaks out of the loop when there are no more hash code indexes present
		{
			break;
		}

		int index = hashCodes[i]; // takes a index from the hashCodes array
		for (NodeList::Iterator it = buckets[index].begin(); it != buckets[index].end(); ++it)
		{
			if (it.v->elem.value.date == date) // checks date with every element in the hashtable
			{
				counter++;
				cout << counter << ". ";
				it.v->elem.value.printTicketInfo();
			}
		}
	}

	cout << "Total Records Found: " << counter << endl;
}

int FlightHASHTABLE::count_collisions() // returns the number of collisions
{
	return collisions;
}

int FlightHASHTABLE::importCSV(string path) // imports records from a file based on the path provided
{

	ifstream myfile(path, ios::in); // opens file
	if (myfile.fail())				// checks if file fails to open
	{
		cout << "File failed to open" << endl;
		return -1;
	}

	int count(0);							 // stores the number of records to be imported
	Flight_Ticket *temp = new Flight_Ticket; // creates new flight ticket object
	string word;							 // word keeps each word with the comma removed

	int i = 0;
	bool f = true;
	string word1, word2; // helper variables for importing

	// reference - assignment2
	while (!myfile.eof()) // traverses to the end of the file
	{
		bool flag;	// helper flag
		if (i == 8) // exception case when delimiter fails to ignore \n (newline) character
		{
			getline(myfile, word, ','); // splits each line based on delimiter comma
			flag = true;
			word1 = "";
			word2 = "";
			for (int i = 0; i < word.length(); i++)
			{
				if (word[i] == '\n') // splits last word of each line based on newline character
				{
					flag = false;
					continue;
				}
				if (flag)
				{
					word1 += word[i];
				}
				else
				{
					word2 += word[i];
				}
			}
		}
		else // normal cases
		{
			getline(myfile, word, ',');
		}

		// switch case to set each parameter of the flight ticket object
		switch (i)
		{
		case 0:
		{
			if (f)
			{
				f = false;
				temp->companyName = word;
				i++;
				break;
			}
			else
			{
				temp->companyName = word2;
				i++;
			}
		}
		case 1:
		{
			temp->flightNumber = stoi(word); // converts string to integer
			i++;
			break;
		}
		case 2:
		{
			temp->country_of_origin = word;
			i++;
			break;
		}
		case 3:
		{
			temp->country_of_destination = word;
			i++;
			break;
		}
		case 4:
		{
			temp->stopOver = word;
			i++;
			break;
		}
		case 5:
		{
			temp->price = word;
			i++;
			break;
		}
		case 6:
		{
			temp->time_of_departure = word;
			i++;
			break;
		}
		case 7:
		{
			temp->time_of_arrival = word;
			i++;
			break;
		}
		case 8:
		{
			temp->date = word1;
			i = 0;
			this->add(temp); // adds the flight ticket object to the hashtabl;e
			count++;
			break;
		}
		default:
		{
			break;
		}
		}
	}
	myfile.close(); // closing the file
	return count;	// returning the total number of records imported
}

void FlightHASHTABLE::printASC(string companyName, int flightNumber)
{
	string key = companyName + to_string(flightNumber);
	int index = hashCode(key);
	int counter(0);
	Flight_Ticket ticketArr[this->getSize()]; // creating a flight ticket object array to help in sorting

	for (NodeList::Iterator it = buckets[index].begin(); it != buckets[index].end(); ++it)
	{
		if (it.v->elem.key == key)
		{
			ticketArr[counter] = it.v->elem.value; // copying hashtable values into the ticket array
			counter++;
		}
	}

	// sorting the array - selection sort
	for (int i = 0; i < counter - 1; i++)
	{
		for (int j = i + 1; j < counter; j++)
		{
			if (ticketArr[j].country_of_destination < ticketArr[i].country_of_destination) // destination country is used as the parameter to sort
			{
				Flight_Ticket temp = ticketArr[i];
				ticketArr[i] = ticketArr[j];
				ticketArr[j] = temp;
			}
		}
	}

	// printing the flight ticket objects in ascending order
	for (int i = 0; i < counter; i++)
	{
		cout << i + 1 << ". ";
		ticketArr[i].printTicketInfo();
	}
}

int FlightHASHTABLE::exportCSV(string path) // exports records and writes them into a file
{
	ofstream myfile;
	myfile.open(path, ios::out);
	if (myfile.fail()) // checks if file fails to open
	{
		return -1;
	}
	int count(0); // stores the count of records exported

	for (int i = 0; i < hashArrLength; i++) // goes through each hash index
	{
		if (hashCodes[i] == -1) // pre-maturely ends the loop when no more hash indexes are present
		{
			break;
		}

		int index = hashCodes[i]; // takes one hash index at a time
		// writes all the records
		for (NodeList::Iterator it = buckets[index].begin(); it != buckets[index].end(); ++it)
		{
			count++;
			Flight_Ticket temp = it.v->elem.value;
			myfile << temp.companyName << ",";
			myfile << temp.flightNumber << ",";
			myfile << temp.country_of_origin << ",";
			myfile << temp.country_of_destination << ",";
			myfile << temp.stopOver << ",";
			myfile << temp.price << ",";
			myfile << temp.time_of_departure << ",";
			myfile << temp.time_of_arrival << ",";
			myfile << temp.date << endl;
		}
	}

	myfile.close(); // closing the file
	return count;	// returning the total number of records written
}

#endif
