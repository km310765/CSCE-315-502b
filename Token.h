#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class Token
{
public:
	char kind; // what kind of token
	string str;
	int value;
	Token(char ch) : kind(ch), str(""), value(0) {}
	Token(char ch, string str) : kind(ch), str(str), value(0) {}
	Token(char ch, int val) : kind(ch), str(""), value(val) {}
	Token(char ch, string str, int val) : kind(ch), str(str), value(val) {}
};

class Token_stream
{
private:
	bool full; // is there a Token in the buffer?
	Token buffer; // here is where we keep a Token put back using putback()
public: 
	Token_stream() : full(false), buffer(0) {}
	Token get(); // get a Token
	void putback(Token t); // put a Token back
	stringstream ss;
};

/*
	Type					Kind
	integer	literal			8	
	string literal			"
	string					a
	assignment				:
	comparison				=
	conjunction				&
	condition				|
	,						commas are emmitted
	other					the character itself
*/

void Token_stream::putback(Token t)
{
    if (full) cerr << "putback() into a full buffer" << endl;
    buffer = t;       // copy t to buffer
    full = true;      // buffer is now full
}

//------------------------------------------------------------------------------

Token Token_stream::get()
{
    if(full) // do we already have a Token ready?
	{
        // remove token from buffer
        full = false;
        return buffer;
    }

    char ch;
    ss >> ch; // note that >> skips whitespace (space, newline, tab, etc.)

    switch (ch)
	{
		case ',':
			return get();
		case '(': case ')': case '+': case '-': case '*': case ';':
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
				if(val.find('"', 1) != string::npos)
				{
					for(int i = val.length() - 1; i >= val.find('"', 1) + 1; i--)
						ss.putback(val[i]);
					return Token('"', val.substr(1, val.find('"', 1) - 1));   // let '"' represent "a literal"
				}
				else
					cerr << "Bad token" << endl;
			}
			break;
		default:
			cerr << "Bad token" << endl;
			break;
    }
}
