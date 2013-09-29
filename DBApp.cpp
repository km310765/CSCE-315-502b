#include "DBApp.h"

void DBApp:: CreateTable() {
	p.parse("CREATE TABLE cars (Car_ID INTEGER, Name VARCHAR(1), model VARCHAR(1), year INTEGER, engine_type INTEGER) PRIMARY KEY (carid);", true);
	p.parse("INSERT INTO cars VALUES FROM (5,\"Joe\", \"cat\", 4,6);", true);
	p.parse("CREATE TABLE hello (carid INTEGER, name VARCHAR(1), model VARCHAR(1), year INTEGER, engine_type INTEGER) PRIMARY KEY (carid);", true);
	p.parse("INSERT INTO cars VALUES FROM (7,\"bob\", \"the builder\", 46,67);", true);
	p.parse("CREATE TABLE Shipments (Car_ID INTEGER, Name VARCHAR(1), model VARCHAR(1), year INTEGER, engine_type INTEGER) PRIMARY KEY (carid);", true);
	p.parse("INSERT INTO cars VALUES FROM (001,\"Audi\", \"R8\", 2013,34);", true);
	p.parse("INSERT INTO cars VALUES FROM (002,\"Chevrolet\", \"Cobalt\", 2013,36);", true);
	p.parse("INSERT INTO cars VALUES FROM (002,\"Chevrolet\", \"Malibu\", 2014,56);", true);
	p.parse("INSERT INTO cars VALUES FROM (003,\"Nissan\", \"Altima\", 2011,38);", true);
	p.parse("INSERT INTO cars VALUES FROM (004,\"Audi\", \"A8\", 2014,43);", true);
	p.parse("INSERT INTO Shipments VALUES FROM (004,\"Audi\", \"A8\", 2015,43);", true);
	p.parse("INSERT INTO Shipments VALUES FROM (004,\"Audi\", \"A8\", 2015,43);", true);
}

void DBApp:: Create_Customer_table() {
	p.parse("CREATE TABLE Customers (Customer_ID INTEGER, FirstName VARCHAR(1), LastName VARCHAR(1), Car_bought VARCHAR(20)) PRIMARY KEY (Customer_ID);", true);
	p.parse("INSERT INTO Customers VALUES FROM (111,\"Saurabh\", \"Mishra\", \"Nissan\");", true);
	p.parse("INSERT INTO Customers VALUES FROM (112,\"Bradley\", \"Abbot\", \"Audi\");", true);
	p.parse("INSERT INTO Customers VALUES FROM (113,\"Michael\", \"Karnes\", \"Mercedes\");", true);
}

void DBApp:: SearchCarsByName() {
	string car_name;
	cout << "Enter the name of tha car you want to search " << endl;
	cin >> car_name;
	p.parse("Search_result <- select (Name == \""+car_name+"\""+") cars;", true);
	p.parse("SHOW Search_result;", true);
	cout << "Search_result <- select (Name == \""+car_name+"\""+");"<< endl;
}

void DBApp:: ShowCustomerNames() {
	p.parse("Customer_names <- project(FirstName,LastName) Customers;", true);
	p.parse("SHOW Customer_names;", true);
}

void DBApp:: AddCar(string table_name) {
	string carID, name, model, year, engine;
	cout << "Enter the Name of the car " << endl;
	cin >> name;
	cout << "Enter the Car ID of the car " << endl;
	cin >> carID; 
	cout << "Enter the Model the car " << endl;
	cin >> model;
	cout << "Enter the Year of the car " << endl;
	cin >> year;
	cout << "Enter the engine type of the car " << endl;
	cin >> engine;
	
	p.parse("INSERT INTO "+table_name+" VALUES FROM ("+carID+", "+ "\"" + name +"\"" + ", " + "\"" + model +"\"" +", " + year +", "+engine+");", true);
	cout << "Values have been added :) " << endl;
}

void DBApp:: Diff() {
	p.parse("NotInWarehouse <- Shipments - cars;", true);
	p.parse("SHOW NotInWarehouse;", true);
}

void DBApp:: ShowCarsByManufacturers(string table_name) { 
	int x;
	cout << "Please select the manufacturer of the car you want to see" << endl;
	cout << "1. General Motors " << endl;
	cout << "2. Chevrolet " << endl;
	cout << "3. Mercedes " << endl;
	cout << "4. Audi " << endl;
	cout << "5. Nissan " << endl;
	cin >> x;
	
	if(x == 1) {
	}
	
	if(x == 2) {
		p.parse("Chevrolet <- select (Name == \"Chevrolet\") "+table_name+";", true);
		p.parse("SHOW Chevrolet;", true);
	}
	
	if(x == 3) {
		p.parse("Mercedes <- select (Name == \"Mercedes\") "+table_name+";", true);
		p.parse("SHOW Mercedes;", true);
	}
	
	if(x == 4) {
		p.parse("Audi <- select (Name == \"Audi\") "+table_name+";", true);
		p.parse("SHOW Audi;", true);
	}
	
	if(x == 5) {
		p.parse("Nissan <- select (Name == \"Nissan\") "+table_name+";", true);
		p.parse("SHOW Nissan;", true);
	}
	
	else {
		cout << "Wrong input " << endl;
		return;
	}
	
}

