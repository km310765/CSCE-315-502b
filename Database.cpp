#include <algorithm> 
#include <iostream>
#include <fstream>
#include "Database.h"
#include "Parser.h"

bool Database::has_relation(string name)
{
	for(int i = 0; i < relations.size(); i++)
		if(relations[i]->get_name() == name)
			return true;
	return false;
}

void Database::set_relation(string name, Relation* r)
{
	for(int i = 0; i < relations.size(); i++)
		if(relations[i]->get_name() == name)
			relations[i] = r;
}

void Database::write(string name)
{
	if(!has_relation(name))
		throw "Relation '" + name + "' not found";
	ofstream file;
	file.open((name + ".db").c_str());
	for(int i = 0; i < relations.size(); i++)
	{
		if(relations[i]->get_name() != name)
			continue;
		file << "CREATE TABLE " << name << " (";
		for(int j = 0; j < relations[i]->get_num_attributes(); j++)
		{
			file << relations[i]->get_attribute(j).get_name() << " " << ((relations[i]->get_attribute(j).get_type() == INT) ? "INTEGER" : "VARCHAR(1)");
			if(j + 1 < relations[i]->get_num_attributes())
				file << ", ";
		}
		file << ") PRIMARY KEY (";
		for(int j = 0; j < relations[i]->get_num_primaries(); j++)
		{
			file << relations[i]->get_primary(j);
			//if(j + 1 < relations[i]->get_num_attributes())
				//file << ", ";
		}
		file << ");\n";
		for(int j = 0; j < relations[i]->get_num_tuples(); j++)
		{
			vector<Cell> cells = relations[i]->get_row(j);
			vector<string> row;
			for(int k = 0; k < cells.size(); k++)
				row.push_back(cells[k].value);
			file << "INSERT INTO " << name << " VALUES FROM (";
			for(int k = 0; k < relations[i]->get_num_attributes(); k++)
			{
				file << ((relations[i]->get_attribute(k).get_type() == INT) ? row[k] : ("\"" + row[k] + "\""));
				if(k + 1 < relations[i]->get_num_attributes())
					file << ", ";
			}
			file << ");\n";
		}
		return;
	}
}

void Database::close()
{
	for(int i = 0; i < relations.size(); i++)
		write(relations[i]->get_name());
}

void Database::open(string name)
{
	for(int i = 0; i < relations.size(); i++)
		if(relations[i]->get_name() == name)
			relations.erase(relations.begin() + i);
	ifstream file;
	file.open((name + ".db").c_str());
	Parser p(this);
	string line;
	while(getline(file, line))
		p.parse(line, false);
}

Relation* Database::get_relation(string name)
{
	for(int i = 0; i < relations.size(); i++)
		if(relations[i]->get_name() == name)
			return relations[i];
	throw "Relation '" + name + "' not found";
}

Relation* Database::create(string name, vector<Attribute> attributes, vector<string> primary)
{
	for(int i = 0; i < relations.size(); i++)
		if(relations[i]->get_name() == name)
			relations.erase(relations.begin() + i);
	Relation* r = new Relation(name, attributes, primary);
	relations.push_back(r);
	return r;
}

Relation* Database::insert(string name, vector<string> literal_list)
{
	if(!has_relation(name))
		throw "Relation '" + name + "' not found";
	Relation* r = get_relation(name);
	r->insert(literal_list);
	return r;
}

Relation* Database::insert(string name, Relation* expr)
{
	if(!has_relation(name))
		throw "Relation '" + name + "' not found";
	Relation* r = get_relation(name);
	r->insert(expr);
	return r;
}

Relation* Database::select_literal(string attribute_name, string op, string literal, Relation* expr)
{
	Relation* r = new Relation();
	r->copy(expr);
	r->clear();
	int attr_loc = expr->find_attribute(attribute_name);
	if(attr_loc == -1)
		throw string("Attribute not found");
	vector<int> condition_met_loc;
	for(int i = 0; i < expr->get_attribute(attr_loc).size(); i++)
	{
		if(op == "==")
			if(expr->get_attribute(attr_loc)[i].value == literal)
				condition_met_loc.push_back(i);
		else if(op == "!=")
			if(expr->get_attribute(attr_loc)[i].value != literal)
				condition_met_loc.push_back(i);
		else if(op == "<=")
			if(expr->get_attribute(attr_loc)[i].value <= literal)
				condition_met_loc.push_back(i);
		else if(op == ">=")
			if(expr->get_attribute(attr_loc)[i].value >= literal)
				condition_met_loc.push_back(i);
		else if(op == "<")
			if(expr->get_attribute(attr_loc)[i].value < literal)
				condition_met_loc.push_back(i);
		else if(op == ">")
			if(expr->get_attribute(attr_loc)[i].value > literal)
				condition_met_loc.push_back(i);
	}
	for(int i = 0; i < condition_met_loc.size(); i++)
		r->insert(expr->get_row(condition_met_loc[i]));
	return r;
}

