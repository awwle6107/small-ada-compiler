/********************************************************************
*** NAME : Chen-Wei Hung
*** CLASS : CSc 446
*** ASSIGNMENT : 3
*** DUE DATE : 02/23/2021
*** INSTRUCTOR : George.Hamer
*********************************************************************
*** DESCRIPTION : This is a recursive descent parser
********************************************************************/
#include "parser.h"
#include "symtable.h"


/********************************************************************
*** FUNCTION get_next_token
*********************************************************************
*** DESCRIPTION : get the next token 
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::get_next_token()
{
	getline(tokenfile, nextToken);
	getline(lexemefile, nextlexeme);
	getline(attributefile, nextattribute);
	//cout << "nexttoken = " << nextToken << endl;
	//cout << "nextlexeme = " << nextlexeme << endl;
}

/********************************************************************
*** FUNCTION match
*********************************************************************
*** DESCRIPTION :match the current token with the desiredToken which is
*				current token should be, if no match, exit program
*** INPUT ARGS :string desiredToken
*** OUTPUT ARGS :
*** IN/OUT ARGS :string desiredToken
*** RETURN :
********************************************************************/
void RDP::match(string desiredToken)
{
	if (nextToken == desiredToken) {
	//	cout << nextToken << endl;
		cout << "matching " << desiredToken << endl;
		get_next_token();
	}
	else {
		cout << "ERROR, expect " << desiredToken << endl;
		exit(0);
	}
}

void RDP::emit(string code)
{
	static int linenum = 0;
	TACfile << code << endl;
	if (true) { // if visual
		if (linenum < 20) {
			cout << code << endl;
		}
		else{
			cout << "Press enter to continue";
			getchar();
			linenum = 1;
			cout << code << endl;
		}
		linenum++;
	}
	code.clear();
}

