#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class Token
{
public:
	char kind; // what kind of token
	string str;
	int value;
	Token() : kind(0), str(""), value(0) {}
	Token(char ch) : kind(ch), str(""), value(0) {}
	Token(char ch, string str) : kind(ch), str(str), value(0) {}
	Token(char ch, int val) : kind(ch), str(""), value(val) {}
	Token(char ch, string str, int val) : kind(ch), str(str), value(val) {}
};

class Token_stream
{
private:
	bool empty; // is there a Token in the buffer?
	vector<Token> buffer; // here is where we keep a Token put back using putback()
public: 
	Token_stream() : empty(true) {}
	Token_stream(Token_stream& ts)
	{
		empty = ts.empty;
		buffer = ts.buffer;
		stringstream sscopy;
		sscopy << ts.ss.rdbuf();
		ss.str(sscopy.str());
		ts.ss.str(sscopy.str());
	}
	Token get(); // get a Token
	void putback(Token t); // put a Token back
	void copy(Token_stream& ts)
	{
		empty = ts.empty;
		buffer = ts.buffer;
		stringstream sscopy;
		sscopy << ts.ss.rdbuf();
		ss.str(sscopy.str());
		ts.ss.str(sscopy.str());
	}
	stringstream ss;
};

/*
Type          Kind
integer literal		8 
string literal		"
string				a
assignment			:
comparison			=
equals				~
conjunction			&
condition			|
,					commas are emmitted
other				the character itself
*/

void Token_stream::putback(Token t)
{
	buffer.push_back(t);       // copy t to buffer
	empty = false;      // buffer is now full
}

//------------------------------------------------------------------------------

Token Token_stream::get()
{
	if(!empty) // do we already have a Token ready?
	{
		// remove token from buffer
		Token temp = buffer.back();
		buffer.pop_back();
		if(buffer.empty())
		empty = true;
		return temp;
	}

	char ch;
	ss >> ch; // note that >> skips whitespace (space, newline, tab, etc.)

	switch (ch)
	{
		case ',':
			return get();
		case '(': case ')': case '+': case '*': case ';':
		return Token(ch);        // let each character represent itself
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			{
				ss.putback(ch);         // put digit back into the input stream
				int val;
				ss >> val;              // read an integer
				return Token('8',val);   // let '8' represent "a number"
			}
		break;
		case '-':
			{
				ss.putback(ch);
				string str;
				ss >> str;
				if(str == "-")
					return Token('-');
				for(int i = str.length() - 1; i >= 0; i--)
					ss.putback(str[i]);
				int val;
				ss >> val;
				return Token('8', val);
			}
			break;
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
		case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
		case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
		case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
		case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
		case '_':
			{
				ss.putback(ch);         // put char back into the input stream
				string val;
				string str;
				ss >> str;
				string acceptable = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_1234567890";
				char c;
				while(true)
				{
					c = str[0];
					if(acceptable.find(c) == string::npos)
					{
						for(int i = str.length() - 1; i >= 0; i--)
							ss.putback(str[i]);
						break;
					}
					str = str.substr(1, str.length() - 1);
					val += c;
				} // read a string
				return Token('a', val);   // let 'a' represent "a string"
			}
			break;
		case '>': case '!': case '=':
			{
				ss.putback(ch);         // put char back into the input stream
				string val;
				ss >> val;              // read a string
				if(val == "=")
					return Token('~');   // let '~' represent "equals"
				return Token('=', val); // let '=' represent "comparison"
			}
			break;
		case '<':
			{
				ss.putback(ch);         // put char back into the input stream
				string val;
				ss >> val;              // read a string
				if(val == "<-")
					return Token(':');   // let ':' represent "assignment"
				else if(val == "<=" || val == "<")
					return Token('=', val); // let '=' represent "comparison"
			}
			break;
		case '&': case '|':
			{
				ss.putback(ch);         // put char back into the input stream
				string val;
				ss >> val;              // read a string
				if(val == "||")
					return Token('|'); // let '|' represent "condition"
				else if(val == "&&")
					return Token('&'); // let '&' represent "conjunction"
			}
			break;
		case '"':
			{
				ss.putback(ch);         // put char back into the input stream
				string val;
				ss >> val;              // read a string
				while(val.find('"', 1) == string::npos)
				{
					string val2;
					ss >> val2;
					val = val + ' ' + val2;
				}
				for(int i = val.length() - 1; i >= val.find('"', 1) + 1; i--)
					ss.putback(val[i]);
				return Token('"', val.substr(1, val.find('"', 1) - 1));   // let '"' represent "a literal"
			}
			break;
		default:
			throw string("Bad token");
			break;
	}
}

#endif
