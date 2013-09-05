#ifndef RELATION_H
#define RELATION_H
#include <iostream>
#include <vector>
#include "Attribute.h"
using namespace std;
//--------------------------------------------------------------------
//Table class for reading in data
//Input data format:
//First: # of cars Then: # of attributes
//Header: -Attribute Title- -Attribute Title- -Attribute Title- ...etc
//Tuples: -Attribute-       -Attribute-       -Attribute-       ...etc
//--------------------------------------------------------------------

class Relation{
	private:
		vector< vector<string> > tuples;
	public:
		vector< Attribute > attr;
		Relation() {}
		void add_attr(Attribute& a) {
			attr.push_back(a);
		}
		
		Attribute getAttribute(int n)
		{
			return attr[n];
		}
		int getNumAttributes()
		{
			return attr.size();
		}
		vector<Cell> getTuple(int n)
		{
			vector<Cell> tuple;
			for(int i = 0; i < attr.size(); i++)
			{
				tuple.push_back(getAttribute(i).cell[n]);
			}
			return tuple;
		}
		
		void read(string s);
		void dis_table();
};
#endif
