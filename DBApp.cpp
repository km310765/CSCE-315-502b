#include "DBApp.h"

void DBApp:: CreateTable() {
	p.parse("CREATE TABLE cars (carid INTEGER, name VARCHAR(1), model VARCHAR(1), year INTEGER, engine_type INTEGER) PRIMARY KEY (carid);");
	/*string tablename, name, type;
	cout << "Enter the name of the table " << endl;
	cin >> tablename;
	cout << "Enter the columns and their types one by one " << endl;
	cout << "Column Name >> " << endl;
	cin >> name;
	cout << "type (string or int) >> " << endl;
	cin >> type;*/
}

void DBApp:: UpdateCar() {
	//cout << "\nUPDATE "+table_name+" SETWHERE (name == "+car_name+");" << endl;
	return;
}
void DBApp:: ShowTable(string tablename) {
	p.parse("SHOW " + tablename + ";");
	//parser.parse("SHOW " + tablename+";");
	return;
}

void DBApp:: DeleteCar(string car_name, string table_name) {
	//cout << "\nDELETE FROM "+table_name+" WHERE (name == "+car_name+");" << endl;
	p.parse("DELETE FROM "+table_name+" WHERE (name == "+car_name+");");
	return;
}

int main() {
int x;
DBApp d;
d.CreateTable();
 while(true) {
		cout << "\nThis is a simple Database Application " << endl;
		cout << "\nPlease choose what you would like to do" << endl;

		cout << "1. Add Cars the to the warehouse " << endl;
		cout << "2. Delete a Car from the warehouse " << endl;
		cout << "3. Select Cars from the warehouse " << endl;
		cout << "4. Update a Car in the warehouse " << endl;
		cout << "5. Show table of cars " << endl;
		cout << "6. Exit " << endl;
		cin >> x;
		
		if(x == 2) {
			string car_name,table_name;
			cout <<"Enter the name of the table from which the car needs to be deleted " << endl;
			cin >> table_name;
			cout << "Enter the name of the car to be deleted " << endl;
			cin >> car_name;
			d.DeleteCar(car_name,table_name);
			cout << "\n....Returning to the main menu " << endl;
		}
		
		if(x == 5) {
			string name;
			cout << "Enter the name of the table to show " << endl;
			cin >> name;
			d.ShowTable(name);
			cout << "\n....Returning to the main menu " << endl;
		}			
		
		if(x == 6) {
			cout << " \n         Good Bye :) " << endl;
			exit(0);
			//parser.parse("EXIT");
		}	
		else {
			cout << "Invalid Input, Please choose from the list. " << endl;
		}
	}
}
