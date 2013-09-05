#include "Relation.h"
#include "Attribute.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
//Table function definitions
void Relation::read(string s){
	/*ifstream ifs(s.c_str());
	int cars; //#of cars
	int num_attr;//#of attributes
	ifs >> cars;
	ifs >> num_attr;
	for (int i = 0; i<num_attr; ++i){//initialization of attr's first level of vectors
		string pusher;
		ifs >> pusher;
		if(pusher.compare("Year"){
			Attribute<int> A(pusher);
			attr.push_back(A);
		}
		else if(pusher.compare("Price"){
			Attribute<int> A(pusher);
			attr.push_back(A);
		}
		else{
			Attribute<string> A(pusher);
			attr.push_back(A);
		}
	}
	for(int i = 0; i<cars; ++i){
		vector<string> v_tuples; //a tuple vector to push back as we go
		for(int j = 0; j<num_attr; ++j){
			char c = ifs.peek();
			if ( (c >= '0') && (c <= '9') ){
				int pusher;
				ifs >> pusher;
				attr[j].getCells().push_back(pusher);
				v_tuples.push_back(pusher);
			}
			else {
				string pusher;
				ifs >> pusher;
				attr[j].getCells().push_back(pusher);
				v_tuples.push_back(pusher);
			}
		}
		tuples.push_back(v_tuples);//pushback a portion of the tuples vector of vectors
	}
	ifs.close();*/
}
void Relation::dis_table(){
	/*for(int i = 0; i<6; ++i){
		for(int j = 0; j<4; ++j){
			cout << attr[j].getCells()[i] << " ";
		}
		cout << endl;
	}*/
}
