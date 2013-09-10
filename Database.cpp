#include "Database.h"
#include <algorithm> 

// Compute the projection of two relations
void Database:: Project(vector<string> attr_name, string rel_name){
	for (int i = 0; i < relation.size(); ++i){
		if (rel_name == relation[i].name){
			Relation new_projection;    // create a new relation
			new_projection.name = "Projection";    // rename the relation 
			for (int j = 0; j < attr_name.size(); ++j){
				int attr_loc;
				attr_loc = (relation[i].findAttribute(attr_name[j]));
				if (attr_loc != -1){
					new_projection.attr.push_back(relation[i].attr[attr_loc]);
				}
				else
					cerr << "No attribute '" << attr_name[j] << "' was found in " << rel_name <<"." << endl;
			}
			if (new_projection.attr.size() == 0){
				cerr << "None of the attributes requested were found to be projected from '" << rel_name << "'." <<endl;
				return;
			}
			else 
				Projection = new_projection;
			return;
		}
	}	
	cerr << "I did not find anything called "<< rel_name << endl;
}

// function used to select a tuple that satisfies a particular condition in a relation
void Database:: Select(string attr_name, string condition, string cell_condition, string rel_name) {
	for (int i = 0; i < relation.size(); ++i){
		if (rel_name == relation[i].name){
			int attr_loc = relation[i].findAttribute(attr_name);
			if (attr_loc != -1){
				Relation new_selection = relation[i];
				new_selection.clear_attr_cells();
				new_selection.name = "Selection";
				vector<string> getCells = relation[i].attr[attr_loc].getCells();
				vector<int> condition_metLoc;
				
				// Different conditions to be tested. The operations are >,<,==,>= , !=,<=
				if(condition == ">"){
					if(relation[i].attr[attr_loc].getType() == INT){
						for (int j = 0; j < getCells.size(); ++j){
							int getCell = atoi(getCells[j].c_str());//conversion from string to int
							int cell_cond = atoi(cell_condition.c_str());
							if (getCell > cell_cond){
								condition_metLoc.push_back(j);
								}
						}
					}
					else
						cerr << "The attribute '" << attr_name << "' in relation '" << rel_name << "' is not of int type." << endl;
				}
				else if(condition == "<"){
					if(relation[i].attr[attr_loc].getType() == INT){
						for (int j = 0; j < getCells.size(); ++j){
							int getCell = atoi(getCells[j].c_str());//conversion from string to int
							int cell_cond = atoi(cell_condition.c_str());
							if (getCell < cell_cond){
								condition_metLoc.push_back(j);
								}
						}
					}
					else
						cerr << "The attribute '" << attr_name << "' in relation '" << rel_name << "' is not of int type." << endl;
				}
				else if(condition == "<="){
					if(relation[i].attr[attr_loc].getType() == INT){
						for (int j = 0; j < getCells.size(); ++j){
							int getCell = atoi(getCells[j].c_str());//conversion from string to int
							int cell_cond = atoi(cell_condition.c_str());
							if (getCell <= cell_cond){
								condition_metLoc.push_back(j);
								}
						}
					}
					else
						cerr << "The attribute '" << attr_name << "' in relation '" << rel_name << "' is not of int type." << endl;
				}
				else if(condition == ">="){
					if(relation[i].attr[attr_loc].getType() == INT){
						for (int j = 0; j < getCells.size(); ++j){
							int getCell = atoi(getCells[j].c_str());//conversion from string to int
							int cell_cond = atoi(cell_condition.c_str());
							if (getCell >= cell_cond){
								condition_metLoc.push_back(j);
								}
						}
					}
					else
						cerr << "The attribute '" << attr_name << "' in relation '" << rel_name << "' is not of int type." << endl;
				}
				else if(condition == "=="){
					for (int j = 0; j < getCells.size(); ++j){
						if (getCells[j] == cell_condition){
							condition_metLoc.push_back(j);
							}
					}
				}
				else if(condition == "!="){
					for (int j = 0; j < getCells.size(); ++j){
						if (getCells[j] != cell_condition){
							condition_metLoc.push_back(j);
							}
					}
				}
				else 
					cerr << "Not a valid condition, use '>', '<', or '=='." << endl;
				if(condition_metLoc.size() == 0){
						cerr << "No cells were found to meet that condition in attribute '" << attr_name << "' of relation '" << rel_name << "'." <<endl;
						return;
					}
				else{
					for (int j = 0; j < condition_metLoc.size(); ++j){
						new_selection.Insert(relation[i].getRow(condition_metLoc[j]));
						Selection = new_selection;
					}
				}
				return;
			}
			else cerr << "No attribute '" << attr_name << "' was found in " << rel_name <<"." << endl;
			return;
		}
	}
	cerr << "I did not find anything called "<< rel_name << endl;
}


