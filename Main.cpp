#include <iostream>
#include "Relation.h"
#include "Database.h"
#include "Attribute.h"

using namespace std;

int main(){
  Database d;
  /*d.Create("Cars");
  d.AddColumn("Cars",Header("Make",STRING));
  d.AddColumn("Cars",Header("Year",INT));
  d.AddColumn("Cars",Header("Model",STRING));
  vector<Cell> cell;
  cell.push_back("Audi");
  cell.push_back("2013");
  cell.push_back("R8");
  d.Insert("Cars", cell);
  cell.clear();
  cell.push_back("Audi2");
  cell.push_back("2013222222");
  cell.push_back("R8222222");
  d.Insert("Cars", cell);

  d.Create("LOL");
  d.AddColumn("LOL",Header("Hello",STRING));
  d.AddColumn("LOL",Header("Bello",STRING));
  d.AddColumn("LOL",Header("haha",STRING));
  
  vector<Cell> cell1;
  vector<Cell> cell2;
  cell1.push_back("lol");
  cell1.push_back("lol2");
  cell1.push_back("lol3");
  cell2.push_back("Audi");
  cell2.push_back("2013");
  cell2.push_back("R0");
  d.Insert("LOL",cell2);
  d.Insert("LOL",cell1);

  d.Show("Cars");
  d.Rename("Cars","sfsd","Model");
  //d.Delete_attr("Cars","Audi");
  d.Delete_attr("Cars","Joinks");
  d.Delete_attr("haha","Audi");
  d.Show("Cars");
  d.Show("LOL");
  //d.Union("Cars","LOL");
  //d.AddColumn("CarsLOLUnion",Header("Union",STRING));
  //d.Show("CarsLOLUnion");
  d.Show("kjlk");
  d.Show("Cars");
  d.Write("Cars");*/
  
  d.Create("Table1");
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
  d.Show("Table3");
  return 0;
}