void DBApp:: UpdateCar(string table_name) {
	//cout << "\nUPDATE "+table_name+" SET WHERE (name == "+car_name+");" << endl;
	int n;
	cout << "Choose the information about the  car that you would like to update " << endl;
	cout << "1. Name of the car " << endl;
	cout << "2. Id of the car " << endl;
	cout << "3. Model of the car " << endl;
	cout << "4. Year of the car " << endl;
	cout << "5. Engine type of the car " << endl;
	cin >> n;
	
	if(n == 1) {
		string new_name, old_name;
		cout << "Enter old name of the car " << endl;
		cin >> old_name;
		cout << "Enter new name of the car " << endl;
		cin >> new_name;
		p.parse("UPDATE "+table_name+" SET Name = \""+new_name+"\" WHERE Name == \""+old_name+"\";", true);
	}	
	return;
}
void DBApp:: ShowTable(string tablename) {
	p.parse("SHOW " + tablename + ";", true);
	//parser.parse("SHOW " + tablename+";");
	return;
}

void DBApp:: ShowCustomers() {
	p.parse("SHOW Customers;", true);
}

void DBApp:: DeleteCar(string car_name, string table_name) {
	//cout << "\nDELETE FROM "+table_name+" WHERE (name == "+car_name+");" << endl;
	p.parse("DELETE FROM "+table_name+" WHERE (Name == \""+car_name+"\");", true);
	return;
}

void DBApp:: ShowCarWithID(string table_name) {
	p.parse("Cars_ID <- project (carid,name) cars;", true);
	p.parse("SHOW Cars_ID;", true);
	return;
}

void DBApp:: Exit() {
	p.parse("EXIT;", true);
}

void DBApp:: test() {
	//p.parse("UPDATE cars SET Name = 0 WHERE Name == Chevrolet;");
	p.parse("Search_result <- select (Name == \"Audi\") cars;", true);
	//p.parse("DELETE FROM cars WHERE (Name == Audi);");
	p.parse("SHOW cars;", true);
}
	
int main() {
int x;


 DBApp d;
 d.CreateTable();
 d.Create_Customer_table();
 string line;
 //d.test();
// d.CreateTable();
 while(true) {
		cout << "\nThis is a simple Database Application " << endl;
		cout << "\nPlease choose what you would like to do" << endl;

		cout << "1. Add Cars the to the warehouse or Shipments " << endl;
		cout << "2. Delete a Car from the warehouse " << endl;
		cout << "3. Show Cars and their ID " << endl;
		cout << "4. Update information about a Car in the warehouse " << endl;
		cout << "5. Show table of cars " << endl;
		cout << "6. Show Customers " << endl;
		cout << "7. Show the cars that are not in the warehouse but are in the shipment " << endl;
		cout << "8. Search cars by name " << endl;
		cout << "9. Show cars by manufacturers " << endl;
		cout << "10. Show the names of all customers " << endl;
		cout << "11. Exit " << endl;
	
		cin >> x;
		
		if(x == 1) {
			string table_name;
			cout << "Enter the name of the table you want to add the cars to, cars or Shipments" << endl;
			cin >> table_name;
			d.AddCar(table_name);
		}
		
		if(x == 2) {
			string car_name,table_name;
			cout <<"Enter the name of the table from which the car needs to be deleted " << endl;
			cin >> table_name;
			cout << "Enter the name of the car to be deleted " << endl;
			cin >> car_name;
			d.DeleteCar(car_name,table_name);
			cout << "\n....Returning to the main menu " << endl;
		}
		
		if(x == 3) {
			string table_name;
			cout << "Enter the name of the table you want to select from " << endl;
			cin >> table_name;
			d.ShowCarWithID(table_name);
		}
		
		if(x == 4) {
			string table_name;
			cout << "Enter the name of the table you want to update " << endl;
			cin >> table_name;
			d.UpdateCar(table_name);
		}
		
		if(x == 5) {
			string name;
			cout << "Enter the name of the table to show " << endl;
			cin >> name;
			d.ShowTable(name);
			cout << "\n....Returning to the main menu " << endl;
		}			
		
		if(x == 6) {
			d.ShowCustomers();
		}
		
		if(x == 7) {
			d.Diff();
		}
		
		if(x == 8) {
			d.SearchCarsByName();
		}
		
		if(x == 9) {
			string table_name;
			cout << "Enter the name of the table,\"cars\" or \"Shipments\" " << endl;
			cin >> table_name;
			d.ShowCarsByManufacturers(table_name);
		}
		
		if(x == 10) {
			cout << "\nHere are all the customers who bought cars from us :)" << endl;
			d.ShowCustomerNames();
		}
		
		if(x == 11) {
			cout << " \n         Good Bye :) " << endl;
			d.Exit();
		}	
		
		if(x == 12) {
			d.test();
		}
		
		else {
			cout << "Invalid Input, Please choose from the list. " << endl;
		}
	}
}
