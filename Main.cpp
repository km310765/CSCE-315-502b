#include <iostream>
#include "Relation.h"
using namespace std;

int main(){
	//Table T;
	//string file = "Cars.txt";
	//T.read(file);
	//T.dis_table();
	Attribute ints(INT, "Integers");
	Attribute chars(CHAR, "Characters");
	Value v1, v2, v3, v4;
	v1.i = 35;
	v2.i = 46;
	v3.c = 'a';
	v4.c = 'z';
	ints.push_back(v1);
	ints.push_back(v2);
	chars.push_back(v3);
	chars.push_back(v4);
	Relation r;
	r.add_attr(ints);
	r.add_attr(chars);
	Attribute intsecho = r.getAttribute(0);
	Attribute charsecho = r.getAttribute(1);
	cout << intsecho.getName() << endl;
	cout << charsecho.getName() << endl;
	vector<Cell> tuple = r.getTuple(0);
	cout << tuple[0].value.i << endl;
	cout << tuple[1].value.c << endl;
	tuple = r.getTuple(1);
	cout << tuple[0].value.i << endl;
	cout << tuple[1].value.c << endl;
	return 0;
}