Relation* Database::select_attribute(string attribute_name1, string op, string attribute_name2, Relation* expr)
{
	Relation* r = new Relation();
	r->copy(expr);
	r->clear();
	int attr_loc1 = expr->find_attribute(attribute_name1);
	int attr_loc2 = expr->find_attribute(attribute_name2);
	if(attr_loc1 == -1 || attr_loc2 == -1)
		throw string("Attribute not found");
	vector<int> condition_met_loc;
	for(int i = 0; i < expr->get_attribute(attr_loc1).size(); i++)
	{
		if(op == "==")
			if(expr->get_attribute(attr_loc1)[i].value == expr->get_attribute(attr_loc2)[i].value)
				condition_met_loc.push_back(i);
		else if(op == "!=")
			if(expr->get_attribute(attr_loc1)[i].value != expr->get_attribute(attr_loc2)[i].value)
				condition_met_loc.push_back(i);
		else if(op == "<=")
			if(expr->get_attribute(attr_loc1)[i].value <= expr->get_attribute(attr_loc2)[i].value)
				condition_met_loc.push_back(i);
		else if(op == ">=")
			if(expr->get_attribute(attr_loc1)[i].value >= expr->get_attribute(attr_loc2)[i].value)
				condition_met_loc.push_back(i);
		else if(op == "<")
			if(expr->get_attribute(attr_loc1)[i].value < expr->get_attribute(attr_loc2)[i].value)
				condition_met_loc.push_back(i);
		else if(op == ">")
			if(expr->get_attribute(attr_loc1)[i].value > expr->get_attribute(attr_loc2)[i].value)
				condition_met_loc.push_back(i);
	}
	for(int i = 0; i < condition_met_loc.size(); i++)
		r->insert(expr->get_row(condition_met_loc[i]));
	return r;
}

Relation* Database::project(vector<string> attribute_list, Relation* atomic_expr)
{
	Relation* r = new Relation();
	for(int i = 0; i < attribute_list.size(); i++)
	{
		int attr_loc = atomic_expr->find_attribute(attribute_list[i]);
		if(attr_loc == -1)
			throw "Attribute '" + attribute_list[i] + "' not found";
		r->add_attribute(atomic_expr->get_attribute(attr_loc));
	}
	for(int i = 0; i < atomic_expr->get_num_primaries(); i++)
		for(int j = 0; j < r->get_num_attributes(); j++)
			if(r->get_attribute(j).get_name() == atomic_expr->get_primary(i))
				r->add_primary(atomic_expr->get_primary(i));
	return r;
}

Relation* Database::update(vector<string> attribute_list, vector<Cell> cells, Relation* atomic_expr)
{
	if(attribute_list.size() != cells.size())
		throw string("Number of attributes not equal to number of literals");
	Relation* r = new Relation();
	for(int i = 0; i < attribute_list.size(); i++)
	{
		int attr_loc = atomic_expr->find_attribute(attribute_list[i]);
		if(attr_loc == -1)
			throw "Attribute '" + attribute_list[i] + "' not found";
	}
	for(int i = 0; i < atomic_expr->get_num_attributes(); i++)
	{
		Attribute a = atomic_expr->get_attribute(i);
		for(int j = 0; j < attribute_list.size(); j++)
			if(attribute_list[j] == a.get_name())
			{
				if(cells[j].type != a.get_type())
					throw string("Types must match");
				a.clear();
				for(int k = 0; k < atomic_expr->get_num_tuples(); k++)
					a.push_back(cells[j]);
			}
		r->add_attribute(a);	
	}
	for(int i = 0; i < atomic_expr->get_num_primaries(); i++)
		for(int j = 0; j < r->get_num_attributes(); j++)
			if(r->get_attribute(j).get_name() == atomic_expr->get_primary(i))
				r->add_primary(atomic_expr->get_primary(i));
	return r;
}

Relation* Database::rename(vector<string> attribute_list, Relation* atomic_expr)
{
	if(attribute_list.size() > atomic_expr->get_num_attributes())
		throw string("Too many attributes given");
	if(attribute_list.size() < atomic_expr->get_num_attributes())
		throw string("Not enough attributes given");
	Relation* r = new Relation();
	r->copy(atomic_expr);
	for(int i = 0; i < attribute_list.size(); i++)
	{
		Attribute a = r->get_attribute(i);
		a.set_name(attribute_list[i]);
		r->set_attribute(i, a);
	}
	return r;
}

Relation* Database::_union(Relation* r1, Relation* r2)
{
	Relation* r = new Relation();
	r->copy(r1);
	for(int i = 0; i < r2->get_num_tuples(); i++)
		if(!r1->contains(r2->get_row(i)))
			r->insert(r2->get_row(i));
	return r;
}

Relation* Database::intersection(Relation* r1, Relation* r2)
{
	Relation* r = new Relation();
	r->copy(r1);
	r->clear();
	for(int i = 0; i < r2->get_num_tuples(); i++)
		if(r1->contains(r2->get_row(i)))
			r->insert(r2->get_row(i));
	return r;
}

Relation* Database::difference(Relation* r1, Relation* r2)
{
	Relation* r = new Relation();
	r->copy(r1);
	r->clear();
	for(int i = 0; i < r1->get_num_tuples(); i++)
		if(!r2->contains(r1->get_row(i)))
			r->insert(r1->get_row(i));
	return r;
}

Relation* Database::product(Relation* r1, Relation* r2)
{
	Relation* r = new Relation();
	r->copy(r1);
	for(int i = 0; i < r2->get_num_attributes(); i++)
		r->add_attribute(r2->get_attribute(i));
	for(int i = 0; i < r2->get_num_primaries(); i++)
		r->add_primary(r2->get_primary(i));
	r->clear();
	for(int i = 0; i < r1->get_num_tuples(); i++)
		for(int j = 0; j < r2->get_num_tuples(); j++)
		{
			vector<Cell> row;
			vector<Cell> row1 = r1->get_row(i);
			vector<Cell> row2 = r2->get_row(j);
			for(int k = 0; k < row1.size(); k++)
				row.push_back(row1[k]);
			for(int k = 0; k < row2.size(); k++)
				row.push_back(row2[k]);
			r->insert(row);
		}
	return r;
}
