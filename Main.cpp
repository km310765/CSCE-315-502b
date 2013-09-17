#include <iostream>
#include <fstream>
#include "Relation.h"
#include "Database.h"
#include "Attribute.h"
#include "Token.h"

using namespace std;

Token_stream ts;
bool identifier(Token_stream& ts)
{
	//cout << "identifier" << endl;
	Token t = ts.get();
	//cout << t.kind << ", " << t.value << ", " << t.str << endl;
	if(t.kind != 'a')
	{
		ts.putback(t);
		return false;
	}
	if(t.str == "select" || t.str == "project" || t.str == "rename" || t.str == "WHERE")
	{
		ts.putback(t);
		return false;
	}
	//cout << "REL: " << t.str << endl;
	return true;
}
bool relation_name(Token_stream& ts){
	return identifier(ts);
}
bool parenthesis(Token_stream& ts){
	//cout << "parenth" << endl;
	vector<Token> tt;
	Token t = ts.get();
	if (t.kind == ' '){
		t = ts.get();
		tt.push_back(t);
	}
	else
		tt.push_back(t);
	if (t.kind != '('){
		while(!tt.empty()){
			ts.putback(tt.back());
			tt.pop_back();
		}
		return false;
	}
	else{
		t = ts.get();
		tt.push_back(t);
	}
	while (t.kind != ')'){
		if(t.kind == ';'){
			while(!tt.empty()){
				//cout << tt.back().kind;
				ts.putback(tt.back());
				tt.pop_back();
			}
			cout << "parenth false" << endl;
			return false;
		}
		t = ts.get();
		tt.push_back(t);
	}
	while(!tt.empty()){
		cout << "o: " << tt.back().kind << endl;
		
		if(tt.back().kind != '(' && tt.back().kind != ')'){
			ts.putback(tt.back());
			//cout << "p: " << tt.back().kind << endl;
		}
		tt.pop_back();
	}
	return true;
}
bool name_check(Token_stream& ts, string input){
	Token t = ts.get();
	//cout << "Name: " << t.kind << ", " << t.value << ", " << t.str << " to " << input << endl;
	if(t.str == input)
		return true;
	ts.putback(t);
	return false;
}
bool operator_check(Token_stream& ts, char c){
	Token t = ts.get();
	//cout << "Operator: " << t.kind << " to " << c << endl;
	if(t.kind == c)
		return true;
	ts.putback(t);
	return false;
}
bool atomic_expr(Token_stream& ts);
bool union_diff_proj(Token_stream& ts){
	//cout << "union" << endl;
	Token_stream tscopy(ts);
	bool ret = atomic_expr(ts);
	//cout << "union2: " << ret << endl;
	if(!ret)
		return false;
	ret = ret && (operator_check(ts, '+') || operator_check(ts, '-') || operator_check(ts, '*')) && atomic_expr(ts);
	if(!ret)
	{
		ts.copy(tscopy);
		return false;
	}
	return true;
}

bool attribute_name(Token_stream& ts)
{
	//cout << "attribute name" << endl;
	return identifier(ts);
}

bool operand(Token_stream& ts)
{
	//cout << "operand" << endl;
	if(attribute_name(ts))
		return true;
	Token t = ts.get();
	if(t.kind == '8' || t.kind == '"')
		return true;
	ts.putback(t);
	return false;
}

bool op(Token_stream& ts)
{
	Token t = ts.get();
	return t.kind == '=';
}

bool comparison(Token_stream& ts)
{
	//cout << "comparison" << endl;
	return (operand(ts) && op(ts) && operand(ts)) || (ts.get().kind == '(' && op(ts) && ts.get().kind == ')');
}

bool conjunction(Token_stream& ts)
{
	//cout << "conjunction" << endl;
	bool valid = comparison(ts);
	if(!valid)
		return false;
	Token t = ts.get();
	if(t.kind == '&')
		return valid && comparison(ts);
	ts.putback(t);
	return true;
}

bool condition(Token_stream& ts)
{
	//cout << "condition" << endl;
	bool valid = conjunction(ts);
	if(!valid)
		return false;
	Token t = ts.get();
	if(t.kind == '|')
		return valid && conjunction(ts);
	ts.putback(t);
	return true;
}

bool selection(Token_stream& ts)
{
	return name_check(ts, "select") && ts.get().kind == '(' && condition(ts) && ts.get().kind == ')' && atomic_expr(ts);
}

bool attribute_list(Token_stream& ts)
{
	if(!attribute_name(ts))
		return false;
	while(attribute_name(ts)) {}
	return true;
}

bool projection(Token_stream& ts)
{
	return name_check(ts, "project") && ts.get().kind == '(' && attribute_list(ts) && ts.get().kind == ')' && atomic_expr(ts);
}

bool rename(Token_stream& ts)
{
	return name_check(ts, "rename") && ts.get().kind == '(' && attribute_list(ts) && ts.get().kind == ')' && atomic_expr(ts);
}