/********************************************************************
*** FUNCTION START
*********************************************************************
*** DESCRIPTION :start the recursive descent parser
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::START()
{
	get_next_token();
	PROG();
	if (nextToken == "eoft") {
		TACfile << "start	proc	" << mainProc << endl;
		cout << "Successful compiling" << endl;
		cout << "Please see .TAC file for full output" << endl;
		TACfile.close();
		TACfileName += ".TAC";
		TACfile.open(TACfileName, ios::in);
		//table.WriteWholeTable();
		generate_assembly_file();
		exit(0);
	}
	else {
		cout << "unused token - " << nextToken << endl;
		exit(0);
	}

}

/********************************************************************
*** FUNCTION PROG
*********************************************************************
*** DESCRIPTION :execute and parse the grammer PROG
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/

void RDP::PROG()
{
	EntryPtr ptr;
	match("proceduret");
	table.checkForDups(nextlexeme);
	table.Insert(nextlexeme, nextToken, table.currentDepth);
	currentProg = nextlexeme;
	procStack[procStackIndex] = nextlexeme;
	procStackIndex++;
	if (table.currentDepth == 0)
		mainProc = nextlexeme;
	ptr = table.LookUp(nextlexeme);
	ptr->TypeOfEntry = progEntry;
	ptr->function.SizeOfLocal = 0;
	ptr->function.NumberOfParameters = 0;
	ptr->function.ParamList = NULL;
	match("idt");
	ARGS();
	match("ist");
	if (isWithPara == false) {
		//////////////////////////////////////////////////////table.WriteTable(table.currentDepth);
		table.currentDepth++;
		isWithPara = true;
	}
	DECLPART();
	PROCEDURES();
	match("begint");
	writeProc();
	SeqOfStatement();
	//exit depth-----
	cout << "here-------------------------------------\n";
	match("endt");
	writeEndProc();
	////////////////////////////////////////////////////////////////table.WriteTable(table.currentDepth);
	table.currentDepth -= 1;
	//---------------
	table.checkForHappyEnding(nextlexeme);
	match("idt");
	
	match("semicolont");
}

/********************************************************************
*** FUNCTION ARGS
*********************************************************************
*** DESCRIPTION : execute and parse the grammer ARGS
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::ARGS()
{
	if (nextToken == "Lroundbrackett") {
		isWithPara = true;
		////////////////////////////////////////////////////////////////table.WriteTable(table.currentDepth);
		table.currentDepth += 1;
		ispara = true;
		match("Lroundbrackett");
		ARGLIST();
		ispara = false;
		if (table.currentDepth > 1)
			setOffsetStack();
		match("Rroundbrackett");
	}
	else {
		isWithPara = false;
	}
}

/********************************************************************
*** FUNCTION ARGLIST
*********************************************************************
*** DESCRIPTION : execute and parse the grammar ARGLIST
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::ARGLIST()
{
	MODE();
	IDLIST();
	match("colont");
	TYPEMARK();
	MOREARGS();
}

/********************************************************************
*** FUNCTION MODE
*********************************************************************
*** DESCRIPTION : execute and parse the grammar MODE
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::MODE()
{
	if (nextToken == "int") {
		isOut = true;
		isOut2 = true;
		match("int");
	}
	else if (nextToken == "outt") {
		isOut = true;
		isOut2 = true;
		match("outt");
	}
	else if (nextToken == "inoutt") {
		isOut = true;
		isOut2 = true;
		match("inoutt");
	}
}

/********************************************************************
*** FUNCTION DECLPART
*********************************************************************
*** DESCRIPTION : execute and parse the grammar DECLPART
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::DECLPART()
{
	if (nextToken == "idt") {
		IDLIST();
		match("colont");
		TYPEMARK();
		match("semicolont");
		DECLPART();
		if (table.currentDepth > 1)
			setOffsetStack();
	}
}


/********************************************************************
*** FUNCTION PROCEDURES
*********************************************************************
*** DESCRIPTION : execute and parse the grammar PROCEDURES
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::PROCEDURES()
{
	if (nextToken == "proceduret") {
		PROG();
		PROCEDURES();
	}
}

/********************************************************************
*** FUNCTION SOS
*********************************************************************
*** DESCRIPTION : execute and parse the grammar SOS
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::SOS()
{

}

/********************************************************************
*** FUNCTION IDLIST
*********************************************************************
*** DESCRIPTION : execute and parse the grammar IDLIST
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::IDLIST()
{
	int paradepth = table.currentDepth - 1;
	EntryPtr ptr = NULL;
	table.checkForDups(nextlexeme);
	if(ispara)
		table.Insert(nextlexeme, nextToken, table.currentDepth);
	else
		table.Insert(nextlexeme, nextToken, table.currentDepth);
	ptr = table.LookUp(nextlexeme);
	ptr->TypeOfEntry = varEntry;
	lexemestack[stackIndex] = nextlexeme;
	stackIndex++;
	if (ispara) {
		if (isOut) {
			string tmp = "@" + nextlexeme;
			parastack.push(tmp);
			isOut = false;
			tmp.clear();
		}
		else
			parastack.push(nextlexeme);
	}
	else if (table.currentDepth > 1) {
		tmpStack[tmpCounter] = nextlexeme;
		tmpCounter++;
	}
	match("idt");
	if (nextToken != "colont") {
		match("commat");
		IDLIST();
	}
}

/********************************************************************
*** FUNCTION TYPEMARK
*********************************************************************
*** DESCRIPTION : execute and parse the grammar TYPEMARK
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::TYPEMARK()
{
	EntryPtr ptr = NULL;
	if (nextToken == "integert") {
		VartypeCounter = intType;
		for (int i = stackIndex - 1; (i >= 0); i--) {
			ptr = table.LookUp(lexemestack[i]);
			ptr->var.TypeOfVariable = intType;
			ptr->var.Offset = currentoffset;
			currentoffset += 2;
			ptr->var.size = 2;
			if (ispara) {
				/*if (isOut2)
					cout << "isoutomg" << endl;
				else
					cout << "isnotoutomg" << endl;*/
				//insert at the end of the list
				EntryPtr pptr = NULL;
				pptr = table.LookUp(currentProg);
				pptr->function.NumberOfParameters += 1;
				pptr->function.SizeOfLocal += 2;
				ParamPtr tmp;
				tmp = new ParamNode;
				tmp->typeOfParameter = intType;
				if (isOut2) {
					tmp->isAddressing = true;
					isOut2 = false;
				}
				tmp->Next = NULL;
				ParamPtr last;
				last = pptr->function.ParamList; //allocate the head 
				if (pptr->function.ParamList == NULL) {
					pptr->function.ParamList = tmp;
				}
				else {
					while (last->Next != NULL) {
						last = last->Next;
					}
					last->Next = tmp;
				}
				////////////////////////////////////
				//EntryPtr pptr = NULL;
				//pptr = table.LookUp(currentProg);
				//pptr->function.SizeOfLocal += 2;
				//pptr->function.NumberOfParameters += 1;
				//ParamPtr tmp;
				//tmp = new ParamNode;
				//tmp->typeOfParameter = intType;
				//
				//tmp->Next = pptr->function.ParamList;
				//pptr->function.ParamList = tmp;
				//cout << "linked list inserted: " << pptr->function.ParamList->typeOfParameter << endl;
				/////////////////////////////////////////////////////
			}
			stackIndex--;
			lexemestack[i].clear();
		}
		match("integert");
	}
	else if (nextToken == "floatt") {
		VartypeCounter = floatType;
		for (int i = stackIndex - 1; (i >= 0); i--) {
			ptr = table.LookUp(lexemestack[i]);
			ptr->var.TypeOfVariable = floatType;
			ptr->var.Offset = currentoffset;
			currentoffset += 4;
			ptr->var.size = 4;
			if (ispara) {
				//insert at the end of the list
				EntryPtr pptr = NULL;
				pptr = table.LookUp(currentProg);
				pptr->function.NumberOfParameters += 1;
				pptr->function.SizeOfLocal += 4;
				ParamPtr tmp;
				tmp = new ParamNode;
				tmp->typeOfParameter = floatType;
				if (isOut2) {
					tmp->isAddressing = true;
					isOut2 = false;
				}
				tmp->Next = NULL;
				ParamPtr last;
				last = pptr->function.ParamList; //allocate the head 
				if (pptr->function.ParamList == NULL) {
					pptr->function.ParamList = tmp;
				}
				else {
					while (last->Next != NULL) {
						last = last->Next;
					}
					last->Next = tmp;
				}
			}

			//if (ispara) {
			//	EntryPtr pptr = NULL;
			//	pptr = table.LookUp(currentProg);
			//	pptr->function.SizeOfLocal += 4;
			//	pptr->function.NumberOfParameters += 1;
			//	ParamPtr tmp;
			//	tmp = new ParamNode;
			//	tmp->typeOfParameter = floatType;
			//	tmp->Next = pptr->function.ParamList;
			//	pptr->function.ParamList = tmp;
			//	cout << "linked list inserted: " << pptr->function.ParamList->typeOfParameter << endl;
			//}
			stackIndex--;
			lexemestack[i].clear();
		}
		match("floatt");
	}
	else if (nextToken == "chart") {
		VartypeCounter = charType;
		//cout << "stack index = " << stackIndex << endl;
		for (int i = stackIndex - 1; (i >= 0); i--) {
			ptr = table.LookUp(lexemestack[i]);
			ptr->var.TypeOfVariable = charType;
			ptr->var.Offset = currentoffset;
			currentoffset += 1;
			ptr->var.size = 1;
			if (ispara) {
				//insert at the end of the list
				EntryPtr pptr = NULL;
				pptr = table.LookUp(currentProg);
				pptr->function.NumberOfParameters += 1;
				pptr->function.SizeOfLocal += 1;
				ParamPtr tmp;
				tmp = new ParamNode;
				tmp->typeOfParameter = charType;
				if (isOut2) {
					tmp->isAddressing = true;
					isOut2 = false;
				}
				tmp->Next = NULL;
				ParamPtr last;
				last = pptr->function.ParamList; //allocate the head 
				if (pptr->function.ParamList == NULL) {
					pptr->function.ParamList = tmp;
				}
				else {
					while (last->Next != NULL) {
						last = last->Next;
					}
					last->Next = tmp;
				}
			}
			//if (ispara) {
			//	EntryPtr pptr = NULL;
			//	pptr = table.LookUp(currentProg);
			//	pptr->function.SizeOfLocal += 1;
			//	pptr->function.NumberOfParameters += 1;
			//	ParamPtr tmp;
			//	tmp = new ParamNode;
			//	tmp->typeOfParameter = charType;
			//	tmp->Next = pptr->function.ParamList;
			//	pptr->function.ParamList = tmp;
			//	cout << "linked list inserted: " << pptr->function.ParamList->typeOfParameter << endl;
			//}
			stackIndex--;
			lexemestack[i].clear();
		}
		match("chart");
	}
	else if (nextToken == "constantt") {
		ptr = table.LookUp(lexemestack[0]);
		ptr->TypeOfEntry = constEntry;
		ptr->constant.Offset = currentoffset;
		match("constantt");
		match("assignopt");
		VALUE();
		//cout << "here is " << nextToken << endl;

	}
	else {
		cout << "ERROR, expect a typemark\n";
		exit(0);
	}
}