// Update function is used to update a certain cell that satisfies a particular condition. 

void Database::Update(string rel_name, string attr_name, string literal, string condition_attr, string condition, string condition_literal) {
	Relation* r;
	for(int i = 0; i < relation.size(); i++)
	{
		if(rel_name == relation[i].name)
		{
			r = &relation[i];
			break;
		}
	}
	int attr_loc = r->findAttribute(condition_attr);    // Find the location of the attribute to be changed
	if(attr_loc == -1)
	{
		cerr << "Could not find attribute: " << condition_attr << endl;
		return;
	}
	Relation selection = *r;
	selection.clear_attr_cells();
	selection.name = "Selection";
	vector<string> cells = r->attr[attr_loc].getCells();
	vector<int> condition_metLoc;

  // look for different conditions 
	if(condition == ">"){
		if(r->attr[attr_loc].getType() == INT){
			for (int j = 0; j < cells.size(); ++j){
				int getCell = atoi(cells[j].c_str());//conversion from string to int
				int cell_cond = atoi(condition_literal.c_str());
				if (getCell > cell_cond){
					condition_metLoc.push_back(j);
					}
			}
		}
		else
			cerr << "The attribute '" << condition_attr << "' in relation '" << rel_name << "' is not of int type." << endl;
	}
	else if(condition == "<"){
		if(r->attr[attr_loc].getType() == INT){
			for (int j = 0; j < cells.size(); ++j){
				int getCell = atoi(cells[j].c_str());//conversion from string to int
				int cell_cond = atoi(condition_literal.c_str());
				if (getCell < cell_cond){
					condition_metLoc.push_back(j);
					}
			}
		}
		else
			cerr << "The attribute '" << condition_attr << "' in relation '" << rel_name << "' is not of int type." << endl;
	}
	else if(condition == "<="){
		if(r->attr[attr_loc].getType() == INT){
			for (int j = 0; j < cells.size(); ++j){
				int getCell = atoi(cells[j].c_str());//conversion from string to int
				int cell_cond = atoi(condition_literal.c_str());
				if (getCell <= cell_cond){
					condition_metLoc.push_back(j);
					}
			}
		}
		else
			cerr << "The attribute '" << condition_attr << "' in relation '" << rel_name << "' is not of int type." << endl;
	}
	else if(condition == ">="){
		if(r->attr[attr_loc].getType() == INT){
			for (int j = 0; j < cells.size(); ++j){
				int getCell = atoi(cells[j].c_str());//conversion from string to int
				int cell_cond = atoi(condition_literal.c_str());
				if (getCell >= cell_cond){
					condition_metLoc.push_back(j);
					}
			}
		}
		else
			cerr << "The attribute '" << condition_attr << "' in relation '" << rel_name << "' is not of int type." << endl;
	}
	else if(condition == "=="){
		for (int j = 0; j < cells.size(); ++j){
			if (cells[j] == condition_literal){
				condition_metLoc.push_back(j);
				}
		}
	}
	else if(condition == "!="){
		for (int j = 0; j < cells.size(); ++j){
			if (cells[j] != condition_literal){
				condition_metLoc.push_back(j);
				}
		}
	}
	else 
		cerr << "Not a valid condition, use '>', '<', or '=='." << endl;
	if(condition_metLoc.size() == 0){
			cerr << "No cells were found to meet that condition in attribute '" << condition_attr << "' of relation '" << rel_name << "'." <<endl;
			return;
		}
	else{
		for (int j = 0; j < condition_metLoc.size(); ++j){
			vector<Cell> row = r->getRow(condition_metLoc[j]);
			int attr_loc = r->findAttribute(attr_name);
			row[attr_loc] = literal;
			r->setRow(condition_metLoc[j], row);
		}
	}
	
}

