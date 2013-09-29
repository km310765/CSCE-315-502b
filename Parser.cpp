#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"
#include "Token.h"

Type type;
string relation_name;
string attribute_name;
string identifier;
string literal;
string op;
Token operand;
Cell typed_literal;
vector<Attribute> typed_attribute_list;
vector<Cell> typed_literal_list;
vector<Token> comparison_operands;
vector<string> comparison_ops;
vector<string> attribute_list;
vector<string> literal_list;
vector<string> conjunction;
Relation* expr;
Relation* atomic_expr;
Relation* selection;
Relation* projection;
Relation* __delete;
Relation* update;
Relation* __rename;
Relation* _union;
Relation* difference;
Relation* product;
Relation* create;
Relation* insert;

Database d;
Token_stream ts;

using namespace std;

bool p_identifier(Token_stream& ts)
{
	Token t = ts.get();
	if(t.kind != 'a' || t.str == "select" || t.str == "project" || t.str == "rename" || t.str == "WHERE")
	{
		ts.putback(t);
		identifier = "";
		return false;
	}
	identifier = t.str;
	return true;
}

bool p_relation_name(Token_stream& ts)
{
	string ident = identifier;
	if(p_identifier(ts))
	{
		relation_name = identifier;
		identifier = ident;
		return true;
	}
	relation_name = "";
	identifier = ident;
	return false;
}
bool str_check(Token_stream& ts, string str)
{
	Token t = ts.get();
	if(t.str == str)
		return true;
	ts.putback(t);
	return false;
}
bool kind_check(Token_stream& ts, char kind)
{
	Token t = ts.get();
	if(t.kind == kind)
		return true;
	ts.putback(t);
	return false;
}

bool p_atomic_expr(Token_stream& ts);

bool p_union(Token_stream& ts)
{
	Token_stream tscopy(ts);
	Relation* ae = atomic_expr;
	if(p_atomic_expr(ts) && kind_check(ts, '+'))
	{
		Relation* aeleft = atomic_expr;
		if(p_atomic_expr(ts))
		{
			_union = d._union(aeleft, atomic_expr);
			atomic_expr = ae;
			return true;
		}
		atomic_expr = ae;
		return false;
	}
	atomic_expr = ae;
	ts.copy(tscopy);
	return false;
}

bool p_difference(Token_stream& ts)
{
	Token_stream tscopy(ts);
	Relation* ae = atomic_expr;
	if(p_atomic_expr(ts) && kind_check(ts, '-'))
	{
		Relation* aeleft = atomic_expr;
		if(p_atomic_expr(ts))
		{
			difference = d.difference(aeleft, atomic_expr);
			atomic_expr = ae;
			return true;
		}
		atomic_expr = ae;
		return false;
	}
	atomic_expr = ae;
	ts.copy(tscopy);
	return false;
}

bool p_product(Token_stream& ts)
{
	Token_stream tscopy(ts);
	Relation* ae = atomic_expr;
	if(p_atomic_expr(ts) && kind_check(ts, '*'))
	{
		Relation* aeleft = atomic_expr;
		if(p_atomic_expr(ts))
		{
			product = d.product(aeleft, atomic_expr);
			atomic_expr = ae;
			return true;
		}
		atomic_expr = ae;
		return false;
	}
	atomic_expr = ae;
	ts.copy(tscopy);
	return false;
}

bool p_literal(Token_stream& ts);

bool p_attribute_name(Token_stream& ts)
{
	string ident = identifier;
	if(p_identifier(ts))
	{
		attribute_name = identifier;
		identifier = ident;
		return true;
	}
	attribute_name = identifier;
	identifier = ident;
	return false;
}

bool p_operand(Token_stream& ts)
{
	string an = attribute_name;
	if(p_attribute_name(ts))
	{
		operand = Token('a', attribute_name);
		attribute_name = an;
		return true;
	}
	attribute_name = an;
	string l = literal;
	if(p_literal(ts))
	{
		operand = Token('"', literal);
		literal = l;
		return true;
	}
	literal = l;
	return false;
}

bool p_op(Token_stream& ts)
{
	Token t = ts.get();
	if(t.kind == '=')
	{
		op = t.str;
		return true;
	}
	ts.putback(t);
	return false;
}

bool p_condition(Token_stream& ts);

