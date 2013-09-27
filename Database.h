#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <map>
#include <exception>
#include "Relation.h"

using namespace std;

class Database {
  public:
	typedef map<string,Relation> Table;
	vector<Relation> relation;
	Relation Selection;
	Relation Projection;
	Database(){Create("Expression");}
	//Database(){};
	Table table;
	void SelectAttribute(string attr_name1, string condition, string attr_name2, string rel_name);
	void SelectLiteral(string attr_name, string condition, string cell_condition, string rel_name);
	void Update(string rel_name, string attr_name, string literal, string rel_name2);
	void Delete(string rel_name, string condition_attr, string condition, string condition_literal);
	void Project(vector<string> attr_name, string rel_name);
	void Create(string rel_name);
	//void Delete(string rel_name);
	void AddColumn(const string& rel_name, const Header& h);
	void Insert(string rel_name, const vector<Cell>& row);
	void Open(const string& rel_name);
	void Write(const string& rel_name);
	void Show(string rel_name);
	Relation Copy_table(const string rel_name1, const string rel_name2);
	//void Rename(string rel_name,string new_name, string old_name);
	void Rename(const string& newrel_name, const string& rel_name,vector<string> attr_names);
	Relation& operator[](const string& s);
	void Delete_attr(const string& rel_name,const string& attribute);
	void Union(const string& rel_name1,const string& rel_name2, const string& rel_name3);
	void Intersection(const string& rel_name1,const string& rel_name2, const string& rel_name3);
	void Difference(const string& rel_name1, const string& rel_name2, const string& rel_name3);
	void Product(const string& rel_name1, const string& rel_name2, const string& rel_name3);
};


#endif