bool expr(Token_stream& ts){
	//cout << "expr" << endl;
	//return U_diff_prod(ts);
	bool ret = selection(ts) || projection(ts) || rename(ts) || union_diff_proj(ts);
	//cout << "expr2: " << ret << endl;
	ret = ret || atomic_expr(ts);
	//cout << "expr3: " << ret << endl;
	return ret;
	//return atomic_expr(ts) || selection(ts) || projection(ts) || renaming(ts) || Union(ts) || difference(ts) || product(ts);
}
bool atomic_expr(Token_stream& ts){
	//cout << "atomic" << endl;
	if(relation_name(ts))
		return true;
	Token t = ts.get();
	//cout << "atomic2: " << t.kind << endl;
	if(t.kind != '(')
	{
		ts.putback(t);
		return false;
	}
	t = ts.get();
	//cout << "next: " << t.kind << ", " << t.value << ", " << t.str << endl;
	ts.putback(t);
	return expr(ts) && ts.get().kind == ')';
}
bool literal_list(Token_stream& ts){
	Token t = ts.get();
	if(t.kind != '(')
	{
		ts.putback(t);
		return false;
	}
	vector<Token> list;
	Token literal = ts.get();
	bool valid = false;
	while(literal.kind == '"' || literal.kind == '8')
	{
		list.push_back(literal);
		literal = ts.get();
		valid = true;
	}
	ts.putback(literal);
	if(!valid)
		return false;
	t = ts.get();
	if(t.kind != ')')
	{
		ts.putback(t);
		return false;
	}
	return true;
}

bool literal(Token_stream& ts)
{
	Token t = ts.get();
	if(t.kind == '8' || t.kind == '"')
		return true;
	ts.putback(t);
	return false;
}

bool type(Token_stream& ts)
{
	return (name_check(ts, "VARCHAR") && ts.get().kind == '(' && ts.get().kind == '8' && ts.get().kind == ')') || name_check(ts, "INTEGER");
}

bool typed_attribute_list(Token_stream& ts)
{
	bool valid = attribute_name(ts) && type(ts);
	if(!valid)
		return false;
	while(attribute_name(ts) && type(ts)) {}
	return true;
}

bool insert(Token_stream& ts){
	bool valid = name_check(ts, "INSERT") && name_check(ts, "INTO") && relation_name(ts) && name_check(ts, "VALUES") && name_check(ts, "FROM");
	if(!valid)
		return false;
	if(literal_list(ts))
		return true;
	return name_check(ts, "RELATION") && expr(ts);
}

bool _delete(Token_stream& ts)
{
	return name_check(ts, "DELETE") && name_check(ts, "FROM") && relation_name(ts) && name_check(ts, "WHERE") && (ts.get().kind == '(' && condition(ts) && ts.get().kind == ')');
}

bool create(Token_stream& ts)
{
	return name_check(ts, "CREATE") && name_check(ts, "TABLE") && relation_name(ts) && (ts.get().kind == '(' && typed_attribute_list(ts) && ts.get().kind == ')')
		&& name_check(ts, "PRIMARY") && name_check(ts, "KEY") && (ts.get().kind == '(' && attribute_list(ts) && ts.get().kind == ')');
}

bool update(Token_stream& ts)
{
	bool ret = name_check(ts, "UPDATE") && relation_name(ts) && name_check(ts, "SET") && attribute_name(ts) && ts.get().str == "=" && literal(ts);
	if(!ret)
		return false;
	while(attribute_name(ts) && ts.get().str == "=" && literal(ts)) {}
	ret = ret && name_check(ts, "WHERE") && condition(ts);
	return ret;
}

bool show(Token_stream& ts){
	return name_check(ts, "SHOW") && atomic_expr(ts);
}
bool exit(Token_stream&ts){
	return name_check(ts, "EXIT");
}
bool write(Token_stream& ts){
	return name_check(ts, "WRITE") && relation_name(ts);
}
bool close(Token_stream& ts){
	return name_check(ts, "CLOSE") && relation_name(ts);
}
bool open(Token_stream& ts){
	return name_check(ts, "OPEN") && relation_name(ts);
}
bool command(Token_stream& ts)
{
	return open(ts) || close(ts) || write(ts) || exit(ts) || show(ts) || create(ts) || update(ts) || insert(ts) || _delete(ts);
}
bool query(Token_stream& ts)
{
	bool ret = relation_name(ts) && operator_check(ts, ':');
	//cout << "query1: " << ret << endl;
	ret = ret && expr(ts);
	//cout << "query2: " << ret << endl;
	return ret;
}
bool termination(Token_stream& ts){
	Token t = ts.get();
	if (t.kind == ';'){
		return true;
	}
	return false;
}
bool program(Token_stream& ts)
{
	//return query(ts) && termination(ts);
	return (command(ts) || query(ts)) && termination (ts);
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
  ifstream infile("good_inputs.txt");
  string line;
  while(getline(infile, line))
  {
	cout << "Input line: " << line << endl;
	Token_stream ts;
	  ts.ss.clear();
	  ts.ss.str("");
	  ts.ss << line;
	  //bool valid = true;
	  if(!program(ts))
		cout << "THIS IS WRONG" << endl;
	}
	/*getline(cin, line);
	cout << "Input line: " << line << endl;
	Token_stream ts;
	  ts.ss.clear();
	  ts.ss.str("");
	  ts.ss << line;
  cout << program(ts) << endl;*/
  return 0;
}