// Delete function is used to delete a certain tuple. it works pretty much like the update function.
void Database::Delete(string rel_name, string condition_attr, string condition, string condition_literal) {
	Relation* r;
	for(int i = 0; i < relation.size(); i++)
	{
		if(rel_name == relation[i].name)
		{
			r = &relation[i];
			break;
		}
	}
	int attr_loc = r->findAttribute(condition_attr);
	if(attr_loc == -1)
	{
		cerr << "Could not find attribute: " << condition_attr << endl;
		return;
	}
	Relation selection = *r;
	selection.clear_attr_cells();
	selection.name = "Selection";
	vector<string> cells = r->attr[attr_loc].getCells();
	vector<int> condition_metLoc;
	if(condition == ">"){
		if(r->attr[attr_loc].getType() == INT){
			for (int j = 0; j < cells.size(); ++j){
				int getCell = atoi(cells[j].c_str());//conversion from string to int
				int cell_cond = atoi(condition_literal.c_str());
				if (getCell > cell_cond){
					condition_metLoc.push_back(j);
					}
			}
		}
		else
			cerr << "The attribute '" << condition_attr << "' in relation '" << rel_name << "' is not of int type." << endl;
	}
	else if(condition == "<"){
		if(r->attr[attr_loc].getType() == INT){
			for (int j = 0; j < cells.size(); ++j){
				int getCell = atoi(cells[j].c_str());//conversion from string to int
				int cell_cond = atoi(condition_literal.c_str());
				if (getCell < cell_cond){
					condition_metLoc.push_back(j);
					}
			}
		}
		else
			cerr << "The attribute '" << condition_attr << "' in relation '" << rel_name << "' is not of int type." << endl;
	}
	else if(condition == "<="){
		if(r->attr[attr_loc].getType() == INT){
			for (int j = 0; j < cells.size(); ++j){
				int getCell = atoi(cells[j].c_str());//conversion from string to int
				int cell_cond = atoi(condition_literal.c_str());
				if (getCell <= cell_cond){
					condition_metLoc.push_back(j);
					}
			}
		}
		else
			cerr << "The attribute '" << condition_attr << "' in relation '" << rel_name << "' is not of int type." << endl;
	}
	else if(condition == ">="){
		if(r->attr[attr_loc].getType() == INT){
			for (int j = 0; j < cells.size(); ++j){
				int getCell = atoi(cells[j].c_str());//conversion from string to int
				int cell_cond = atoi(condition_literal.c_str());
				if (getCell >= cell_cond){
					condition_metLoc.push_back(j);
					}
			}
		}
		else
			cerr << "The attribute '" << condition_attr << "' in relation '" << rel_name << "' is not of int type." << endl;
	}
	else if(condition == "=="){
		for (int j = 0; j < cells.size(); ++j){
			if (cells[j] == condition_literal){
				condition_metLoc.push_back(j);
				}
		}
	}
	else if(condition == "!="){
		for (int j = 0; j < cells.size(); ++j){
			if (cells[j] != condition_literal){
				condition_metLoc.push_back(j);
				}
		}
	}
	else 
		cerr << "Not a valid condition, use '>', '<', or '=='." << endl;
	if(condition_metLoc.size() == 0){
			cerr << "No cells were found to meet that condition in attribute '" << condition_attr << "' of relation '" << rel_name << "'." <<endl;
			return;
		}
	else{
		for (int j = 0; j < condition_metLoc.size(); ++j){
			r->deleteRow(j);
		}
	}
	
}


