#include <iostream>
#include <algorithm>
#include <vector>
#include "Relation.h"

using namespace std;

int Relation::find_attribute(string name)
{
	for(int i = 0; i < get_num_attributes(); i++)
		if(attributes[i].get_name() == name)
			return i;
	return -1;
}

vector<Cell> Relation::get_row(int i)
{
	vector<Cell> row;
	for(int j = 0; j < get_num_attributes(); j++)
		row.push_back(attributes[j][i]);
	return row;
}

void Relation::copy(Relation* r)
{
	attributes = r->attributes;
	primary = r->primary;
}

void Relation::clear()
{
	for(int i = 0; i < get_num_attributes(); i++)
		attributes[i].clear();
}

void Relation::show()
{
	if(get_num_attributes() == 0)
		throw + "Relation '" + get_name() + "' has no members";
	vector<Cell> tuple;
	cout << endl;
	cout << "     ";
	for(int i = 0; i < get_num_attributes(); i++)
		cout << "---------------";
	cout << endl;
	cout << "     " << get_name() << " TABLE" << endl;
	cout << "     ";
	for(int i = 0; i < get_num_attributes(); i++)
		cout << "---------------";
	cout << endl;
	for(int i = 0; i < get_num_attributes(); i++)
		cout << setw(15) << get_attribute(i).get_name();
	cout << endl;
	cout << "     ";
	for(int i = 0; i < get_num_attributes(); i++)
		cout << "---------------";
	cout << endl;
	for(int i = 0; i < get_num_tuples(); i++)
	{
		for(int j = 0; j < get_num_attributes(); j++)
			cout << setw(15) << get_row(i)[j].value;
		cout << endl;
	}
	cout << endl;
}

bool Relation::less_than(vector<Cell> row1, vector<Cell> row2)
{
	for(int i = 0; i < primary.size(); i++)
		for(int j = 0;  j < get_num_attributes(); j++)
			if(primary[i] == get_attribute(j).get_name())
				if(row1[j].value < row2[j].value)
					return true;
				else if(row1[j].value > row2[j].value)
					return false;
	for(int i = 0; i < get_num_attributes(); i++)
		if(find(primary.begin(), primary.end(), get_attribute(i).get_name()) == primary.end())
			if(row1[i].value < row2[i].value)
				return true;
			else if(row1[i].value > row2[i].value)
				return false;
	return true;
}

bool Relation::contains(vector<Cell> row)
{
	if(row.size() != get_num_attributes())
		return false;
	for(int i = 0; i < get_num_tuples(); i++)
	{
		bool found = true;
		for(int j = 0; j < get_num_attributes(); j++)
			if(get_row(i)[j].value != row[j].value)
				found = false;
		if(found)
			return true;
	}
	return false;
}

void Relation::insert(vector<Cell> row)
{
	bool found = false;
	for(int i = 0; i < get_num_tuples(); i++)
		if(less_than(row, get_row(i)))
		{
			for(int j = 0; j < get_num_attributes(); j++)
			{
				attributes[j].insert(i, row[j]);
				found = true;
			}
			break;
		}
	if(!found)
		for(int i = 0; i < get_num_attributes(); i++)
			attributes[i].push_back(row[i]);
}

void Relation::insert(vector<string> row)
{
	if(row.size() != get_num_attributes())
		throw string("Length does not match number of attributes");
	vector<Cell> cells;
	for(int i = 0; i < get_num_attributes(); i++)
	{
		Cell c;
		c.type = attributes[i].get_type();
		c.value = row[i];
		cells.push_back(c);
	}
	insert(cells);
}

void Relation::insert(Relation* expr)
{
	if(expr->get_num_attributes() != get_num_attributes())
		throw string("Number of attributes does not match");
	for(int i = 0; i < expr->get_num_tuples(); i++)
	{
		vector<Cell> cells = expr->get_row(i);
		bool found = false;
		for(int j = 0; j < get_num_tuples(); j++)
			if(less_than(cells, get_row(j)))
			{
				for(int k = 0; k < get_num_attributes(); k++)
				{
					attributes[k].insert(j, cells[k]);
					found = true;
				}
				break;
			}
		if(!found)
			for(int j = 0; j < get_num_attributes(); j++)
				attributes[j].push_back(cells[j]);
	}
}
