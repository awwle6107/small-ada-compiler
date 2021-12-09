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

/***********************************************************
 * create a hash value with provided string using hashpjw
 *
 * @param a given lexeme string
 * @return an interger hash value
 **********************************************************/
int symbolTable::hash(string lexeme) {
	char* p;
	unsigned h = 0, g;
	for (p = &lexeme[0]; *p != '\0'; p = p + 1) {
		h = (h << 24) + (*p);
		if (g = h & 0xf0000000) {
			h = h ^ (g >> 24);
			h = h ^ g;
		}
	}
	return h % TableSize;
}




 /***********************************************************
  * insert entry information into symbol table
  *
  * @param a given lexeme string for hash for bucket index, store the lexeme
  * string, token string, and depth int to the entry
  * @return void
  **********************************************************/
void symbolTable::Insert(string lexeme, string token, int depth)
{
	int hash_v = hash(lexeme);
	EntryPtr Entry;
	Entry = new TableEntry;

	Entry->Lexeme = lexeme;
	Entry->Token = token;
	Entry->depth = depth;
	Entry->next = st[hash_v];
	
	st[hash_v] = Entry;
}


/***********************************************************
 * output the entried with specific depth
 *
 * @param a given lexeme string for hash for bucket index, store the lexeme
 * string, token string, and depth int to the entry
 * @return void
 **********************************************************/
void symbolTable::WriteTable(int depth)
{
	cout << "\n******************************\nlooking for depth " << depth << " :\n******************************\n";

	for (int i = 0; i < TableSize; i++) {
		EntryPtr traverse = st[i];

		while (traverse != NULL) {
			if (depth == traverse->depth) {
				cout << "lexeme: " << traverse->Lexeme << "\t";
				cout << "token: " << traverse->Token << "\t";
				cout << "depth = " << traverse->depth << "\t";
				switch (traverse->TypeOfEntry)
				{
				case constEntry:
					cout << "entry type = const\t";
					switch (traverse->constant.TypeOfConstant)
					{
					case charType:
						cout << "consttype = char" << "\t";
						cout << "value = " << traverse->constant.Value << "\t";
						break;
					case intType:
						cout << "consttype = int" << "\t";
						cout << "value = " << traverse->constant.Value << "\t";

						break;
					case floatType:
						cout << "consttype = float" << "\t";
						cout << "value = " << traverse->constant.ValueR << "\t";
						break;
					default:
						break;
					}
					cout << "offset = " << traverse->constant.Offset << endl;
					break;
				case varEntry:
					cout << "entry type = var\t";
					cout << "offset = " << traverse->var.Offset << "\t";
					cout << "size = " << traverse->var.size << "\t";
					switch (traverse->var.TypeOfVariable)
					{
					case charType:
						cout << "vartype = char" << endl;
						break;
					case intType:
						cout << "vartype = int" << endl;
						break;
					case floatType:
						cout << "vartype = float" << endl;
						break;
					default:
						break;
					}
					break;
				case progEntry:
					cout << "entry type = prog" << "\t";
					cout << "size of local = " << traverse->function.SizeOfLocal << "\t";
					cout << "number of parameters = " << traverse->function.NumberOfParameters << '\t';
					cout << "parameter list: ";

					ParamPtr tmp;
					tmp = traverse->function.ParamList;

					while (tmp != NULL) {
						switch (tmp->typeOfParameter)
						{
						case charType:
							cout << "char, ";
							break;
						case intType:
							cout << "integer, ";
							break;
						case floatType:
							cout << "float, ";
							break;
						default:
							break;
						}

						tmp = tmp->Next;
					}
					//here should have travers for linked parameter list
					break;
				default:
					break;
				}
				cout << endl;
			}
			traverse = traverse->next;
		}
	}
}


/***********************************************************
 * output the whole table
 *
 * @param void
 * @return void
 **********************************************************/
