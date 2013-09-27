#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"
#include "Token.h"

string rel_name;
vector<string> rel_names;
vector<string> create_names;

//Operations storage
vector<string> operators;
vector<Token> operands;

//Attribute storage
vector<string> attr_value;
vector<string> attr_list;
vector<Type> attr_type;

//Literals
vector<Cell> lit_list;

vector<Token> conditions;

Database d;

Token_stream ts;

using namespace std;
//Relation storage
bool identifier(Token_stream& ts, string kind)
{
  Token t = ts.get();
  /*if(t.kind == '8') {
		stringstream ss;
		ss << t.value;
		attr_value.push_back(ss.str());
	}
	else if(t.kind == 'a') {
		attr_value.push_back(t.str);
	}*/
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
	create_names.push_back(t.str);
	if(kind == "rel"){
		rel_name = t.str;
		rel_names.push_back(t.str);
		
	}
	if(kind == "attr"){
		attr_value.push_back(t.str);
	}
	if(kind == "list"){
		attr_list.push_back(t.str);
	}
  //cout << rel_names[0] << endl;
  return true;
}
bool relation_name(Token_stream& ts){
  return identifier(ts, "rel");
}
bool name_check(Token_stream& ts, string input){
  Token t = ts.get();
  if(t.str == input)
    return true;
  ts.putback(t);
  return false;
}
bool operator_check(Token_stream& ts, char c){
  Token t = ts.get();
  if(t.kind == c)
    return true;
  ts.putback(t);
  return false;
}
bool atomic_expr(Token_stream& ts);
bool union_diff_proj(Token_stream& ts){
  Token_stream tscopy(ts);
  bool ret = atomic_expr(ts);
  if(!ret)
    return false;
	Relation rel1;
	if(operator_check(ts, '+'))
	{
		Relation rel1 = d.Copy_table(rel_names.back(), "AtomicLeft");
		rel_names.pop_back();
		if(!atomic_expr(ts))
		{
			ts.copy(tscopy);
			return false;
		}
		Relation rel2 = d.Copy_table(rel_names.back(), "AtomicRight");
		rel_names.pop_back();
		for(int i = 0; i < d.relation.size(); i++)
		{
			if(d.relation[i].name == "AtomicLeft")
			{
				d.relation[i] = rel1;
				break;
			}
		}
		d.Union("AtomicExpression", "AtomicLeft", "AtomicRight");
		rel_names.push_back("AtomicExpression");
		return true;
	}
	else if(operator_check(ts, '-'))
	{
		Relation rel1 = d.Copy_table(rel_names.back(), "AtomicLeft");
		rel_names.pop_back();
		if(!atomic_expr(ts))
		{
			ts.copy(tscopy);
			return false;
		}
		Relation rel2 = d.Copy_table(rel_names.back(), "AtomicRight");
		rel_names.pop_back();
		for(int i = 0; i < d.relation.size(); i++)
		{
			if(d.relation[i].name == "AtomicLeft")
			{
				d.relation[i] = rel1;
				break;
			}
		}
		d.Difference("AtomicExpression", "AtomicLeft", "AtomicRight");
		rel_names.push_back("AtomicExpression");
		return true;
	}
	else if(operator_check(ts, '*'))
	{
		Relation rel1 = d.Copy_table(rel_names.back(), "AtomicLeft");
		rel_names.pop_back();
		if(!atomic_expr(ts))
		{
			ts.copy(tscopy);
			return false;
		}
		Relation rel2 = d.Copy_table(rel_names.back(), "AtomicRight");
		rel_names.pop_back();
		for(int i = 0; i < d.relation.size(); i++)
		{
			if(d.relation[i].name == "AtomicLeft")
			{
				d.relation[i] = rel1;
				break;
			}
		}
		d.Product("AtomicExpression", "AtomicLeft", "AtomicRight");
		rel_names.push_back("AtomicExpression");
		return true;
	}
	else
	{
		ts.copy(tscopy);
		return false;
	}
}

bool attribute_name(Token_stream& ts, string kind)
{
	if(kind == "attr")
		return identifier(ts, kind);
	if(kind == "list")
		return identifier(ts, kind);
}

