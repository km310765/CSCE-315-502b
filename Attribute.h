#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H
#include <iostream>
#include <vector>

using namespace std;

typedef string Cell;

enum Type {INT, STRING};

typedef pair<string, Type> Header;

class Attribute{
  public:
    string name;
  public:
    Type type;
    vector<string> cell;
    Attribute () {};
    Attribute(Type t, string n) : type(t), name(n) {};
   
    void push_back(string v)
    {
      cell.push_back(v);
    }
    vector<string> getCells() const{
      return cell;
    }
    string getName()
    {
      return name;
    }
	int getLength() const{
		return cell.size();
	}
	void setName(string set_name)
    {
      name = set_name;
    }

  Cell& operator[](size_t row){
    return cell[row];
  }
    //string getName();
};
#endif