// this function will be used to delete a particular attribute from the table
void Database::Delete_attr(const string& rel_name,const string& attribute) {
 for (int i = 0; i < relation.size(); ++i){
    if (rel_name == relation[i].name){
      relation[i].Delete_attr(attribute);
      return;
    }
  }
  cerr << "I did not find anything called "<< rel_name << endl;
}
 
 
// This function is used to change the name of a column
void Database:: Rename(string rel_name,string new_name, string old_name) {
  for (int i = 0; i < relation.size(); ++i){
    if (rel_name == relation[i].name){
      relation[i].Rename(new_name,old_name);
      return;
    }
  }
  cerr << "I did not find anything called "<< rel_name << endl;
}
// This function is going to add columns in our table when we initially create a table. 
// This allows us to have tables with different sizes. 
void Database::AddColumn(const string& rel_name, const Header& h){
  for (int i = 0; i < relation.size(); ++i){
    if (rel_name == relation[i].name){
      relation[i].add_attr(h.first,h.second);
      return;
    }
  }
  cerr << "I did not find anything called "<< rel_name << endl;
}

// This function takes in a vector of cells which is our tuple and inserts it in the table
void Database:: Insert(string rel_name, const vector<Cell>& row) {
  for (int i = 0; i < relation.size(); ++i){
    if (rel_name == relation[i].name){
      relation[i].Insert(row);
      return;
    }
  }
  cerr << "I did not find anything called "<< rel_name << endl;
}

// Call the rlation constructor
void Database:: Create(string rel_name) {
  relation.push_back(Relation(rel_name));
}


//Delete a relation from the database
void Database:: Delete(string rel_name) {
  for(int i = 0; i < relation.size(); i++)
  {
	if(rel_name == relation[i].name) {
		relation.erase(relation.begin() + i);
		return;
	}
  }
  cerr << "I did not find anything called " << rel_name << endl;
}

void Database:: Write(const string& rel_name) {
  cout << "Requires file I/O, will be done in the next part " << endl;
}


void Database:: Show(string rel_name) {
  for (int i = 0; i < relation.size(); ++i){
    if (rel_name == relation[i].name){
      relation[i].Show();
      return;
    }
  }
  cerr << "I did not find anything called "<< rel_name << endl;
}
Relation& Database::operator[](const string& s){
  return table[s]; 
}


// Computes the union of two relations and stores it in a new relation
void Database::Union(const string& rel_name1, const string& rel_name2,const string& rel_name3) {
  int i1,i2,i3;

  // Find if the relations exist in the database and store their index
  for(int i = 0; i < relation.size(); i++)
  {
	if(rel_name1 == relation[i].name) {
		relation.erase(relation.begin() + i);
	}
  }
  Create(rel_name1);
  for (int i = 0; i < relation.size(); ++i){
    if (rel_name1 == relation[i].name){
      i1 =i;
      break;
    }
  }
  for (int i = 0; i < relation.size(); ++i){
    if (rel_name2 == relation[i].name){
      i2 =i;
      break;
    }
  }
  for (int i = 0; i < relation.size(); ++i){
    if (rel_name3 == relation[i].name){
      i3 = i;
      break;
    }
  }

  // Check if the number of columns in the two table is the same
  if(relation[i2].getNumAttributes() != relation[i3].getNumAttributes())
  {
	cerr << "Size does not match " << endl;
	return;
	}

  // Check if the attributes have the same type
  for(int i=0; i<relation[i2].getNumAttributes();i++){
    if(relation[i2].getAttributeType(i) != relation[i3].getAttributeType(i)) {
      cerr << "Attributes do not have the same type, cannot insert " << endl;
      return;
    }
  }
  
  for(int i = 0; i < relation[i2].getNumAttributes(); i++)
  {
	Attribute a = relation[i2].getAttribute(i);
	relation[i1].add_attr(a.getName(), a.type);
  }
  
  for(int i = 0; i < relation[i2].getRowSize(); i++)
  {
	Insert(relation[i1].name, relation[i2].getRow(i));
	}


    //Create(rel_name2+rel_name3+"Union");
    bool insrt = true;

    int i2Rowsize = relation[i2].getRowSize();
    int jrowsize = relation[i3].getRowSize();
    for(int j=0; j < jrowsize; j++) {
		bool found = false;
      for(int i=0; i < relation[i2].getRowSize(); i++) {
        if(relation[i3].getRow(j) == relation[i2].getRow(i)) {
		found = true;
          break;
        }
      }
	  if(!found)
		Insert(relation[i1].name,relation[i3].getRow(j));
    }
}