void symbolTable::WriteWholeTable()
{
	cout << "\n******************************\nthe whole table\n******************************\n";

	for (int i = 0; i < TableSize; i++) {
		EntryPtr traverse = st[i];

		while (traverse != NULL) {
			cout << "lexeme: " << traverse->Lexeme << "\t";
			cout << "token: " << traverse->Token << "\t";
			cout << "depth = " << traverse->depth << "\t";
			switch (traverse->TypeOfEntry)
			{
			case constEntry:
				cout << "entry type = const\t";
				switch (traverse->constant.TypeOfConstant)
				{
				case charType:
					cout << "consttype = char" << "\t";
					cout << "value = " << traverse->constant.Value << "\t";
					break;
				case intType:
					cout << "consttype = int" << "\t";
					cout << "value = " << traverse->constant.Value << "\t";

					break;
				case floatType:
					cout << "consttype = float" << "\t";
					cout << "value = " << traverse->constant.ValueR << "\t";
					break;
				default:
					break;
				}
				cout << "offset = " << traverse->constant.Offset << endl;
				break;
			case varEntry:
				cout << "entry type = var\t";
				cout << "offset = " << traverse->var.Offset << "\t";
				cout << "size = " << traverse->var.size << "\t";
				switch (traverse->var.TypeOfVariable)
				{
				case charType:
					cout << "vartype = char" << endl;
					break;
				case intType:
					cout << "vartype = int" << endl;
					break;
				case floatType:
					cout << "vartype = float" << endl;
					break;
				default:
					break;
				}
				break;
			case progEntry:
				cout << "entry type = prog" << "\t";
				cout << "size of local = " << traverse->function.SizeOfLocal << "\t";
				cout << "number of parameters = " << traverse->function.NumberOfParameters << '\t';
				cout << "parameter list: ";

				ParamPtr tmp;
				tmp = traverse->function.ParamList;

				while (tmp != NULL) {
					switch (tmp->typeOfParameter)
					{
					case charType:
						cout << "char, ";
						break;
					case intType:
						cout << "integer, ";
						break;
					case floatType:
						cout << "float, ";
						break;
					default:
						break;
					}
					cout << tmp->isAddressing << endl;

					tmp = tmp->Next;
				}
				//here should have travers for linked parameter list
				break;
			default:
				break;
			}
			cout << endl;
			traverse = traverse->next;
		}
	}
}
/***********************************************************
 * find a specific lexeme, and return its pointer
 *
 * @param a given lexeme string for look up
 * @return an entry pointer or null
 **********************************************************/
EntryPtr symbolTable::LookUp(string lexeme)
{
	int hash_v = hash(lexeme);
	return st[hash_v];
}

/***********************************************************
 * delete all entris with given depth
 * @param depth integer
 * @return void
 **********************************************************/
void symbolTable::DeleteDepth(int depth)
{
	for (int i = 0; i < TableSize; i++) {
		EntryPtr tmpPtr = st[i];
		EntryPtr previouPtr = NULL;
		while (tmpPtr != NULL) {
			if (tmpPtr->depth == depth) {
				if (previouPtr != NULL) {
					previouPtr->next = tmpPtr->next;
				}
				else {
					st[i] = st[i]->next;
				}
			}
			else {
				previouPtr = tmpPtr;
			}
			tmpPtr = tmpPtr->next;
		}
	}
}

/***********************************************************
 * check for duplicate id in the same depth
 * @param given a lexeme to look up with
 * @return void
 **********************************************************/
void symbolTable::checkForDups(string lexeme)
{
	EntryPtr pptr;

	pptr = LookUp(lexeme);
	if (pptr && pptr->depth == currentDepth) {
		cout << "Error - duplicate identifier: " << lexeme << endl;
		exit(0);
	}
}

/***********************************************************
 * check for if the procedure ended well
 * @param given a lexeme to look up with
 * @return void
 **********************************************************/
void symbolTable::checkForHappyEnding(string lexeme)
{
	EntryPtr pptr;
	pptr = LookUp(lexeme);
	if (pptr == NULL) {
		cout << "Error - Invalid ending: " << lexeme << endl;
		exit(0);
	}
}

/***********************************************************
 * check for if the lexeme is declared, if not, exit
 * @param given a lexeme to look up with
 * @return void
 **********************************************************/
void symbolTable::checkIfDeclared(string lexeme)
{
	EntryPtr pptr = nullptr;
	pptr = LookUp(lexeme);
	if (pptr == NULL) {
		cout << "Error - undeclared variable: " << lexeme << endl;
		exit(0);
	}
}
/***********************************************************
 * create _t# node for tac file
 * @
 * @return pointer to the generated node
 **********************************************************/
EntryPtr symbolTable::newTemp()
{
	//cout << "-----------------------------------newtmp------------------------------" << endl;
	string temp;
	static int i = 1;
	temp = temp + "_t" + to_string(i);
	i++;
	Insert(temp, "idt", currentDepth);
	if (i > 99) {
		cout << "temp overflow";
		exit(99);
	}
	//cout << "temp = " << temp << endl;
	return LookUp(temp);
}

