#include "DBApp.h"

void DBApp:: CreateTable() {
	p.parse("OPEN cars;", true);
	p.parse("OPEN Customers;", true);
	p.parse("OPEN Shipments;", true);
	p.parse("OPEN Customers_car_info;", true);
	p.parse("OPEN Unsold_Cars;", true);
	cout << "\nLoaded tables cars, Customers, Shipments, Customers_car_info, and Unsold_Cars " << endl;
}

void DBApp:: Close(){
	p.parse("CLOSE;", true);
}
void DBApp:: AddShipment() {
	p.parse("cars <- cars + Shipments;",true);
	p.parse("SHOW cars;",true);
}

void DBApp:: UnsoldCars() {
	p.parse("Unsold_Cars <- cars - (project (Car_ID, Name, model, year) (select (Car_ID == B_Car_ID) (cars * Customers)));", true); 
	p.parse("SHOW Unsold_Cars;", true);
}
void DBApp:: SearchCarsByName() {
	string car_name;
	cout << "Enter the name of the car you want to search " << endl;
	cin >> car_name;
	p.parse("SHOW (select (Name == \""+car_name+"\""+") cars);", true);
}

void DBApp:: ShowCustomerNames() {
	p.parse("SHOW (project(FirstName,LastName) Customers);", true);
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
	
	p.parse("INSERT INTO "+table_name+" VALUES FROM ("+carID+", "+ "\"" + name +"\"" + ", " + "\"" + model +"\"" +", " + year + ");", true);
	cout << "Values have been added :) " << endl;
}

void DBApp:: Diff() {
	p.parse("SHOW (Shipments - cars);", true);
}

void DBApp:: UpdateCar(string table_name) {
	int n;
	cout << "Choose the information about the  car that you would like to update " << endl;
	cout << "1. Name of the car " << endl;
	cout << "2. Id of the car " << endl;
	cout << "3. Model of the car " << endl;
	cout << "4. Year of the car " << endl;
	cin >> n;
	
	if(n == 1) {
		string new_name, old_name;
		cout << "Enter old name of the car " << endl;
		cin >> old_name;
		cout << "Enter new name of the car " << endl;
		cin >> new_name;
		p.parse("UPDATE "+table_name+" SET Name = \""+new_name+"\" WHERE Name == \""+old_name+"\";", true);
	}	
	if(n == 2) {
		string new_Id, old_Id;
		cout << "Enter old Id of the car " << endl;
		cin >> old_Id;
		cout << "Enter new Id of the car " << endl;
		cin >> new_Id;
		p.parse("UPDATE "+table_name+" SET Car_ID = "+new_Id+" WHERE Car_ID == "+old_Id+";", true);
	}	
	if(n == 3) {
		string new_model, old_model;
		cout << "Enter old model of the car " << endl;
		cin >> old_model;
		cout << "Enter new model of the car " << endl;
		cin >> new_model;
		p.parse("UPDATE "+table_name+" SET model = \""+new_model+"\" WHERE model == \""+old_model+"\";", true);
	}	
	if(n == 4) {
		string new_year, old_year;
		cout << "Enter old year of the car " << endl;
		cin >> old_year;
		cout << "Enter new year of the car " << endl;
		cin >> new_year;
		p.parse("UPDATE "+table_name+" SET year = "+new_year+" WHERE year == "+old_year+";", true);
	}
	return;
}
void DBApp:: ShowTable(string tablename) {
	p.parse("SHOW " + tablename + ";", true);
	return;
}

void DBApp:: Customers_car_info() {
	p.parse("Customers_car_info <- project (Customer_ID, FirstName, LastName, Car_ID, Name, model, year) (select (Car_ID == B_Car_ID) (cars * Customers))", true); 
	p.parse("SHOW Customers_car_info;", true);
}
void DBApp:: DeleteCar(string car_Id, string table_name) {
	p.parse("DELETE FROM "+table_name+" WHERE (Car_ID == \""+car_Id+"\");", true);
	return;
}

void DBApp:: ShowCarWithID(string table_name) {
	p.parse("SHOW (project (Car_ID,Name) cars);", true);
	return;
}

void DBApp:: Exit() {
	p.parse("EXIT;", true);
}

void DBApp:: Write(string table_name) {
	p.parse("WRITE "+table_name+";",true);	
}

void DBApp:: Open(string table_name) {
	p.parse("OPEN "+table_name+";",true);
}

int main() {
	int x = 0;
	 DBApp d;
	 d.CreateTable();
	 string line;
	 cout << "\nThis is a simple Database Application " << endl;
 //d.test();
// d.CreateTable();
 while(true) {
		cout << "\nPlease choose what you would like to do" << endl;
		cout << "1. Add Cars the to the warehouse or Shipments " << endl;
		cout << "2. Delete a Car from the warehouse " << endl;
		cout << "3. Show Cars and their ID " << endl;
		cout << "4. Update information about a Car in the warehouse " << endl;
		cout << "5. Show table " << endl;
		cout << "6. Show Customers and their car's information" << endl;
		cout << "7. Show Unsold Cars" << endl;
		cout << "8. Show the cars that are not in the warehouse but are in the shipment " << endl;
		cout << "9. Search cars by name " << endl;
		cout << "10. Show the names of all customers " << endl;
		cout << "11. Add the shipment to the cars in the warehouse " << endl;
		cout << "12. Write back to the file " << endl;
		cout << "13. Open an existing table " << endl;
		cout << "14. Exit " << endl;
		
		cin >> x;
		
		if(x == 1) {
			string table_name;
			cout << "Enter the name of the table you want to add the cars to, cars or Shipments" << endl;
			cin >> table_name;
			d.AddCar(table_name);
		}
		
		else if(x == 2) {
			string car_Id,table_name;
			cout <<"Enter the name of the table from which the car needs to be deleted " << endl;
			cin >> table_name;
			cout << "Enter the Car_ID of the car to be deleted " << endl;
			cin >> car_Id;
			d.DeleteCar(car_Id,table_name);
		}
		
		else if(x == 3) {
			string table_name;
			cout << "Enter the name of the table you want to select from " << endl;
			cin >> table_name;
			d.ShowCarWithID(table_name);
		}
		
		else if(x == 4) {
			string table_name;
			cout << "Enter the name of the table you want to update " << endl;
			cin >> table_name;
			d.UpdateCar(table_name);
		}
		
		else if(x == 5) {
			string name;
			cout << "Enter the name of the table to show " << endl;
			cin >> name;
			d.ShowTable(name);
		}			
		
		else if(x == 6) {
			d.Customers_car_info();
		}
		
		else if(x == 7){
			d.UnsoldCars();
		}
		else if(x == 8) {
			d.Diff();
		}
		
		else if(x == 9) {
			d.SearchCarsByName();
		}
		
		else if(x == 10) {
			cout << "\nHere are all the customers who bought cars from us :)" << endl;
			d.ShowCustomerNames();
		}
		
		else if(x == 11) {
			d.AddShipment();
		}
		
		else if(x == 12) { 
			string table_name;
			cout << "Enter the name of the table to be written back to the file " << endl;
			cin >> table_name;
			d.Write(table_name);
		}
		
		else if(x == 13) {
			string table_name;
			cout << "Enter the name of the table to be opened from the file " << endl;
			cin >> table_name;
			d.Open(table_name);
		}
		
		else if(x == 14) {
			cout << " \n         Good Bye :) " << endl;
			d.Close();
			d.Exit();
		}	
		
		else {
			cout << "Invalid Input, Please choose from the list. " << endl;
		}
	}
}