bool operand(Token_stream& ts)
{
  if(attribute_name(ts, "attr"))
  {
	operands.push_back(Token('a', attr_value.back()));
	attr_value.pop_back();
    return true;
	}
  Token t = ts.get();
  if(t.kind == '8') {
	stringstream ss;
	ss << t.value;
	operands.push_back(t);
    return true;
	}
if(t.kind == '"'){
	operands.push_back(t);
	return true;
}
  ts.putback(t);
  return false;
}

bool op(Token_stream& ts)
{
  Token t = ts.get();
  if(t.kind == '=')
  {
	  operators.push_back(t.str);
	  return true;
  }
  return false;
}

bool condition(Token_stream& ts);

bool comparison(Token_stream& ts)
{  
  return (operand(ts) && op(ts) && operand(ts)) || (ts.get().kind == '(' && condition(ts) && ts.get().kind == ')');
}

bool conjunction(Token_stream& ts)
{
  bool valid = comparison(ts);
  if(!valid)
    return false;
  Token t = ts.get();
  if(t.kind == '&') {
  conditions.push_back(t);
    return valid && comparison(ts);
	}
  ts.putback(t);
  return true;
}

bool condition(Token_stream& ts)
{
	conditions.clear();
  bool valid = conjunction(ts);
  if(!valid)
    return false;
  Token t = ts.get();
  if(t.kind == '|') {
	conditions.push_back(t);
    return valid && conjunction(ts);
	}
  ts.putback(t);
  return true;
}

bool selection(Token_stream& ts)
{
	bool ret = name_check(ts, "select") && ts.get().kind == '(' && condition(ts) && ts.get().kind == ')' && atomic_expr(ts);
	if(ret){//(string attr_name, string condition, string cell_condition, string rel_name)
		//cout << "Select(" << attr_value.back() << ", " << operators.back() << ", " << operands.back() << ", " << ");" << endl;
		Token back = operands.back();
		operands.pop_back();
		if(back.kind == '8')
		{
			stringstream ss;
			ss << back.value;
			d.SelectLiteral( operands.back().str, operators.back(), ss.str(), rel_names.back());
		}
		else if(back.kind == '"')
			d.SelectLiteral( operands.back().str, operators.back(), back.str, rel_names.back());
		else if(back.kind == 'a')
			d.SelectAttribute(operands.back().str, operators.back(), back.str, rel_names.back());
		operators.pop_back();
		operands.pop_back();
		rel_names.pop_back();
		rel_names.push_back("Expression");
		while(!conditions.empty())
		{
			d.Copy_table("Expression", "AtomicRight");
			Token back = operands.back();
			operands.pop_back();
			if(back.kind == '8')
			{
				stringstream ss;
				ss << back.value;
				d.SelectLiteral( operands.back().str, operators.back(), ss.str(), rel_names.back());
			}
			else if(back.kind == '"')
				d.SelectLiteral( operands.back().str, operators.back(), back.str, rel_names.back());
			else if(back.kind == 'a')
				d.SelectAttribute(operands.back().str, operators.back(), back.str, rel_names.back());
			operators.pop_back();
			operands.pop_back();
			rel_names.pop_back();
			rel_names.push_back("Expression");
			if(conditions.back().kind == '|')
			{
				d.Copy_table("Expression", "AtomicLeft");
				d.Union("Expression", "AtomicLeft", "AtomicRight");
			}
			else if(conditions.back().kind == '&')
			{
				d.Copy_table("Expression", "AtomicLeft");
				d.Intersection("Expression", "AtomicLeft", "AtomicRight");
			}
			conditions.pop_back();
		}
	}
  return ret;
}

bool attribute_list(Token_stream& ts)
{
  if(!attribute_name(ts, "list"))
    return false;
  while(attribute_name(ts, "list")) {}
  return true;
}

bool projection(Token_stream& ts)
{
  bool ret = name_check(ts, "project") && ts.get().kind == '(' && attribute_list(ts) && ts.get().kind == ')' && atomic_expr(ts);
  if(ret){//Project(vector<string> attr_name, string rel_name);
	try
	{
		d.Project(attr_list, rel_names.back());
	}
	catch(string str)
	{
		cerr << str << endl;
		return false;
	}
	attr_list.clear();
	rel_names.pop_back();
	rel_names.push_back("Expression");
  }
  return ret;
}

