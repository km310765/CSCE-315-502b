#include <iostream>
#include "Relation.h"
#include "Database.h"
#include "Attribute.h"
#include "Token.h"

using namespace std;

int main(){
  /*d.Create("Table1");
  d.AddColumn("Table1", Header("Str1", STRING));
  d.AddColumn("Table1", Header("Str2", STRING));
  d.AddColumn("Table1", Header("Str3", STRING));
  vector<Cell> cell;
  cell.push_back("1.1");
  cell.push_back("1.2");
  cell.push_back("1.3");
  d.Insert("Table1", cell);
  cell.clear();
  cell.push_back("1.4");
  cell.push_back("1.5");
  cell.push_back("1.6");
  d.Insert("Table1", cell);
  cell.clear();
  cell.push_back("1.7");
  cell.push_back("1.8");
  cell.push_back("1.9");
  d.Insert("Table1", cell);
  
  d.Create("Table2");
  d.AddColumn("Table2", Header("Str1", STRING));
  d.AddColumn("Table2", Header("Str2", STRING));
  d.AddColumn("Table2", Header("Str3", STRING));
  cell.clear();
  cell.push_back("2.1");
  cell.push_back("2.2");
  cell.push_back("2.3");
  d.Insert("Table2", cell);
  cell.clear();
  cell.push_back("2.4");
  cell.push_back("2.5");
  cell.push_back("2.6");
  d.Insert("Table2", cell);
  cell.clear();
  cell.push_back("2.7");
  cell.push_back("2.8");
  cell.push_back("2.9");
  d.Insert("Table2", cell);
  
  d.Create("Table3");
  d.AddColumn("Table3", Header("Str1", STRING));
  d.AddColumn("Table3", Header("Str2", STRING));
  d.AddColumn("Table3", Header("Str3", STRING));
  cell.clear();
  cell.push_back("2.1");
  cell.push_back("2.2");
  cell.push_back("2.3");
  d.Insert("Table3", cell);
  cell.clear();
  cell.push_back("3.4");
  cell.push_back("3.5");
  cell.push_back("3.6");
  d.Insert("Table3", cell);
  cell.clear();
  cell.push_back("2.7");
  cell.push_back("2.8");
  cell.push_back("2.9");
  d.Insert("Table3", cell);
  
  d.Show("Table3");
  d.Update("Table3", "Str2", "6", "Str3", "==", "2.9");
  d.Show("Table3");*/
  Token_stream ts;
  ts.ss.clear();
  ts.ss.str("");
  cout << "Input line: ";
  string line;
  getline(cin, line);
  ts.ss << line;
  while(true)
  {
	Token t = ts.get();
	cout << t.kind << ", " << t.str << ", " << t.value << endl;
	if(t.kind == ';')
		break;
  }
  return 0;
}
