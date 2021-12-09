/********************************************************************
*** NAME : Chen-Wei Hung
*** CLASS : CSc 446
*** ASSIGNMENT : 4
*** DUE DATE : 03/05/2021
*** INSTRUCTOR : George.Hamer
*********************************************************************
*** DESCRIPTION : This is a symbol table for ada compiler using the
*				  implementation of chaining hash table, this program
*				  support inserting an entry with lexeme, token, depth
*				  and union description, writting out the entries with
*				  specific depth value, deletting the entries with specific
*				  depth value, writting out the while symbol table, and
*				  loop up a specific lexeme.
********************************************************************/
#include "symtable.h"
#include "lex.h"
#include "parser.h"
int main(const int argc, const char* argv[])
{
	if (argc != 2)
	{
		cout << "USAGE : " << argv[0] << "FILENAME" << endl;
	}
	else
	{
		ANALYSER scanny(argc, argv);
		scanny.process_file();
		scanny.display();
	}
	RDP parsing(argc, argv);
	parsing.START();
	parsing.table.WriteWholeTable();

	//symbolt.processFile();
	//symbolt.LookUp("id2");

	//symbolt.WriteWholeTable();
	//symbolt.WriteTable(1);
	//symbolt.DeleteDepth(2);
	//cout << "\n\n**********************\nafter delete depth 2\n******************************\n\n";
	//symbolt.WriteWholeTable();



	return 0;
}