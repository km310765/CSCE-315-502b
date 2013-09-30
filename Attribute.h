#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "Database.h"

class Parser
{
private:
	Database* d;
public:
	Parser() { d = new Database(); }
	Parser(Database* datab) : d(datab) {}
	void parse(string line, bool print);
};

#endif
