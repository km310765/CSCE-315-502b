#ifndef RELATION_H
#define RELATION_H
#include <iostream>
#include <vector>
#include "Attribute.h"
#include "iomanip"
using namespace std;
//--------------------------------------------------------------------
//Table class for reading in data
//Input data format:
//First: # of cars Then: # of attributes
//Header: -Attribute Title- -Attribute Title- -Attribute Title- ...etc
//Tuples: -Attribute- -Attribute- -Attribute- ...etc
//--------------------------------------------------------------------

class Relation{
  public:
    vector< Attribute > attr;
    string name;
    Relation() {}
    Relation(const string& name){
      Create(name);
    }
    void add_attr(const Attribute& a) {
      attr.push_back(a);
    }
    void add_attr(string nm, Type t) {
      add_attr(Attribute(t,nm));
    }
    Attribute& operator[](size_t col){
      return attr[col];
    }
    Attribute& getAttribute(int n)
    {
      return (*this)[n];
    }
	int findAttribute(string attr_name){
		for(int i = 0; i < attr.size(); i++){
			if(attr_name == attr[i].getName())
				return i;
		}
		return -1;
	}
    int getNumAttributes()
    {
      return attr.size();
    }
	Type getAttributeType(int n) {
      return attr[n].type;
    }
	void setRow(int n, vector<Cell> row)
	{
		for(int i = 0; i < attr.size(); i++)
			attr[i].cell[n] = row[i];
	}
	void deleteRow(int n)
	{
		for(int i = 0; i < attr.size(); i++)
			attr[i].cell.erase(attr[i].cell.begin() + n);
	}
    vector<Cell> getRow(int n)
    {
      vector<Cell> tuple;
      for(int i = 0; i < attr.size(); i++)
      {
        tuple.push_back(getAttribute(i).cell[n]);
      }
      return tuple;
    }

    int getRowSize() {
      return attr[0].getLength();
    }

    void read(string s);

    void Create(string rel_name){
      name = rel_name;
    }

    void Show() {
	if(attr.size() !=0){
		  vector<Cell> tuple;
		  cout << endl;
		  cout << "     ";
		  for(int i = 0; i< attr.size(); i++) {
			cout << "---------------";
			}
			cout << endl;
		  cout << "     " << name << " TABLE" << endl;
		  cout << "     ";
			for(int i = 0; i< attr.size(); i++) {
				cout << "---------------";
			}
			cout << endl;
		  for(int i = 0; i< attr.size(); i++) {
			cout << setw(15) << getAttribute(i).getName();
		  }
		  cout << endl;
		  cout << "     ";
			for(int i = 0; i< attr.size(); i++) {
				cout << "---------------";
			}
			cout << endl;
		  for(int r=0; r < attr[0].getLength(); ++r) {
			for(int c = 0; c< attr.size(); c++){
			  cout << setw(15) << (*this)[c][r];
			}
			cout << endl;
		  }
		  cout << endl;
	  }
	  else 
		cerr << "Relation '" << name << "' has no members." << endl;
    }

    void Rename(string new_name, string old_name) {
      for(int i =0; i < attr.size(); i++) {
        if(old_name == attr[i].name) {
          attr[i].name = new_name;
        }
      }
    }

    void Insert(vector<Cell> row) { 
      for(int c = 0; c< attr.size(); c++)
        attr[c].push_back(row[c]);
    }

    void Write() {
      cout << "Requires File I/O, will be implemented in the next part " << endl;
    }
	void clear_attr_cells(){
		for (int i = 0; i < attr.size(); ++i){
			attr[i].cell.clear();
		}
	}
    void Delete_attr(const string& attribute) {
      for(int i =0; i < attr.size(); i++) {
        for(int j = 0; j < attr[0].getLength(); j++) {
          if(attribute == attr[i][j]) {
            cout << "Found " << attr[i][j] << endl;
            attr[i][j] = "";
            cout << "Deleted " << attribute << endl;
            return;
          }
        }
      }
      cout << "Did not find " << attribute << " in the relation" << endl;
    }

};
#endif