bool p_comparison(Token_stream& ts)
{
	if(kind_check(ts, '(') && p_comparison(ts) && kind_check(ts, ')'))
		return true;
	Token oper = operand;
	string o = op;
	if(p_operand(ts) && p_op(ts))
	{
		comparison_operands.push_back(operand);
		comparison_ops.push_back(op);
		if(!p_operand(ts))
		{
			comparison_operands.pop_back();
			comparison_ops.pop_back();
			operand = oper;
			op = o;
			return false;
		}
		comparison_operands.push_back(operand);
		operand = oper;
		op = o;
		return true;
	}
	operand = oper;
	op = o;
	return false;
}

bool p_conjunction(Token_stream& ts)
{
	if(!p_comparison(ts))
		return false;
	while(kind_check(ts, '&') && p_comparison(ts))
		conjunction.push_back("&");
	return true;
}

bool p_condition(Token_stream& ts)
{
	if(!p_conjunction(ts))
		return false;
	while(kind_check(ts, '|') && p_conjunction(ts))
		conjunction.push_back("|");
	return true;
}

bool p_selection(Token_stream& ts)
{
	vector<string> conj = conjunction;
	vector<Token> c_oper = comparison_operands;
	vector<string> c_ops = comparison_ops;
	conjunction.clear();
	comparison_operands.clear();
	comparison_ops.clear();
	Relation* ae = atomic_expr;
	if(str_check(ts, "select") && kind_check(ts, '(') && p_condition(ts) && kind_check(ts, ')') && p_atomic_expr(ts))
	{
		Token operand2 = comparison_operands.back();
		comparison_operands.pop_back();
		Token operand1 = comparison_operands.back();
		comparison_operands.pop_back();
		string op = comparison_ops.back();
		comparison_ops.pop_back();
		if(operand2.kind == '"')
			selection = d.select_literal(operand1.str, op, operand2.str, atomic_expr);
		else if(operand2.kind == 'a')
			selection = d.select_attribute(operand1.str, op, operand2.str, atomic_expr);
		while(!comparison_operands.empty())
		{
			operand2 = comparison_operands.back();
			comparison_operands.pop_back();
			operand1 = comparison_operands.back();
			comparison_operands.pop_back();
			string op = comparison_ops.back();
			comparison_ops.pop_back();
			Relation* r;
			if(operand2.kind == '"')
				r = d.select_literal(operand1.str, op, operand2.str, atomic_expr);
			else if(operand2.kind == 'a')
				r = d.select_attribute(operand1.str, op, operand2.str, atomic_expr);
			if(conjunction.back() == "&")
				selection = d.intersection(selection, r);
			else if(conjunction.back() == "|")
				selection = d._union(selection, r);
		}
		conjunction = conj;
		comparison_operands = c_oper;
		comparison_ops = c_ops;
		atomic_expr = ae;
		return true;
	}
	conjunction = conj;
	comparison_operands = c_oper;
	comparison_ops = c_ops;
	atomic_expr = ae;
	return false;
}

bool p_attribute_list(Token_stream& ts)
{
	string an = attribute_name;
	vector<string> al;
	while(p_attribute_name(ts))
		al.push_back(attribute_name);
	attribute_name = an;
	attribute_list = al;
	return !attribute_list.empty();
}

bool p_projection(Token_stream& ts)
{
	vector<string> al = attribute_list;
	Relation* ae = atomic_expr;
	if(str_check(ts, "project") && kind_check(ts, '(') && p_attribute_list(ts) && kind_check(ts, ')') && p_atomic_expr(ts))
	{
		projection = d.project(attribute_list, atomic_expr);
		attribute_list = al;
		atomic_expr = ae;
		return true;
	}
	attribute_list = al;
	atomic_expr = ae;
	return false;
}

bool p_rename(Token_stream& ts)
{
	vector<string> al = attribute_list;
	Relation* ae = atomic_expr;
	if(str_check(ts, "rename") && kind_check(ts, '(') && p_attribute_list(ts) && kind_check(ts, ')') && p_atomic_expr(ts))
	{
		__rename = d.rename(attribute_list, atomic_expr);
		attribute_list = al;
		atomic_expr = ae;
		return true;
	}
	attribute_list = al;
	atomic_expr = ae;
	return false;
}

