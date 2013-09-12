#include <iostream>
#include "Relation.h"
#include "Database.h"
#include "Attribute.h"
#include "Token.h"

using namespace std;

Token_stream ts;

bool identifier(Token_stream& ts)
{
	string identifier_name = "";
	Token t = ts.get();
	if(t.kind != 'a')
	{
		ts.putback(t);
		return false;
	}
	identifier_name += t.value;
	t = ts.get();
	while(t.kind == 'a' || t.kind == '0')
	{
		identifier_name += t.value;
		t = ts.get();
	}
	ts.putback(t);
	cout << identifier_name << endl;
	return true;
}

bool relation_name(Token_stream& ts)
{
	return identifier(ts);
}

bool expr(Token_stream& ts)
{
	return true;
}

bool query(Token_stream& ts)
{
	if(!relation_name(ts))
		return false;
	Token next = ts.get();
	if(next.kind != ':')
	{
		ts.putback(next);
		return false;
	}
	if(!expr(ts))
		return false;
	return true;
}

bool command(Token_stream& ts)
{
	return false;
}

bool valid_line(Token_stream& ts)
{
	return query(ts) || command(ts);
}

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
  bool valid = true;
  while(true)
  {
	Token t = ts.get();
	//cout << t.kind << ", " << t.value << endl;
	if(t.kind == ';')
		//break;
		return 0;
	ts.putback(t);
	cout << valid_line(ts) << endl;
  }
  return 0;
}
