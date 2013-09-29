#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <map>
#include <exception>
#include "Relation.h"

using namespace std;

class Database
{
private:
	vector<Relation*> relations;
public:
	int size() { return relations.size(); }
	bool has_relation(string name);
	void push_back(Relation* r) { relations.push_back(r); }
	void set_relation(string name, Relation* r);
	void write(string name);
	void close();
	void open(string name);
	Relation* get_relation(int i) { return relations[i]; }
	Relation* get_relation(string name);
	Relation* create(string name, vector<Attribute> attributes, vector<string> primary);
	Relation* insert(string name, vector<string> literal_list);
	Relation* insert(string name, Relation* expr);
	Relation* select_literal(string attribute_name, string op, string literal, Relation* expr);
	Relation* select_attribute(string attribute_name1, string op, string attribute_name2, Relation* expr);
	Relation* project(vector<string> attribute_list, Relation* atomic_expr);
	Relation* update(vector<string> attribute_list, vector<Cell> cells, Relation* atomic_expr);
	Relation* rename(vector<string> attribute_list, Relation* atomic_expr);
	Relation* _union(Relation* r1, Relation* r2);
	Relation* intersection(Relation* r1, Relation* r2);
	Relation* difference(Relation* r1, Relation* r2);
	Relation* product(Relation* r1, Relation* r2);
};

#endif