bool rename(Token_stream& ts)
{
	bool ret = name_check(ts, "rename") && ts.get().kind == '(' && attribute_list(ts) && ts.get().kind == ')' && atomic_expr(ts);
	if(ret)
	{
		d.Rename("Expression", rel_names.back(), attr_list);
		rel_names.pop_back();
		rel_names.push_back("Expression");
	}
  return ret;
}

bool expr(Token_stream& ts){
  //return U_diff_prod(ts);
  bool ret = selection(ts) || projection(ts) || rename(ts) || union_diff_proj(ts);
  //cout << "expr2: " << ret << endl;
  ret = ret || atomic_expr(ts);
  if(ret)
  {
	d.Copy_table(rel_names.back(), "Expression");
	rel_names.pop_back();
	rel_names.push_back("Expression");
  }
  //cout << "expr3: " << ret << endl;
  return ret;
  //return atomic_expr(ts) || selection(ts) || projection(ts) || renaming(ts) || Union(ts) || difference(ts) || product(ts);
}
bool atomic_expr(Token_stream& ts){
  if(relation_name(ts))
  {
	//d.Copy_table(rel_names.back(), "AtomicExpression");
	//rel_names.pop_back();
	//rel_names.push_back("AtomicExpression");
    return true;
	}
  Token t = ts.get();
  //cout << "atomic2: " << t.kind << endl;
  if(t.kind != '(')
  {
    ts.putback(t);
    return false;
  }
  t = ts.get();
  ts.putback(t);
  bool ret = expr(ts) && ts.get().kind == ')';
  if(ret)
  {
	d.Copy_table(rel_names.back(), "AtomicExpression");
	rel_names.pop_back();
	rel_names.push_back("AtomicExpression");
    return true;
  }
  return false;
}
bool literal_list(Token_stream& ts){
  Token t = ts.get();
  if(t.kind != '(')
  {
    ts.putback(t);
    return false;
  }
  //vector<Token> list;
  Token literal = ts.get();
  bool valid = false;
  while(literal.kind == '"' || literal.kind == '8')
  {
    //list.push_back(literal);
	if(literal.kind == '"')
		lit_list.push_back(literal.str);
	if(literal.kind == '8'){
		stringstream ss;
		ss << literal.value;
		lit_list.push_back(ss.str());
	}
    literal = ts.get();
    valid = true;
  }
  ts.putback(literal);
  if(!valid){
	lit_list.clear();
	return false;
	}
  t = ts.get();
  if(t.kind != ')')
  {
	lit_list.clear();
    ts.putback(t);
    return false;
  }
  return true;
}

bool literal(Token_stream& ts)
{
  Token t = ts.get();
  if(t.kind == '8'){
	stringstream ss;
	ss << t.value;
	lit_list.push_back(ss.str());
    return true;
	}
	else if(t.kind == '"')
	{
		lit_list.push_back(t.str);
		return true;
	}
  ts.putback(t);
  return false;
}

bool type(Token_stream& ts)
{
  if(name_check(ts, "VARCHAR") && ts.get().kind == '(' && ts.get().kind == '8' && ts.get().kind == ')')
  {
	attr_type.push_back(STRING);
	return true;
  }
   else if (name_check(ts, "INTEGER")) {
	attr_type.push_back(INT); 
		return true;
	}
	return false;
}

bool typed_attribute_list(Token_stream& ts)
{
  bool valid = attribute_name(ts, "attr") && type(ts);
  if(!valid)
    return false;
  while(attribute_name(ts, "attr") && type(ts)) {}
  return true;
}

bool insert(Token_stream& ts){
  bool valid = name_check(ts, "INSERT") && name_check(ts, "INTO") && relation_name(ts) && name_check(ts, "VALUES") && name_check(ts, "FROM");
  if(!valid)
    return false;
  if(literal_list(ts)){
	d.Insert(rel_names[0], lit_list);
	return true;
	}
	bool ret = name_check(ts, "RELATION") && expr(ts);
	if(ret){
		for(int i = 0; i < d.relation.size(); i++)
		{
			if(d.relation[i].name == rel_names.back())
			{
				for(int j = 0; j < d.relation[i].getRowSize(); j++)
				{
					vector<Cell> row = d.relation[i].getRow(j);
					d.Insert(rel_names[0], row);
				}
			}
		}
	}
  return ret;
}

