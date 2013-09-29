#ifndef RELATION_H
#define RELATION_H
#include <iostream>
#include <vector>
#include "Attribute.h"
#include "iomanip"
using namespace std;

class Relation
{
private:
	string name;
	vector<Attribute> attributes;
	vector<string> primary;
	
	bool less_than(vector<Cell> row1, vector<Cell> row2);
public:
	Relation() {}
	Relation(string n) : name(n) {}
	Relation(string n, vector<Attribute> a, vector<string> p) : name(n), attributes(a), primary(p) {}
	
	string get_name() { return name; }
	vector<Attribute> get_attributes() { return attributes; }
	bool contains(vector<Cell> row);
	int get_num_attributes() { return attributes.size(); }
	int get_num_tuples() { return attributes[0].size(); }
	int get_num_primaries() { return primary.size(); }
	int find_attribute(string name);
	Attribute get_attribute(int i) { return attributes[i]; }
	string get_primary(int i) { return primary[i]; }
	Attribute operator[](size_t i) { return get_attribute(i); }
	vector<Cell> get_row(int i);
	void copy(Relation* r);
	void clear();
	void set_name(string n ) { name = n; }
	void set_attribute(int i, Attribute a) { attributes[i] = a; }
	void show();
	void insert(vector<Cell> row);
	void insert(vector<string> row);
	void insert(Relation* expr);
	void add_attribute(Attribute a) { attributes.push_back(a); }
	void add_primary(string s) { primary.push_back(s); }
};

#endif
