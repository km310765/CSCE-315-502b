#ifndef DBAPP_H
#define DBAPP_H

#include <iostream>
#include <stdlib.h>
#include "Parser.h"

using namespace std;

class DBApp {
	Parser p;
	
	public:
		void Close();
		void CreateTable();
		void AddNewCar();
		void DeleteCar(string car_name, string table_name);
		void UpdateCar(string table_name);
		void ShowCarWithID(string table_name);
		void ShowTable(string tablename);
		void AddCar(string table_name);
		void Exit();
		void Diff();
		void UnsoldCars();
		void Customers_car_info();
		void Create_Customer_table();
		void SearchCarsByName();
		void ShowCustomerNames();
		void AddShipment();
		void Write(string table_name);
		void Open(string table_name);
};

#endif