bool _delete(Token_stream& ts)
{
	if(name_check(ts, "DELETE") && name_check(ts, "FROM") && relation_name(ts) && name_check(ts, "WHERE") && (ts.get().kind == '(' && condition(ts) && ts.get().kind == ')')) {
	string rel_name = rel_names.back();
	Token back = operands.back();
	operands.pop_back();
	if(back.kind == '8')
	{
		stringstream ss;
		ss << back.value;
		d.SelectLiteral( operands.back().str, operators.back(), ss.str(), rel_names.back());
	}
	else if(back.kind == '"')
		d.SelectLiteral( operands.back().str, operators.back(), back.str, rel_names.back());
	else if(back.kind == 'a')
			d.SelectAttribute(operands.back().str, operators.back(), back.str, rel_names.back());
	operators.pop_back();
	operands.pop_back();
	rel_names.pop_back();
	rel_names.push_back("Expression");
	while(!conditions.empty())
	{
		d.Copy_table("Expression", "AtomicRight");
		Token back = operands.back();
		operands.pop_back();
		if(back.kind == '8')
		{
			stringstream ss;
			ss << back.value;
			d.SelectLiteral( operands.back().str, operators.back(), ss.str(), rel_names.back());
		}
		else if(back.kind == '"')
			d.SelectLiteral( operands.back().str, operators.back(), back.str, rel_names.back());
		else if(back.kind == 'a')
				d.SelectAttribute(operands.back().str, operators.back(), back.str, rel_names.back());
		operators.pop_back();
		operands.pop_back();
		rel_names.pop_back();
		rel_names.push_back("Expression");
		if(conditions.back().kind == '|')
		{
			d.Copy_table("Expression", "AtomicLeft");
			d.Union("Expression", "AtomicLeft", "AtomicRight");
		}
		else if(conditions.back().kind == '&')
		{
			d.Copy_table("Expression", "AtomicLeft");
			d.Intersection("Expression", "AtomicLeft", "AtomicRight");
		}
		conditions.pop_back();
	}
	d.Copy_table(rel_name, "AtomicLeft");
	d.Copy_table("Expression", "AtomicRight");
	d.Difference(rel_name, "AtomicLeft", "AtomicRight");
		return true;
	}
	return false;
}

bool create(Token_stream& ts)
{
  if(name_check(ts, "CREATE") && name_check(ts, "TABLE") && relation_name(ts) && (ts.get().kind == '(' && typed_attribute_list(ts) && ts.get().kind == ')')
    && name_check(ts, "PRIMARY") && name_check(ts, "KEY") && (ts.get().kind == '(' && attribute_list(ts) && ts.get().kind == ')')) {

	d.Create(create_names[0]);
		for(int i=0;i<attr_type.size(); i++) {	 
		d.AddColumn(create_names[0], Header(create_names[i+1], attr_type[i]));		
	}
	return true;
}
	return false;
}