bool p_expr(Token_stream& ts)
{
	Relation* sel = selection;
	if(p_selection(ts))
	{
		expr = selection;
		selection = sel;
		return true;
	}
	selection = sel;
	Relation* proj = projection;
	if(p_projection(ts))
	{
		expr = projection;
		projection = proj;
		return true;
	}
	projection = proj;
	Relation* ren = __rename;
	if(p_rename(ts))
	{
		expr = __rename;
		__rename = ren;
		return true;
	}
	__rename = ren;
	Relation* un = _union;
	if(p_union(ts))
	{
		expr = _union;
		_union = un;
		return true;
	}
	_union = un;
	Relation* diff = difference;
	if(p_difference(ts))
	{
		expr = difference;
		difference = diff;
		return true;
	}
	difference = diff;
	Relation* prod = product;
	if(p_product(ts))
	{
		expr = product;
		product = prod;
		return true;
	}
	product = prod;
	Relation* ae = atomic_expr;
	if(p_atomic_expr(ts))
	{
		expr = atomic_expr;
		atomic_expr = ae;
		return true;
	}
	atomic_expr = ae;
	return false;
}

bool p_atomic_expr(Token_stream& ts)
{
	string rn = relation_name;
	if(p_relation_name(ts))
	{
		atomic_expr = d.get_relation(relation_name);
		relation_name = rn;
		return true;
	}
	relation_name = rn;
	Relation* e = expr;
	if(kind_check(ts, '(') && p_expr(ts) && kind_check(ts, ')'))
	{
		atomic_expr = expr;
		expr = e;
		return true;
	}
	expr = e;
	return false;
}

bool p_literal(Token_stream& ts)
{
	Token t = ts.get();
	if(t.kind == '8')
	{
		stringstream ss;
		ss << t.value;
		literal = ss.str();
		return true;
	}
	else if(t.kind == '"')
	{
		literal = t.str;
		return true;
	}
	ts.putback(t);
	return false;
}

bool p_typed_literal(Token_stream& ts)
{
	Token t = ts.get();
	if(t.kind == '8')
	{
		stringstream ss;
		ss << t.value;
		typed_literal.type = INT;
		typed_literal.value = ss.str();
		return true;
	}
	else if(t.kind == '"')
	{
		typed_literal.type = STRING;
		typed_literal.value = t.str;
		return true;
	}
	ts.putback(t);
	return false;
}

bool p_literal_list(Token_stream& ts)
{
	string l = literal;
	vector<string> ll;
	while(p_literal(ts))
		ll.push_back(literal);
	literal = l;
	literal_list = ll;
	return !literal_list.empty();
	return false;
}

bool p_assignment_list(Token_stream& ts)
{
	string an = attribute_name;
	Cell tl = typed_literal;
	attribute_list.clear();
	typed_literal_list.clear();
	if(!(p_attribute_name(ts) && kind_check(ts, '~') && p_typed_literal(ts)))
	{
		attribute_name = an;
		typed_literal = tl;
		return false;
	}
	attribute_list.push_back(attribute_name);
	typed_literal_list.push_back(typed_literal);
	while(p_attribute_name(ts) && kind_check(ts, '~') && p_typed_literal(ts))
	{
		attribute_list.push_back(attribute_name);
		typed_literal_list.push_back(typed_literal);
	}
	attribute_name = an;
	typed_literal = tl;
	return true;
}

bool p_type(Token_stream& ts)
{
	if(str_check(ts, "VARCHAR") && kind_check(ts, '(') && kind_check(ts, '8') && kind_check(ts, ')'))
	{
		type = STRING;
		return true;
	}
	else if(str_check(ts, "INTEGER"))
	{
		type = INT;
		return true;
	}
	return false;
}

bool p_typed_attribute_list(Token_stream& ts)
{
	string an = attribute_name;
	Type t = type;
	vector<Attribute> tal;
	while(p_attribute_name(ts) && p_type(ts))
	{
		Attribute a(type, attribute_name);
		tal.push_back(a);
	}
	attribute_name = an;
	type = t;
	typed_attribute_list = tal;
	return !typed_attribute_list.empty();
}

