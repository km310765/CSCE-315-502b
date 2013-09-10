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

class Relation{//relation has a vector of attributes and a name. This is basically a "Table"
  public:
    vector< Attribute > attr;
    string name;
    Relation() {}
    Relation(const string& name){ //constructor which initializes name
      Create(name);
    }
    void add_attr(const Attribute& a) {//pushes back an attribute
      attr.push_back(a);
    }
    void add_attr(string nm, Type t) {//adds an attribute with defined types and names
      add_attr(Attribute(t,nm));
    }
    Attribute& operator[](size_t col){//returns an attribute
      return attr[col];
    }
    Attribute& getAttribute(int n)//another simpler return function (different people used different things for now)
    {
      return (*this)[n];
    }
	int findAttribute(string attr_name){//searches for a specific attribute and returns its int location else it returns -1 for not found
		for(int i = 0; i < attr.size(); i++){
			if(attr_name == attr[i].getName())
				return i;
		}
		return -1;
	}
    int getNumAttributes()//attr size value
    {
      return attr.size();
    }
	Type getAttributeType(int n) {//returns the attributes defined type
      return attr[n].type;
    } 
    vector<Cell> getRow(int n)// returns a tuple
    {
      vector<Cell> tuple;
      for(int i = 0; i < attr.size(); i++)
      {
        tuple.push_back(getAttribute(i).cell[n]);
      }
      return tuple;
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

    int getRowSize() {//returns the column length dispite the name (havent changed)
      return attr[0].getLength();
    }

    void read(string s);//not needed yet

    void Create(string rel_name){//sets a name
      name = rel_name;
    }

    void Show() {//the function that displays data in relation. Messy but looks nice in the command window
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

    void Rename(string new_name, string old_name) {//renames an attribute with a new name
      for(int i =0; i < attr.size(); i++) {
        if(old_name == attr[i].name) {
          attr[i].name = new_name;
        }
      }
    }

    void Insert(vector<Cell> row) { //inserts a tuple
      for(int c = 0; c< attr.size(); c++)
        attr[c].push_back(row[c]);
    }

    void Write() {
      cout << "Requires File I/O, will be implemented in the next part " << endl;
    }
	void clear_attr_cells(){//clears the whole attribute
		for (int i = 0; i < attr.size(); ++i){
			attr[i].cell.clear();
		}
	}
    void Delete_attr(const string& attribute) {//deletes a whole attribute
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