bool update(Token_stream& ts)
{

  bool ret = name_check(ts, "UPDATE") && relation_name(ts) && name_check(ts, "SET");
  if(!ret)
    return false;
	string rel_name = rel_names.back();
  while(attribute_name(ts, "list") && ts.get().str == "=" && literal(ts)) {}
  ret = ret && name_check(ts, "WHERE") && condition(ts);
  if(ret)
  {//void Update(string rel_name, string attr_name, string literal, string condition_attr, string condition, string condition_literal);
	Token back = operands.back();
	operands.pop_back();
	if(back.kind == '8')
	{
		stringstream ss;
		ss << back.value;
		d.SelectLiteral( operands.back().str, operators.back(), ss.str(), rel_names.back());
	}
	else if(back.kind == '"')
		d.SelectLiteral( operands.back().str, operators.back(), back.str, rel_names.back());
	else if(back.kind == 'a')
		d.SelectAttribute(operands.back().str, operators.back(), back.str, rel_names.back());
	operators.pop_back();
	operands.pop_back();
	rel_names.pop_back();
	rel_names.push_back("Expression");
	while(!conditions.empty())
	{
		d.Copy_table("Expression", "AtomicRight");
		Token back = operands.back();
		operands.pop_back();
		if(back.kind == '8')
		{
			stringstream ss;
			ss << back.value;
			d.SelectLiteral( operands.back().str, operators.back(), ss.str(), rel_names.back());
		}
		else if(back.kind == '"')
			d.SelectLiteral( operands.back().str, operators.back(), back.str, rel_names.back());
		else if(back.kind == 'a')
			d.SelectAttribute(operands.back().str, operators.back(), back.str, rel_names.back());
		operators.pop_back();
		operands.pop_back();
		rel_names.pop_back();
		rel_names.push_back("Expression");
		if(conditions.back().kind == '|')
		{
			d.Copy_table("Expression", "AtomicLeft");
			d.Union("Expression", "AtomicLeft", "AtomicRight");
		}
		else if(conditions.back().kind == '&')
		{
			d.Copy_table("Expression", "AtomicLeft");
			d.Intersection("Expression", "AtomicLeft", "AtomicRight");
		}
		conditions.pop_back();
	}
	for(int i = 0; i < lit_list.size(); i++)
	{
		d.Copy_table(rel_name, "AtomicExpression");
		d.Difference(rel_name, "AtomicExpression", "Expression");
		d.Update("Expression", attr_list[i], lit_list[i], "Expression");
		d.Copy_table(rel_name, "AtomicExpression");
		d.Union(rel_name, "AtomicExpression", "Expression");
	}
}
  return ret;
}

bool show(Token_stream& ts)
{
	if( name_check(ts, "SHOW") && atomic_expr(ts))
	{
		try
		{
			d.Show(rel_names[0]);
		}
		catch(string s)
		{
			cerr << s << endl;
			return false;
		}
		return true;
	}
}
bool exit(Token_stream&ts){
	if(name_check(ts, "EXIT"))
		exit(0);
	else
		return false;
}
bool write(Token_stream& ts){
  if(name_check(ts, "WRITE") && relation_name(ts)) {
	d.Write(rel_names[0] );
	rel_names.clear();
	return true;
	}
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
  if(!ret)
	return false;
  string rel1 = rel_names.back();
  ret = ret && expr(ts);
  d.Copy_table("Expression", rel1);
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
  return (command(ts) || query(ts)) && termination (ts);
}

void Parser::parse(string line)
{
	cout << "Input line: " << line << endl;
	Token_stream ts;
	ts.ss.clear();
	ts.ss.str("");
	ts.ss << line;
	rel_names.clear();
	attr_type.clear();
	attr_list.clear();
	operators.clear();
	attr_value.clear();
	lit_list.clear();
	conditions.clear();
	create_names.clear();
	operands.clear();
	if(!program(ts))
	  cout << "THIS IS WRONG" << endl;
	else
	  cout << " VALID SYNTAX " << endl;
}

/*int main(){
	d.Create("a");
	d.AddColumn("a", Header("Str1", STRING));
	d.AddColumn("a", Header("Str2", INT));
	
	d.Create("b");
	d.AddColumn("b", Header("Str1", STRING));
	d.AddColumn("b", Header("Str2", INT));
	
	vector<Cell> cells;
	cells.push_back("Blah");
	cells.push_back("2");
	d.Insert("a", cells);
	cells.clear();
	cells.push_back("Blah2");
	cells.push_back("3");
	d.Insert("b", cells);
  ifstream infile("good_inputs.txt");
  string line;
  while(getline(cin, line))
  {
    cout << "Input line: " << line << endl;
    Token_stream ts;
    ts.ss.clear();
    ts.ss.str("");
    ts.ss << line;
    if(!program(ts))
      cout << "THIS IS WRONG" << endl;
    else
      cout << " VALID SYNTAX " << endl;
	rel_names.clear();
	attr_type.clear();
	attr_list.clear();
	operators.clear();
	attr_value.clear();
	lit_list.clear();
	conditions.clear();
	create_names.clear();
	operands.clear();
  }

  return 0;
}*/


