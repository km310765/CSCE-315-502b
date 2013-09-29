#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include <iostream>
#include <vector>

using namespace std;

enum Type { INT = 1, STRING };

struct Cell
{
	string value;
	Type type;
};

class Attribute
{
private:
	string name;
	Type type;
	vector<Cell> cells;
public:
	Attribute() {}
	Attribute(Type t, string n) : type(t), name(n) {}
	
	int size() { return cells.size(); }
	string get_name() { return name; }
	Type get_type() { return type; }
	Cell get(int i) { return cells[i]; }
	Cell operator[](size_t i) { return get(i); }
	vector<Cell> get_cells() { return cells; }
	void clear() { cells.clear(); }
	void push_back(Cell c) { cells.push_back(c); }
	void push_back(string v, Type t)
	{
		Cell c;
		c.value = v;
		c.type = t;
		cells.push_back(c);
	}
	void insert(int i, Cell c) { cells.insert(cells.begin() + i, c); }
	void set_name(string n) { name = n; }
};

#endif
