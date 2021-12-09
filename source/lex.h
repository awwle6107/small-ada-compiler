#ifndef _LEX_H
#define _LEX_H
/********************************************************************
*** NAME : Chen-Wei Hung
*** CLASS : CSc 446
*** ASSIGNMENT : 1
*** DUE DATE : 02/03/2021
*** INSTRUCTOR : George.Hamer
*********************************************************************
*** DESCRIPTION : This is a lexical analyser
********************************************************************/

using namespace std;
#include<iostream>
#include<string>
#include<iomanip>
#include<fstream>
#include <algorithm>


const int RESERVED_WORD_COUNT = 27;
const int RELOP_TOKEN_COUNT = 6;

class ANALYSER
{
public:

/********************************************************************
*** FUNCTION ANALYSER CONSTRUCTOR
*********************************************************************
*** DESCRIPTION :set the file name from argv[1], opening file, and initialize variables
*** INPUT ARGS :const int argc, const char* argv[]
*** OUTPUT ARGS :
*** IN/OUT ARGS :const int argc, const char* argv[]
*** RETURN :
********************************************************************/
	ANALYSER(const int argc, const char* argv[])
	{
		filename = argv[1];
		index = 0;
		infile.open(filename);
		if (!infile)
		{
			cout << "Fail opening " << filename << endl;
			exit(0);
		}
		else
		{
			cout << "opening file: " << filename << endl;
		}
	}

	fstream infile;

/********************************************************************
*** FUNCTION process_file
*********************************************************************
*** DESCRIPTION : prcoess file
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
	void process_file();


/********************************************************************
*** FUNCTION display
*********************************************************************
*** DESCRIPTION : display the result
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
	void display();


private:
	const string RESERVED_WORD[RESERVED_WORD_COUNT] =
	{ "BEGIN", "MODULE", "CONSTANT", "PROCEDURE", "IS", "IF", "THEN", "ELSE", "ELSIF", "WHILE", "LOOP",
		"FLOAT", "INTEGER", "CHAR", "GET", "PUT", "PUTLN", "END", "IN", "OUT", "INOUT", "NOT" , "MOD", "REM", "AND", "OR" };
	const string RESERVED_WORD_TOKEN[RESERVED_WORD_COUNT] =
	{ "begint", "modulet", "constantt", "proceduret", "ist", "ift", "thent", "elset", "elsift", "whilet", "loopt",
		"floatt", "integert", "chart", "gett", "putt", "putlnt", "endt", "int", "outt", "inoutt", "nott" , "mulopt", "mulopt", "mulopt", "addopt" };

	const string RELOP_TOKEN[RELOP_TOKEN_COUNT] = { "=", "/=", ">", ">=", "<", "<=" }; 

	string token_list[256];
	string lexeme_list[256];
	string attribute[256];
	string filename;
	int index;

	/********************************************************************
	*** FUNCTION get_next_token
	*********************************************************************
	*** DESCRIPTION : read the next token from file
	*** INPUT ARGS :
	*** OUTPUT ARGS :
	*** IN/OUT ARGS :
	*** RETURN :
	********************************************************************/
	string get_next_token();

/********************************************************************
*** FUNCTION process_token
*********************************************************************
*** DESCRIPTION : process the read lexeme into different token
*** INPUT ARGS :string lexeme
*** OUTPUT ARGS :
*** IN/OUT ARGS :string lexeme
*** RETURN :
********************************************************************/
	void process_token(string lexeme);


/********************************************************************
*** FUNCTION process_word_toekn
*********************************************************************
*** DESCRIPTION : process the word tokens
*** INPUT ARGS : string lexeme
*** OUTPUT ARGS :
*** IN/OUT ARGS :string lexeme
*** RETURN :
********************************************************************/
	void process_word_token(string lexeme);

	/********************************************************************
	*** FUNCTION process_symbol_token
	*********************************************************************
	*** DESCRIPTION : process the symbol tokens
	*** INPUT ARGS : string lexe,e
	*** OUTPUT ARGS :
	*** IN/OUT ARGS :string lexeme
	*** RETURN :
	********************************************************************/
	void process_symbol_token(string lexeme);

/********************************************************************
*** FUNCTION process_num_token
*********************************************************************
*** DESCRIPTION : process the number tokens
*** INPUT ARGS :string lexeme
*** OUTPUT ARGS :
*** IN/OUT ARGS :string lexeme
*** RETURN :
********************************************************************/
	void process_num_token(string lexeme);

/********************************************************************
*** FUNCTION is_reserved_word
*********************************************************************
*** DESCRIPTION : see if the lexeme is reserved word, if not return -1, else return the index of the enumerated list of reserved word
*** INPUT ARGS :string lexeme
*** OUTPUT ARGS :
*** IN/OUT ARGS :string lexeme
*** RETURN :int result
********************************************************************/
	int is_reserved_word(string lexeme);

	/********************************************************************
*** FUNCTION is_relop_symbol
*********************************************************************
*** DESCRIPTION : see if the lexeme is relop symbbol, if not return -1, else return the index of the enumerated list of relop symbol
*** INPUT ARGS :string lexeme
*** OUTPUT ARGS :
*** IN/OUT ARGS :string lexeme
*** RETURN :int result
********************************************************************/
	bool is_relop_symbol(string lexeme);


	/********************************************************************
*** FUNCTION process_comment
*********************************************************************
*** DESCRIPTION : process the comment in the file
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
	void process_comment();

	/********************************************************************
*** FUNCTION process_string_literal
*********************************************************************
*** DESCRIPTION :process the string literals in the file
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
	void process_string_literal();


};


#endif