/********************************************************************
*** FUNCTION VALUE
*********************************************************************
*** DESCRIPTION : execute and parse the grammar VALUE
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::VALUE()
{
	EntryPtr ptr;
	ptr = table.LookUp(lexemestack[0]);
	if (nextToken == "numt") {
		if (nextlexeme.find(".") != std::string::npos) {
			ptr->constant.TypeOfConstant = floatType;
			ptr->constant.ValueR = atof(nextlexeme.c_str());
		}
		else {
			ptr->constant.TypeOfConstant = intType;
			ptr->constant.Value = atoi(nextlexeme.c_str());
		}
		lexemestack[0].clear();
		stackIndex--;
		match("numt");
	}
	else if (nextToken == "literal") {
		match("literal");
	}
	else {
		cout << "ERROR, expect constant value\n";
		exit(0);
	}
}

/********************************************************************
*** FUNCTION MOREARGS
*********************************************************************
*** DESCRIPTION : execute and parse the grammar MOREARGS
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::MOREARGS()
{
	if (nextToken == "semicolont") {
		match("semicolont");
		ARGLIST();
	}
}

void RDP::SeqOfStatement()
{
	if (nextToken == "idt" || nextToken == "putt" || nextToken == "gett" || nextToken == "putlnt") {
		Statement();
		match("semicolont");
		StatTail();
	}

}


/********************************************************************
*** FUNCTION StatTail
*********************************************************************
*** DESCRIPTION : execute and parse the grammar StatTail
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::StatTail()
{
	if (nextToken == "idt" || nextToken == "putt" || nextToken == "gett" || nextToken == "putlnt") {
		Statement();
		match("semicolont");
		StatTail();
	}
}


/********************************************************************
*** FUNCTION Statement
*********************************************************************
*** DESCRIPTION : execute and parse the grammar Statement
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::Statement()
{
	if (nextToken == "idt") {
		AssignStat();
	}
	else
		IOStat();
}


/********************************************************************
*** FUNCTION AssignStat
*********************************************************************
*** DESCRIPTION : execute and parse the grammar AssignStat
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::AssignStat()
{
	//cout << "assignstat" << endl;
	EntryPtr idtptr = nullptr;
	EntryPtr syn = nullptr;
	string code;
	idtptr = table.LookUp(nextlexeme);
	procCallId = nextlexeme; //to remeber the current id to write in tac file if there is a proccall
	match("idt");
	if (nextToken == "assignopt") {
		procCallId.clear(); //its not proc call, so clear the stored id
		match("assignopt");
		Expr(syn);
		code.clear();
		if (table.currentDepth > 1) {
			int i = stackLookUp(idtptr->Lexeme);
			if (isOut) {
				code += "@";
				isOut = false;
			}
			code = code + "_bp" + offsetStack[i];
			code += "\t=\t";
			if (syn->Lexeme[0] == '_' || isalpha(syn->Lexeme[0])) {
				i = stackLookUp(syn->Lexeme);
				if (isOut) {
					code += "@";
					isOut = false;
				}
				code = code + "_bp" + offsetStack[i];
			}
			else
				code += syn->Lexeme;
		}
		else {
			code += idtptr->Lexeme;
			code += "\t=\t";
			code += syn->Lexeme;
		}
		emit(code);
	}
	else {
		ProcCall();
	}
	TACfile << endl;
}


/********************************************************************
*** FUNCTION IOStat
*********************************************************************
*** DESCRIPTION : execute and parse the grammar IOStat
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::IOStat()
{
	if (nextToken == "gett") {
		In_Stat();
	}
	else if (nextToken == "putt" || nextToken == "putlnt") {
		Out_Stat();
	}
}

/********************************************************************
*** FUNCTION Expr
*********************************************************************
*** DESCRIPTION : execute and parse the grammar Expr
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::Expr(EntryPtr& syn)
{
	Relation(syn);
}

/********************************************************************
*** FUNCTION Relation
*********************************************************************
*** DESCRIPTION : execute and parse the grammar Relation
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::Relation(EntryPtr& syn)
{
	SimpleExpr(syn);
}

/********************************************************************
*** FUNCTION SimpleExpr
*********************************************************************
*** DESCRIPTION : execute and parse the grammar SimpleExpr
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::SimpleExpr(EntryPtr& syn)
{
	EntryPtr Tsyn = nullptr;
	Term(Tsyn);
	MoreTerm(Tsyn);
	syn = Tsyn;
}

/********************************************************************
*** FUNCTION Term
*********************************************************************
*** DESCRIPTION : execute and parse the grammar Term
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::Term(EntryPtr& Tsyn)
{
	Factor(Tsyn);
	MoreFactor(Tsyn);
}


/********************************************************************
*** FUNCTION MoreTerm
*********************************************************************
*** DESCRIPTION : execute and parse the grammar MoreTerm
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::MoreTerm(EntryPtr& Rsyn)
{
	//cout << "rsyn->lexeme = " << Rsyn->Lexeme << endl;
	//cout << "moreterm" << endl;
	EntryPtr Tsyn = nullptr;
	EntryPtr tmpptr = nullptr;
	string code;
	if (nextToken == "addopt") {
		tmpptr = table.newTemp();
		if (table.currentDepth > 1) {
			if (isalpha(tmpptr->Lexeme[0]) || tmpptr->Lexeme[0] == '_') { //making sure the lexeme is not number
				int i = stackLookUp(tmpptr->Lexeme);
				if (isOut) {
					code += "@";
					isOut = false;
				}
				code = code + "_bp" + offsetStack[i];
			}
			else {
				code += tmpptr->Lexeme;
			}

			code += "\t=\t";
			if (isalpha(Rsyn->Lexeme[0]) || Rsyn->Lexeme[0] == '_') { //making sure the lexeme is not number
				int i = stackLookUp(Rsyn->Lexeme);
				if (isOut) {
					code += "@";
					isOut = false;
				}
				code = code + "_bp" + offsetStack[i];
			}
			else {
				code += Rsyn->Lexeme;
			}
			code = code + "\t" + nextlexeme + "\t"; // strcat the addopt to code
		}
		else {
			code += tmpptr->Lexeme;
			code += "\t=\t";
			code += Rsyn->Lexeme;
			code = code + "\t" + nextlexeme + "\t"; // strcat the addopt to code
		}
		match("addopt");
		Term(Tsyn);

		if (table.currentDepth > 1) {

			if(isalpha(Tsyn->Lexeme[0]) || Tsyn->Lexeme[0] == '_'){
				int i = stackLookUp(Tsyn->Lexeme);
				if (isOut) {
					code += "@";
					isOut = false;
				}
				code = code + "_bp" + offsetStack[i];
			}
			else{
				code += Tsyn->Lexeme;
			}
		}
		else {
			code += Tsyn->Lexeme;
		}
		Rsyn = tmpptr;
		emit(code);
		MoreTerm(Rsyn);
	}
	else {

	}
}

/********************************************************************
*** FUNCTION MoreFactor
*********************************************************************
*** DESCRIPTION : execute and parse the grammar MoreFactor
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::MoreFactor(EntryPtr& Tsyn)
{
	if (nextToken == "mulopt") {

		EntryPtr Mplace = nullptr;
		EntryPtr tmpptr = table.newTemp();
		string code;

		if (table.currentDepth > 1) {
			if (isalpha(tmpptr->Lexeme[0]) || tmpptr->Lexeme[0] == '_') { //making sure the lexeme is not number
				int i = stackLookUp(tmpptr->Lexeme);
				if (isOut) {
					code += "@";
					isOut = false;
				}
				code = code + "_bp" + offsetStack[i];
			}
			else {//when the lexeme is number
				code += tmpptr->Lexeme;
			}

			code += "\t=\t";
			if (isalpha(Tsyn->Lexeme[0]) || Tsyn->Lexeme[0] == '_') {
				int i = stackLookUp(Tsyn->Lexeme);
				if (isOut) {
					code += "@";
					isOut = false;
				}
				code = code + "_bp" + offsetStack[i];
			}
			else {
				code += Tsyn->Lexeme;
			}
			code = code + "\t" + nextlexeme + "\t"; // strcat the mulopt to code

		}
		else {
			code += tmpptr->Lexeme;
			code += "\t=\t";
			code += Tsyn->Lexeme;
			code = code + "\t" + nextlexeme + "\t";//strcat the mulopt to code
		}
		match("mulopt");
		Factor(Mplace);
		if (table.currentDepth > 1) {
			if (isalpha(Mplace->Lexeme[0]) || Mplace->Lexeme[0] == '_') {//making sure the lexeme is not number
				int i = stackLookUp(Mplace->Lexeme);
				if (isOut) {
					code += "@";
					isOut = false;
				}
				code = code + "_bp" + offsetStack[i];
			}
			else { // when the lexeme is number
				code += Tsyn->Lexeme;
			}
		}
		else {
			code += Mplace->Lexeme;
		}
		Tsyn = tmpptr;
		emit(code);
		MoreFactor(Tsyn);
	}
}

/********************************************************************
*** FUNCTION Factor
*********************************************************************
*** DESCRIPTION : execute and parse the grammar Factor
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::Factor(EntryPtr& Tsyn)
{
	if (nextToken == "idt") {
		Tsyn = table.LookUp(nextlexeme);
		match("idt");
	}
	else if (nextToken == "numt") {
		Tsyn = new TableEntry;
		Tsyn->Lexeme = nextlexeme;
		match("numt");
	}
	else if (nextToken == "Lroundbrackett") {
		match("Lroundbrackett");
		Expr(Tsyn);
		match("Rroundbrackett");
	}
	else if (nextToken == "nott") {
		match("nott");
		Factor(Tsyn); // not sure
	}
	else if (nextToken == "addopt") {
		match("addopt");
		Factor(Tsyn); // not sure
	}
}

/********************************************************************
*** FUNCTION ProcCall
*********************************************************************
*** DESCRIPTION : execute and parse the grammar ProcCall
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::ProcCall()
{
	match("Lroundbrackett");
	Params();
	TACfile << "Call\t" << procCallId << endl;
	procCallId.clear();
	match("Rroundbrackett");
}
/********************************************************************
*** FUNCTION Params
*********************************************************************
*** DESCRIPTION : execute and parse the grammar Params
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::Params()
{
	EntryPtr travers = table.LookUp(procCallId);//too look up the paralist nodes from current proc call
	ParamPtr tmp = travers->function.ParamList;
	//cout << "boolean value = " << tmp->isAddressing << endl;
	if (nextToken == "idt") {
		if(tmp->isAddressing)
			TACfile << "push\t@" << nextlexeme << endl;
		else
			TACfile << "push\t" << nextlexeme << endl;
		match("idt");
		ParamsTail(tmp);
	}
	else if (nextToken == "numt"){
		TACfile << "push\t" << nextlexeme << endl;
		match("numt");
		ParamsTail(tmp);
	}
}


/********************************************************************
*** FUNCTION ParamsTail
*********************************************************************
*** DESCRIPTION : execute and parse the grammar ParamsTail
*** INPUT ARGS :ParamPtr& tmp: to inheratance the pointer tmp
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::ParamsTail(ParamPtr& tmp)
{
	if (nextToken == "commat") {
		tmp = tmp->Next;
		match("commat");
		if (nextToken == "idt") {
			if (tmp->isAddressing)
				TACfile << "push\t@" << nextlexeme << endl;
			else
				TACfile << "push\t" << nextlexeme << endl;
			match("idt");
			ParamsTail(tmp);
		}
		else if (nextToken == "numt") {
			TACfile << "push\t" << nextlexeme << endl;

			match("numt");
			ParamsTail(tmp);
		}
	}
}
/********************************************************************
*** FUNCTION setOffsetStack
*********************************************************************
*** DESCRIPTION : build the offset stack for look up and TAC file output
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::setOffsetStack()
{
	static int offset = 0;
	static int BP = 2;
	int sizestack[10] = { 0 }; //for the need to calculate _t offset
	for (int i = 0; i < tmpCounter; i++) {
		dataValueStack[SPindex] = tmpStack[i];
		EntryPtr pptr = nullptr;
		pptr = table.LookUp(tmpStack[i]);
		offset = offset - pptr->var.size;
		sizestack[i] = pptr->var.size;
		offsetStack[SPindex] = to_string(offset);
		SPindex++;
	}

	for (int i = 0; i < tmpCounter; i++) {
		string t = "_t" + to_string(i + 1);
		dataValueStack[SPindex] = t;
		offset = offset - sizestack[i];
		offsetStack[SPindex] = to_string(offset);
		SPindex++;
	}

	for (int i = 0; i < tmpCounter; i++) {
		tmpStack[i].clear();
		sizestack[i] = 0;
	}
	tmpCounter = 0;

	int paracount = 0; //to store _t for parameter values
	while (!parastack.empty()) {
		dataValueStack[SPindex] = parastack.top();
		EntryPtr pptr = nullptr;
		string tmp = parastack.top(); //erase the '@' if it has one
		if (tmp[0] == '@')
			tmp.erase(0, 1);
		pptr = table.LookUp(tmp);
		BP = BP + pptr->var.size;
		sizestack[paracount] = pptr->var.size;
		offsetStack[SPindex] = to_string(BP);
		SPindex++;
		parastack.pop();
		paracount++;
	}

	for (int i = 0; i < paracount; i++) {
		string t = "_t" + to_string(i + 1);
		dataValueStack[SPindex] = t;
		offset = offset - sizestack[i];
		offsetStack[SPindex] = to_string(offset);
		SPindex++;
	}

	cout << "------------------here is the stack---------------" << endl;

	for (int i = 0; i < SPindex; i++) {
		cout << dataValueStack[i] << "\t";
		cout << offsetStack[i] << endl;
	}
}
/********************************************************************
*** FUNCTION stackLookUp
*********************************************************************
*** DESCRIPTION : look up a value's index in the stack
*** INPUT ARGS :string value
*** OUTPUT ARGS :int i
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
int RDP::stackLookUp(string value)
{
	//cout << "stack look up: " << value << endl;
	for (int i = 0; !dataValueStack[i].empty(); i++) {
		string tmp = dataValueStack[i];
		if (tmp[0] == '@') {
			tmp.erase(0, 1);
			if (tmp == value) {
				isOut = true;
				return i;
			}
		}
		else if (tmp == value)
			return i;

	}
	cout << "stack value: "<< value << "no found\n";
	exit(999);
	return -1;
}
/********************************************************************
*** FUNCTION writeProc
*********************************************************************
*** DESCRIPTION : using the procstack to write "proc	procedureID" in TAC file
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::writeProc()
{
	int i = 0;
	for (i = 0; i < 99; i++) {
		if (procStack[i].empty()) {
			i--;
			break;
		}
	}
	if (i < 0) {
		cout << "Error - stack index = -1";
		exit(8);
	}
	if(!procStack[i].empty())
		TACfile << "proc	" << procStack[i] << endl;
}
/********************************************************************
*** FUNCTION writeProc
*********************************************************************
*** DESCRIPTION : using the procstack to write "endp	procedureID" in TAC file
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void RDP::writeEndProc()
{
	int i = 0;
	for (i = 0; i < 99; i++) {
		if (procStack[i].empty()) {
			i--;
			break;
		}
	}
	if (i < 0) {
		cout << "Error - stack index = -1";
		exit(8);
	}
	if (!procStack[i].empty())
		TACfile << "endp	" << procStack[i] << endl;
	procStack[i].clear();
}

void RDP::In_Stat()
{
	match("gett");
	TACfile << "rdi_bp"; //for rdi in assembly
	match("Lroundbrackett");
	Id_LIST();
	match("Rroundbrackett");
}

void RDP::Id_LIST()
{
	int i = stackLookUp(nextlexeme); //the nextlexeme is the id lexeme
	TACfile << offsetStack[i] << endl;
	match("idt");
	Id_List_Tail();
}

void RDP::Id_List_Tail()
{
	if (nextToken == "commat") {
		match("commat");
		match("idt");
		Id_List_Tail();
	}

}

void RDP::Out_Stat()
{
	static int S_count = 0; //for wrs_S0 output

	if (nextToken == "putt") {
		match("putt");
		TACfile << "wrs_S" << S_count << endl;//for wrs_S0 output
		S_count++;
		match("Lroundbrackett");
		Write_List();
		match("Rroundbrackett");
	}
	else if (nextToken == "putlnt") {
		match("putlnt");
		TACfile << "wri_bp"; //for wri_in assembly for putln
		match("Lroundbrackett");
		Write_List();
		match("Rroundbrackett");
	}

}

void RDP::Write_List()
{

	Write_Token();
	Write_List_Tail();
}

void RDP::Write_List_Tail()
{
	if (nextToken == "commat") {
		match("commat");
		Write_Token();
		Write_List_Tail();
	}
}

void RDP::Write_Token()
{
	if (nextToken == "idt") {
		int i = stackLookUp(nextlexeme); //nextlexeme is current id lexeme
		TACfile << offsetStack[i] << endl;
		TACfile << "wrln" << endl;
		match("idt");
	}
	else if (nextToken == "numt")
		match("numt");
	else if (nextToken == "literal")
		match("literal");
}

void RDP::generate_assembly_file()
{
	if (TACfile)
		cout << "oope";
	while (!TACfile.eof())
	{
		string TacWord = getNextTacWord();
		if (TacWord == "eof")
			exit(1);

		if (TacWord == "proc") {
			procHandling(TacWord);
		}



	}
}

void RDP::procHandling(string proc) 
{
	string procId = getNextTacWord();
	asmFile << procId << "\t";
	asmFile << proc << endl;
	asmFile << "push\tBP" << endl;
	asmFile << "mov\tBP,SP" << endl;
	asmFile << "SP,0" << endl; //need fixing
} 

string RDP::getNextTacWord()
{
	if (TACfile.eof())
		return "eof";
	char ch_peek = TACfile.peek();
	//cout << "peek = [" << ch_peek << "]" << endl;

	char ch_tmp;
	string word_tmp;
	while (ch_peek == ' ' || ch_peek == '\n' || ch_peek == '\t')
	{
		if (TACfile.eof())
			return "eof";
		TACfile >> noskipws >> ch_tmp;
		//cout << "skipping [" << ch_tmp << "]" << endl;
		ch_peek = TACfile.peek();
	}

	TACfile >> ch_tmp;
	word_tmp += ch_tmp;
	while (true) {
		if (TACfile.eof())
			return "eof";
		ch_peek = TACfile.peek();
		if (ch_peek == ' ' || ch_peek == '\n' || ch_peek == '\t')
			break;
		else {
			TACfile >> ch_tmp;
			word_tmp += ch_tmp;
		}
	}
	return word_tmp;
}