bool p_insert(Token_stream& ts)
{
	string rn = relation_name;
	if(str_check(ts, "INSERT") && str_check(ts, "INTO") && p_relation_name(ts) && str_check(ts, "VALUES") && str_check(ts, "FROM"))
	{
		vector<string> ll = literal_list;
		if(kind_check(ts, '(') && p_literal_list(ts) && kind_check(ts, ')'))
		{
			insert = d.insert(relation_name, literal_list);
			relation_name = rn;
			literal_list = ll;
			return true;
		}
		literal_list = ll;
		Relation* e = expr;
		if(str_check(ts, "RELATION") && p_expr(ts))
		{
			insert = d.insert(relation_name, expr);
			relation_name = rn;
			expr = e;
			return true;
		}
		relation_name = rn;
		expr = e;
		return false;
	}
	relation_name = rn;
	return false;
}

bool p_delete(Token_stream& ts)
{
	string rn = relation_name;
	vector<string> conj = conjunction;
	vector<Token> c_oper = comparison_operands;
	vector<string> c_ops = comparison_ops;
	conjunction.clear();
	comparison_operands.clear();
	comparison_ops.clear();
	if(str_check(ts, "DELETE") && str_check(ts, "FROM") && p_relation_name(ts) && str_check(ts, "WHERE") && p_condition(ts))
	{
		Relation* rel = d.get_relation(relation_name);
		Token operand2 = comparison_operands.back();
		comparison_operands.pop_back();
		Token operand1 = comparison_operands.back();
		comparison_operands.pop_back();
		string op = comparison_ops.back();
		comparison_ops.pop_back();
		Relation* r;
		if(operand2.kind == '"')
			r = d.select_literal(operand1.str, op, operand2.str, rel);
		else if(operand2.kind == 'a')
			r = d.select_attribute(operand1.str, op, operand2.str, rel);
		while(!comparison_operands.empty())
		{
			operand2 = comparison_operands.back();
			comparison_operands.pop_back();
			operand1 = comparison_operands.back();
			comparison_operands.pop_back();
			string op = comparison_ops.back();
			comparison_ops.pop_back();
			Relation* r2;
			if(operand2.kind == '"')
				r2 = d.select_literal(operand1.str, op, operand2.str, rel);
			else if(operand2.kind == 'a')
				r2 = d.select_attribute(operand1.str, op, operand2.str, rel);
			if(conjunction.back() == "&")
				r = d.intersection(r2, r);
			else if(conjunction.back() == "|")
				r = d._union(r2, r);
		}
		rel = d.difference(rel, r);
		rel->set_name(relation_name);
		d.set_relation(relation_name, rel);
		__delete = rel;
		relation_name = rn;
		conjunction = conj;
		comparison_operands = c_oper;
		comparison_ops = c_ops;
		return true;
	}
	relation_name = rn;
	conjunction = conj;
	comparison_operands = c_oper;
	comparison_ops = c_ops;
	return false;
}

bool p_create(Token_stream& ts)
{
	string rn = relation_name;
	vector<Attribute> tal = typed_attribute_list;
	vector<string> al = attribute_list;
	if(str_check(ts, "CREATE") && str_check(ts, "TABLE") && p_relation_name(ts) && kind_check(ts, '(') && p_typed_attribute_list(ts) && kind_check(ts, ')')
		&& str_check(ts, "PRIMARY") && str_check(ts, "KEY") && kind_check(ts, '(') && p_attribute_list(ts) && kind_check(ts, ')'))
	{
		Relation* r = d.create(relation_name, typed_attribute_list, attribute_list);
		create = r;
		relation_name = rn;
		typed_attribute_list = tal;
		attribute_list = al;
		return true;
	}
	create = NULL;
	relation_name = rn;
	typed_attribute_list = tal;
	attribute_list = al;
	return false;
}

