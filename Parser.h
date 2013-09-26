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
public:
	Parser() {}
	void parse(string line);
};

#endif
