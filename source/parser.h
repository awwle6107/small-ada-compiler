/********************************************************************
*** NAME : Chen-Wei Hung
*** CLASS : CSc 446
*** ASSIGNMENT : 3
*** DUE DATE : 02/23/2021
*** INSTRUCTOR : George.Hamer
*********************************************************************
*** DESCRIPTION : This is a recursive descent parser
********************************************************************/
#ifndef _PARSER_H
#define _PARSER_H

#include<iostream>
#include<string>
#include<iomanip>
#include<fstream>
#include <algorithm>
#include<stack>
#include "symtable.h"

using namespace std;

class RDP
{
public:
	RDP(const int argc, const char* argv[])
	{
		tokenfile.open("tokenlist.dat");
		lexemefile.open("lexemelist.dat");
		attributefile.open("attribute.dat");
		lineNo = 0;
		stackIndex = 0;
		currentoffset = 0;
		ispara = false;
		isWithPara = false;
		isOut = false;
		isOut2 = false;
		withAtSign = false;
		TACfileName = argv[1];
		TACfileName.erase(TACfileName.find("."));
		TACfileName += ".TAC";
		TACfile.open(TACfileName, ios::out | ios::trunc);
		SPindex = 0;
		dataValueStack[SPindex] = "Ret Address";
		offsetStack[SPindex] = "2";
		SPindex++;
		dataValueStack[SPindex] = "Old BP";
		offsetStack[SPindex] = "0";
		SPindex++;
		tmpCounter = 0;
		procStackIndex = 0;
		asmFileName = TACfileName.erase(TACfileName.find("."));
		asmFileName += ".asm";
			asmFile.open(asmFileName, ios::out | ios::trunc);

	}
	~RDP()
	{
		tokenfile.close();
		lexemefile.close();
	}
	string TACfileName;
	string nextToken;
	string nextlexeme;
	string nextattribute;
	int currentoffset;
	fstream tokenfile;
	fstream lexemefile;
	fstream attributefile;
	fstream TACfile;

	int lineNo;
	EntryType EntryTypeCounter;
	VarType VartypeCounter;
	string lexemestack[100];
	string currentProg;
	int stackIndex;
	bool ispara; 
	bool isWithPara; //see of the current proc has para to determine if is token can be used to increase depth
	bool isOut;	//see if the current id lexeme is addressing more to add @ to string code
	bool isOut2; //used to determine if and inserting id is out mode to modify the paralist nodes bool value isAddressing
	bool withAtSign;


	//for offset notation-----
	string dataValueStack[100];
	string offsetStack[100];
	int SPindex;
	string tmpStack[100];
	int tmpCounter;
	stack <string> parastack;
	///-----------------------

	//----for the procs
	string mainProc;
	string procCallId;//to remeber the current id to write in tac file if there is a proccall
	string procStack[100];
	int procStackIndex;
	//-------------------
	void get_next_token();
	void match(string desiredToken);
	void emit(string code);
	void START();
	void PROG();
	void ARGS();
	void DECLPART();
	void PROCEDURES();
	void SOS();
	void ARGLIST();
	void MODE();
	void IDLIST();
	void TYPEMARK();
	void VALUE();
	void MOREARGS();
	void SeqOfStatement();
	void StatTail();
	void Statement();
	void AssignStat();
	void IOStat();
	void Expr(EntryPtr& syn);
	void Relation(EntryPtr& syn);
	void SimpleExpr(EntryPtr& syn);
	void MoreTerm(EntryPtr& Rsyn);
	void Term(EntryPtr& Tsyn);
	void MoreFactor(EntryPtr& Tsyn);
	void Factor(EntryPtr& Tsyn);
	void ProcCall();
	void Params();
	void ParamsTail(ParamPtr& tmp);
	void setOffsetStack();

	int stackLookUp(string value);

	void writeProc();

	void writeEndProc();

	void In_Stat();

	void Id_LIST();

	void Id_List_Tail();

	void Out_Stat();

	void Write_List();

	void Write_List_Tail();

	void Write_Token();

	void generate_assembly_file();

	void procHandling(string proc);

	string getNextTacWord();


	symbolTable table;

	string asmFileName;
	fstream asmFile;

private:

};




#endif