bool p_update(Token_stream& ts)
{
	string rn = relation_name;
	vector<string> al = attribute_list;
	vector<Cell> tll = typed_literal_list;
	vector<string> conj = conjunction;
	vector<Token> c_oper = comparison_operands;
	vector<string> c_ops = comparison_ops;
	conjunction.clear();
	comparison_operands.clear();
	comparison_ops.clear();
	if(str_check(ts, "UPDATE") && p_relation_name(ts) && str_check(ts, "SET") && p_assignment_list(ts) && str_check(ts, "WHERE") && p_condition(ts))
	{
		Relation* rel = d.get_relation(relation_name);
		Token operand2 = comparison_operands.back();
		comparison_operands.pop_back();
		Token operand1 = comparison_operands.back();
		comparison_operands.pop_back();
		string op = comparison_ops.back();
		comparison_ops.pop_back();
		Relation* r;
		if(operand2.kind == '"')
			r = d.select_literal(operand1.str, op, operand2.str, rel);
		else if(operand2.kind == 'a')
			r = d.select_attribute(operand1.str, op, operand2.str, rel);
		while(!comparison_operands.empty())
		{
			operand2 = comparison_operands.back();
			comparison_operands.pop_back();
			operand1 = comparison_operands.back();
			comparison_operands.pop_back();
			string op = comparison_ops.back();
			comparison_ops.pop_back();
			Relation* r2;
			if(operand2.kind == '"')
				r2 = d.select_literal(operand1.str, op, operand2.str, rel);
			else if(operand2.kind == 'a')
				r2 = d.select_attribute(operand1.str, op, operand2.str, rel);
			if(conjunction.back() == "&")
				r = d.intersection(r2, r);
			else if(conjunction.back() == "|")
				r = d._union(r2, r);
		}
		rel = d.difference(rel, r);
		rel = d._union(rel, d.update(attribute_list, typed_literal_list, r));
		rel->set_name(relation_name);
		d.set_relation(relation_name, rel);
		update = rel;
		relation_name = rn;
		attribute_list = al;
		typed_literal_list = tll;
		conjunction = conj;
		comparison_operands = c_oper;
		comparison_ops = c_ops;
		return true;
	}
	relation_name = rn;
	attribute_list = al;
	typed_literal_list = tll;
	conjunction = conj;
	comparison_operands = c_oper;
	comparison_ops = c_ops;
	return false;
}

bool p_show(Token_stream& ts)
{
	Relation* ae = atomic_expr;
	if(str_check(ts, "SHOW") && p_atomic_expr(ts))
	{
		if(atomic_expr)
			atomic_expr->show();
		atomic_expr = ae;
		return true;
	}
	atomic_expr = ae;
	return false;
}

bool p_exit(Token_stream& ts)
{
	if(str_check(ts, "EXIT"))
		exit(0);
	return false;
}

bool p_write(Token_stream& ts)
{
	string rn = relation_name;
	if(str_check(ts, "WRITE") && p_relation_name(ts))
	{
		d.write(relation_name);
		relation_name = rn;
		return true;
	}
	relation_name = rn;
	return false;
}

bool p_close(Token_stream& ts)
{
	if(str_check(ts, "CLOSE"))
	{
		d.close();
		return true;
	}
	return false;
}

bool p_open(Token_stream& ts)
{
	string rn = relation_name;
	if(str_check(ts, "OPEN") && p_relation_name(ts))
	{
		d.open(relation_name);
		relation_name = rn;
		return true;
	}
	relation_name = rn;
	return false;
}

bool p_command(Token_stream& ts)
{
	return p_open(ts) || p_close(ts) || p_write(ts) || p_exit(ts) || p_show(ts) || p_create(ts) || p_update(ts) || p_insert(ts) || p_delete(ts);
}

bool p_query(Token_stream& ts)
{
	if(p_relation_name(ts) && kind_check(ts, ':') && p_expr(ts))
	{
		Relation* r;
		if(d.has_relation(relation_name))
			r = d.get_relation(relation_name);
		else
		{
			r = new Relation(relation_name);
			d.push_back(r);
		}
		r->copy(expr);
		return true;
	}
	return false;
}

bool p_termination(Token_stream& ts)
{
	Token t = ts.get();
	return t.kind == ';';
}

bool program(Token_stream& ts)
{
	return (p_command(ts) || p_query(ts)) && p_termination(ts);
}

void Parser::parse(string line, bool print)
{
	Token_stream ts;
	ts.ss.clear();
	ts.ss.str("");
	ts.ss << line;
	cout << "Input line: " << line << endl;
	try
	{
		if(!program(ts) && print)
			cout << "THIS IS WRONG" << endl;
		else if(print)
			cout << " VALID SYNTAX " << endl;
	}
	catch(string err)
	{
		if(print)
			cout << err << endl;
	}
}


