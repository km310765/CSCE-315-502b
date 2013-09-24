#include <iostream>
#include <fstream>
#include "Database.h"
#include "Token.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
//Relation storage
string rel_name;
vector<string> rel_names;

//Operations storage
vector<string> operators;
vector<string> operands;

//Attribute storage
vector<string> attr_value;
vector<string> attr_list;
vector<Type> attr_type;

//Literals
string _literal;
vector<Cell> literal_list;

//Database
Database d;

Token_stream ts;
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
    }
    tt.pop_back();
  }
  return true;
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
  ret = ret && (operator_check(ts, '+') || operator_check(ts, '-') || operator_check(ts, '*')) && atomic_expr(ts);
  if(!ret)
  {
    ts.copy(tscopy);
    return false;
  }
  return true;
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
    return true;
  Token t = ts.get();
  if(t.kind == '8') {
	operands.push_back(t.str);
    return true;
	}
if(t.kind == '"'){
	operands.push_back(t.value);
	return true;
}
  ts.putback(t);
  return false;
}

bool op(Token_stream& ts)
{
  Token t = ts.get();
  operators.push_back(t.str);
  if(t.kind == '='){
	operators.push_back(t.str);
	return true;
  }
  return false;
}

bool comparison(Token_stream& ts)
{  
  return (operand(ts) && op(ts) && operand(ts)) || (ts.get().kind == '(' && op(ts) && ts.get().kind == ')');
}

bool conjunction(Token_stream& ts)
{
  bool valid = comparison(ts);
  if(!valid)
    return false;
  Token t = ts.get();
  if(t.kind == '&') {
    return valid && comparison(ts);
	}
  ts.putback(t);
  return true;
}

bool condition(Token_stream& ts)
{
  bool valid = conjunction(ts);
  if(!valid)
    return false;
  Token t = ts.get();
  if(t.kind == '|') {
    return valid && conjunction(ts);
	}
  ts.putback(t);
  return true;
}

bool selection(Token_stream& ts)
{
	bool ret = name_check(ts, "select") && ts.get().kind == '(' && condition(ts) && ts.get().kind == ')' && atomic_expr(ts);
	if(ret){//(string attr_name, string condition, string cell_condition, string rel_name)
		d.Select( attr_value.back(), operators.back(), operands.back(), rel_names.back());
		attr_value.pop_back();
		operators.pop_back();
		operands.pop_back();
		rel_names.pop_back();
		rel_names.push_back("Expression");
	}
  return ret
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
	d.Project(attr_list, rel_names.back());
	attr_list.clear();
	rel_names.pop_back();
  }
  return ret;
}

bool rename(Token_stream& ts)
{
  return name_check(ts, "rename") && ts.get().kind == '(' && attribute_list(ts) && ts.get().kind == ')' && atomic_expr(ts);
  return name_check(ts, "rename") && ts.get().kind == '(' && attribute_list(ts) && ts.get().kind == ')' && atomic_expr(ts);
}

bool expr(Token_stream& ts){
  //return U_diff_prod(ts);
  bool ret = selection(ts) || projection(ts) || rename(ts) || union_diff_proj(ts);
  //cout << "expr2: " << ret << endl;
  ret = ret || atomic_expr(ts);
  //cout << "expr3: " << ret << endl;
  return ret;
  //return atomic_expr(ts) || selection(ts) || projection(ts) || renaming(ts) || Union(ts) || difference(ts) || product(ts);
}
bool atomic_expr(Token_stream& ts){
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
  //vector<Token> list;
  Token literal = ts.get();
  bool valid = false;
  while(literal.kind == '"' || literal.kind == '8')
  {
    //list.push_back(literal);
	if(literal.kind == '"')
		literal_list.push_back(literal.str);
	if(literal.kind == '8'){
		stringstream ss;
		ss << literal.value;
		literal_list.push_back(ss.str());
	}
    literal = ts.get();
    valid = true;
  }
  ts.putback(literal);
  if(!valid){
	literal_list.clear();
	return false;
	}
  t = ts.get();
  if(t.kind != ')')
  {
	literal_list.clear();
    ts.putback(t);
    return false;
  }
  return true;
}

bool literal(Token_stream& ts)
{
  Token t = ts.get();
  if(t.kind == '8' || t.kind == '"'){
	s_literal = t.str;
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
}

bool typed_attribute_list(Token_stream& ts)
{
	cout << "typed attribute list " << endl;
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
	d.Insert(rel_names[0], literal_list);
	return true;
	}
	bool ret = name_check(ts, "RELATION") && expr(ts)
	if(ret){
		vector<string> one_rel;
		one_rel.push_back(rel_names.back());
		d.Insert(rel_names[0], one_rel);
	}
  return ret;
}

bool _delete(Token_stream& ts)
{
	if(name_check(ts, "DELETE") && name_check(ts, "FROM") && relation_name(ts) && name_check(ts, "WHERE") && (ts.get().kind == '(' && condition(ts) && ts.get().kind == ')')) {
		string s = attr_value[attr_value.size()-1];
		d.Delete(rel_names[0], rel_names[1], operators[0], s);
		attr_value.clear();
		rel_names.clear();
		operators.clear();
		return true;
	}
}

bool create(Token_stream& ts)
{
  if(name_check(ts, "CREATE") && name_check(ts, "TABLE") && relation_name(ts) && (ts.get().kind == '(' && typed_attribute_list(ts) && ts.get().kind == ')')
    && name_check(ts, "PRIMARY") && name_check(ts, "KEY") && (ts.get().kind == '(' && attribute_list(ts) && ts.get().kind == ')')) {
	
	d.Create(rel_names[0]);
		for(int i=0;i<attr_type.size(); i++) {	 
		d.AddColumn(rel_names[0], Header(rel_names[i+1], attr_type[i]));		
	}
	attr_type.clear();
	return true;
}
}

bool update(Token_stream& ts)
{

  bool ret = name_check(ts, "UPDATE") && relation_name(ts) && name_check(ts, "SET") && attribute_name(ts, "attr") && ts.get().str == "=" && literal(ts);
  if(!ret)
    return false;
  while(attribute_name(ts, "attr") && ts.get().str == "=" && literal(ts)) {}
  ret = ret && name_check(ts, "WHERE") && condition(ts);
  if(ret)//void Update(string rel_name, string attr_name, string literal, string condition_attr, string condition, string condition_literal);
	d.Update(rel_names.back(), attr_value[attr_value.size()-2], s_literal, attr_value.back(), operators.back(), "=");  
	rel_names.pop_back();
	attr_value.pop_back(); 
	attr_value.pop_back();
	operators.pop_back();
	s_literal = "";
  return ret;
}

bool show(Token_stream& ts){
  
  if( name_check(ts, "SHOW") && atomic_expr(ts)) {
	d.Show(rel_names[0]);
	rel_names.clear();
	return true;
	}
}
bool exit(Token_stream&ts){
  return name_check(ts, "EXIT");
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
  return (command(ts) || query(ts)) && termination (ts);
}
int main(){
  ifstream infile("good_inputs.txt");
  string line;
  while(getline(infile, line))
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
  } 
  
  // Token_stream ts;
  // ts.ss.clear();
  // ts.ss.str("");
  // cout << "Input line: ";
  // string line;
  // getline(cin, line);
  // ts.ss << line;
  // while(true)
  // {
	// Token t = ts.get();
	// cout << t.kind << ", " << t.str << ", " << t.value << endl;
	// if(t.kind == ';')
		// break;
  // }
  return 0;
}

  