// Compute the difference of the relations

void Database::Difference(const string& rel_name1, const string& rel_name2, const string& rel_name3) {
  int i1,i2,i3;

  // check to see the relations exist in the database.
  for(int i = 0; i < relation.size(); i++)
  {
	if(rel_name1 == relation[i].name) {
		relation.erase(relation.begin() + i);
	}
  }
  Create(rel_name1);
  for (int i = 0; i < relation.size(); ++i){
    if (rel_name1 == relation[i].name){
      i1 =i;
      break;
    }
  }
  for (int i = 0; i < relation.size(); ++i){
    if (rel_name2 == relation[i].name){
      i2 =i;
      break;
    }
  }
  for (int i = 0; i < relation.size(); ++i){
    if (rel_name3 == relation[i].name){
      i3 = i;
      break;
    }
  }
  
  // Check if the attributes have same type and same length
  if(relation[i2].getNumAttributes() != relation[i3].getNumAttributes())
  {
	cerr << "Size does not match " << endl;
	return;
	}
  for(int i=0; i<relation[i2].getNumAttributes();i++){
    if(relation[i2].getAttributeType(i) != relation[i3].getAttributeType(i)) {
      cerr << "Attributes do not have the same type, cannot insert " << endl;
      return;
    }
  }
  
  for(int i = 0; i < relation[i2].getNumAttributes(); i++)
  {
	Attribute a = relation[i2].getAttribute(i);
	relation[i1].add_attr(a.getName(), a.type);
  }
  
  //Create(rel_name1+rel_name2+"Union");
  bool insrt = true;
  
  int i2Rowsize = relation[i2].getRowSize();
  int jrowsize = relation[i3].getRowSize();
  for(int i = 0; i < i2Rowsize; i++)
  {
	bool found = false;
	for(int j = 0; j < jrowsize; j++)
	{
		if(relation[i3].getRow(j) == relation[i2].getRow(i))
		{
			found = true;
			break;
		}
	}
	if(!found)
		Insert(relation[i1].name, relation[i2].getRow(i));
  }
}

// Compute the cross product of two relations
void Database::Product(const string& rel_name1, const string& rel_name2, const string& rel_name3) {
  int i1,i2,i3;

  //find the relations in the database
  for(int i = 0; i < relation.size(); i++)
  {
	if(rel_name1 == relation[i].name) {
		relation.erase(relation.begin() + i);
	}
  }
  Create(rel_name1);
  for (int i = 0; i < relation.size(); ++i){
    if (rel_name1 == relation[i].name){
      i1 =i;
      break;
    }
  }
  for (int i = 0; i < relation.size(); ++i){
    if (rel_name2 == relation[i].name){
      i2 =i;
      break;
    }
  }
  for (int i = 0; i < relation.size(); ++i){
    if (rel_name3 == relation[i].name){
      i3 = i;
      break;
    }
  }
  
  for(int i = 0; i < relation[i2].getNumAttributes(); i++)
  {
	Attribute a = relation[i2].getAttribute(i);
	relation[i1].add_attr(a.getName(), a.type);
  }
  for(int i = 0; i < relation[i3].getNumAttributes(); i++)
  {
	Attribute a = relation[i3].getAttribute(i);
	relation[i1].add_attr(a.getName(), a.type);
  }
  
  //Create(rel_name1+rel_name2+"Union");
  bool insrt = true;
  int i2Rowsize = relation[i2].getRowSize();
  int jrowsize = relation[i3].getRowSize();
  for(int i = 0; i < i2Rowsize; i++)
  {
	for(int j = 0; j < jrowsize; j++)
	{
		vector<Cell> row1 = relation[i2].getRow(i);
		vector<Cell> row2 = relation[i3].getRow(j);
		row1.insert(row1.end(), row2.begin(), row2.end());
		Insert(relation[i1].name, row1);
	}
  }
}
