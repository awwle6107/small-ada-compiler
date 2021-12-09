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
#ifndef SYM_H
#define SYM_H
#include <iostream>
#include<string>
#include<fstream>
#include<ios>
using namespace std;
const int TableSize = 211; //use a prime number

enum VarType { charType, intType, floatType };

enum EntryType { constEntry, varEntry, progEntry };

struct ParamNode;

typedef ParamNode* ParamPtr;

struct ParamNode {
    bool isAddressing = false;
    VarType typeOfParameter;
    ParamPtr Next;
};

struct TableEntry;

typedef TableEntry* EntryPtr; //pointer to actual table entry

struct TableEntry {
    string Token;
    string Lexeme;
    int depth;
    EntryType TypeOfEntry; // tag field for the union
    union {
        struct {
            VarType TypeOfVariable;
            int Offset;
            int size;
        } var;
        struct {
            VarType TypeOfConstant; //int or float constant
            int Offset;
            union {
                int Value;
                float ValueR;
            };
        } constant;
        struct { //this is the entry for a function
            int SizeOfLocal;
            int NumberOfParameters;
            VarType ReturnType;
            ParamPtr ParamList; //linked list of paramter types
        } function;
    }; //end of union
    EntryPtr next; //points to next list item
};

//function prototypes for symbol table routines

class symbolTable
{

public:
    EntryPtr *st;

    symbolTable(){
        
        st = new EntryPtr[TableSize];
        for (int i = 0; i < TableSize; i++)
            st[i] = NULL;

        lexemeFile.open("lexemelist.dat");
        tokenFile.open("tokenlist.dat");
        currentDepth = 0;
        offset_counter = 0;
    }
    void processFile();
    void Insert(string lexeme, string token, int depth);
    void WriteTable(int depth);
    void WriteWholeTable();
    EntryPtr LookUp(string lexeme);
    void DeleteDepth(int depth);
    void checkForDups(string lexeme);

    void checkForHappyEnding(string lexeme);

    void checkIfDeclared(string lexeme);

    EntryPtr newTemp();




    void test() { cout << "hello wprld" << endl; }

    fstream lexemeFile;
    fstream tokenFile;
    int currentDepth;


    EntryType EntryTypeCounter;
    VarType VartypeCounter;
    int offset_counter;
    string const_value;



private:
    int hash(string lexeme);
};



#endif
