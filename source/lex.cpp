#include "lex.h"
/********************************************************************
*** NAME : Chen-Wei Hung
*** CLASS : CSc 446
*** ASSIGNMENT : 1
*** DUE DATE : 02/03/2021
*** INSTRUCTOR : George.Hamer
*********************************************************************
*** DESCRIPTION : This is a lexical analyser
********************************************************************/




/********************************************************************
*** FUNCTION process_file
*********************************************************************
*** DESCRIPTION : prcoess file
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void ANALYSER::process_file()
{
	char ch_tmp;
	string lexeme_tmp;
	//fstream infile;
	//infile.open(filename.c_str());
	if (!infile)
	{
		cout << "Fail opening " << filename << endl;
		exit(0);
	}
	while (!infile.eof())
	{
		lexeme_tmp = get_next_token();
		//cout << "token is :[" << lexeme_tmp << "]" << endl;
		if (lexeme_tmp == "--")
		{
			lexeme_tmp.clear();
		}
		else if (lexeme_tmp == "GET_NEXT_TOKEN_ERROR")
		{
			cout << "GET_NEXT_TOKEN_ERROR:"  << lexeme_tmp << endl;
		}
		else
		{
			process_token(lexeme_tmp);
		}
		lexeme_tmp.clear();
		//infile >> noskipws >> ch_tmp;
		//while (ch_tmp == '\n' || ch_tmp > ' ' && !infile.eof())
		//{
		//	lexeme_tmp += ch_tmp;
		//	infile >> noskipws >> ch_tmp;
		//}
		//cout << "lexeme: [" << lexeme_tmp << "]" << endl;
		//process_token(lexeme_tmp);
		//lexeme_tmp.clear();
	}

}


/********************************************************************
*** FUNCTION get_next_token
*********************************************************************
*** DESCRIPTION : read the next token from file
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
string ANALYSER::get_next_token()
{
	//cout << "getnexttoken\n\n";
	char ch_peek = infile.peek();
	//cout << "peek = [" << ch_peek << "]" << endl;

	char ch_tmp;
	string lexeme_tmp;
	while (ch_peek == ' ' || ch_peek == '\n' || ch_peek == '\t')
	{
		infile >> noskipws >> ch_tmp;
		//cout << "skipping [" << ch_tmp << "]" << endl;
		ch_peek = infile.peek();
	}

	if (isalpha(ch_peek))
	{
		infile >> ch_tmp;
		lexeme_tmp += ch_tmp;
		while (true)
		{
			ch_peek = infile.peek();
			if (isalnum(ch_peek) || ch_peek == '_')
			{
				infile >> ch_tmp;
				lexeme_tmp += ch_tmp;
			}
			else
				break;
		}
		return lexeme_tmp;
	}
	else if (isdigit(ch_peek))
	{
		infile >> ch_tmp;
		lexeme_tmp += ch_tmp;
		while (true)
		{
			ch_peek = infile.peek();
			if (isdigit(ch_peek) || ch_peek == '.')
			{
				infile >> ch_tmp;
				lexeme_tmp += ch_tmp;
			}
			else
				break;
		}
		return lexeme_tmp;
	}
	else if (!isalpha(ch_peek) && !isdigit(ch_peek))
	{
		if (ch_peek == '-') //check for comment symbol
		{
			infile >> ch_tmp;
			lexeme_tmp += ch_tmp;
			ch_peek = infile.peek();
			if (ch_peek == '-')
			{
				infile >> ch_tmp;
				lexeme_tmp += ch_tmp;
				process_comment();
				return lexeme_tmp;
			}
			else
				return lexeme_tmp;
		}
		else if (ch_peek == '"')
		{
			process_string_literal();
			return"--";
		}
		else
		{
			infile >> ch_tmp;
			lexeme_tmp += ch_tmp;
			ch_peek = infile.peek();
			if (ch_peek == '=')
			{
				infile >> ch_tmp;
				lexeme_tmp += ch_tmp;
			}
			return lexeme_tmp;
		}

	}
	else
		return "GET_NEXT_TOKEN_ERROR";

}


/********************************************************************
*** FUNCTION process_token
*********************************************************************
*** DESCRIPTION : process the read lexeme into different token
*** INPUT ARGS :string lexeme
*** OUTPUT ARGS :
*** IN/OUT ARGS :string lexeme
*** RETURN :
********************************************************************/
void ANALYSER::process_token(string lexeme)
{
	if (isalpha(lexeme[0]))
	{
		process_word_token(lexeme);
	}
	else if (isdigit(lexeme[0]))
	{
		process_num_token(lexeme);
	}
	else if (!isalnum(lexeme[0]) )
	{
		process_symbol_token(lexeme);
	}
}


/********************************************************************
*** FUNCTION process_word_toekn
*********************************************************************
*** DESCRIPTION : process the word tokens
*** INPUT ARGS : string lexeme
*** OUTPUT ARGS :
*** IN/OUT ARGS :string lexeme
*** RETURN :
********************************************************************/
void ANALYSER::process_word_token(string lexeme)
{
	int num = is_reserved_word(lexeme);
	if (num != -1)
	{
		//cout << "reserverd word : [" << lexeme << "]\n";
		token_list[index] = RESERVED_WORD_TOKEN[num];
		lexeme_list[index] = lexeme;
		index++;
	}
	else
	{
		if (lexeme.size() > 17) {
			token_list[index] = "idt";
			lexeme_list[index] = lexeme;
			attribute[index] = "ERROR, EXCEED MAXINUM LENGTH";
		}
		else {
			token_list[index] = "idt";
			lexeme_list[index] = lexeme;
		}
		index++;
	}
}


/********************************************************************
*** FUNCTION process_symbol_token
*********************************************************************
*** DESCRIPTION : process the symbol tokens
*** INPUT ARGS : string lexe,e
*** OUTPUT ARGS :
*** IN/OUT ARGS :string lexeme
*** RETURN :
********************************************************************/
void ANALYSER::process_symbol_token(string lexeme)
{
	if (is_relop_symbol(lexeme))
	{
		token_list[index] = "relopt";
		lexeme_list[index] = lexeme;
		index++;
		return;
	}
	else if (lexeme == ":=")
	{
		token_list[index] = "assignopt";
		lexeme_list[index] = lexeme;
		index++;
		return;
	}
	else if (lexeme == "+" || lexeme == "-")
	{
		token_list[index] = "addopt";
		lexeme_list[index] = lexeme;
		index++;
		return;
	}
	else if (lexeme == "*" || lexeme == "/" || lexeme == "%")
	{
		token_list[index] = "mulopt";
		lexeme_list[index] = lexeme;
		index++;
		return;
	}
	else if(lexeme == "(")
	{
		token_list[index] = "Lroundbrackett";
		lexeme_list[index] = lexeme;
		index++;
		return;
	}
	else if (lexeme == ")")
	{
		token_list[index] = "Rroundbrackett";
		lexeme_list[index] = lexeme;
		index++;
		return;
	}
	else if (lexeme == ":")
	{
		token_list[index] = "colont";
		lexeme_list[index] = lexeme;
		index++;
		return;
	}
	else if (lexeme == ";")
	{
		token_list[index] = "semicolont";
		lexeme_list[index] = lexeme;
		index++;
		return;
	}
	else if (lexeme == ".")
	{
		token_list[index] = "periodt";
		lexeme_list[index] = lexeme;
		index++;
		return;
	}
	else if (lexeme == "\"")
	{
		token_list[index] = "doublequote";
		lexeme_list[index] = lexeme;
		index++;
		return;
	}
	else if (lexeme == ",")
	{
		token_list[index] = "commat";
		lexeme_list[index] = lexeme;
		index++;
		return;
	}
	else if (lexeme == "\n")
	{
		return;
	}
	else if (lexeme == "\t") {
		return;
	}
	// else
	// {
		// cout << "symbol error is: [" << lexeme << "]" << endl;	
		// token_list[index] = " SYMBOL-ERROR";
		// lexeme_list[index] = lexeme;
		// index++;
		// return;
	// }

}



/********************************************************************
*** FUNCTION process_num_token
*********************************************************************
*** DESCRIPTION : process the number tokens
*** INPUT ARGS :string lexeme
*** OUTPUT ARGS :
*** IN/OUT ARGS :string lexeme
*** RETURN :
********************************************************************/
void ANALYSER::process_num_token(string lexeme)
{
	if (lexeme.find('.') == std::string::npos)
	{
		token_list[index] = "numt";
		lexeme_list[index] = lexeme;
		attribute[index] = lexeme;
		index++;
	}
	else
	{
		if (lexeme[lexeme.size() - 1] == '.') {
			token_list[index] = "unknown";
			lexeme_list[index] = lexeme;
			attribute[index] = "ERROR, UNKNOWN TOKEN";
		}
		else {
			token_list[index] = "numt";
			lexeme_list[index] = lexeme;
			attribute[index] = lexeme;
		}
		index++;

	}
}

/********************************************************************
*** FUNCTION is_reserved_word
*********************************************************************
*** DESCRIPTION : see if the lexeme is reserved word, if not return -1, else return the index of the enumerated list of reserved word
*** INPUT ARGS :string lexeme
*** OUTPUT ARGS :
*** IN/OUT ARGS :string lexeme
*** RETURN :int result
********************************************************************/
int ANALYSER::is_reserved_word(string lexeme)
{
	int result = -1;
	string tmp = lexeme;
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);

	for (int i = 0; i < RESERVED_WORD_COUNT; i++)
	{
		if (tmp == RESERVED_WORD[i])
		{
			result = i;
			break;
		}
	}
	return result;
}



/********************************************************************
*** FUNCTION is_relop_symbol
*********************************************************************
*** DESCRIPTION : see if the lexeme is relop symbbol, if not return -1, else return the index of the enumerated list of relop symbol
*** INPUT ARGS :string lexeme
*** OUTPUT ARGS :
*** IN/OUT ARGS :string lexeme
*** RETURN :int result
********************************************************************/
bool ANALYSER::is_relop_symbol(string lexeme)
{
	bool result = false;
	for (int i = 0; i < RELOP_TOKEN_COUNT; i++)
		if (lexeme == RELOP_TOKEN[i])
		{
			result = true;
			break;
		}
	return result;
}


/********************************************************************
*** FUNCTION display
*********************************************************************
*** DESCRIPTION : display the result
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void ANALYSER::display()
{
	//cout << endl << endl;

	//cout << setw(20) << left << "TOKEN" << setw(40) << left << "LEXEME" << setw(20) << left << "ATTRIBUTE" << endl << endl;
	//for (int i = 0; !token_list[i].empty(); i++)
	//{
	//	cout << setw(20) << left << token_list[i];
	//	cout << setw(40) << left << lexeme_list[i];
	//	cout << setw(20) << left << attribute[i] << endl;
	//}

	fstream outfile;
	outfile.open("tokenlist.dat", ios::out | ios::trunc);

	for (int i = 0; !token_list[i].empty(); i++)
	{
		outfile << token_list[i] << endl;
	}
	outfile << "eoft";
	outfile.close();
	outfile.open("lexemelist.dat", ios::out | ios::trunc);

	for (int i = 0; !lexeme_list[i].empty(); i++)
	{
		outfile << lexeme_list[i] << endl;
	}
	outfile.close();

	outfile.open("attribute.dat", ios::out | ios::trunc);

	for (int i = 0; i < 256; i++)
	{
		outfile << attribute[i] << endl;
	}
	outfile.close();


}

/********************************************************************
*** FUNCTION process_comment
*********************************************************************
*** DESCRIPTION : process the comment in the file
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void ANALYSER::process_comment()
{
	string discard;
	getline(infile, discard);
}


/********************************************************************
*** FUNCTION process_string_literal
*********************************************************************
*** DESCRIPTION :process the string literals in the file
*** INPUT ARGS :
*** OUTPUT ARGS :
*** IN/OUT ARGS :
*** RETURN :
********************************************************************/
void ANALYSER::process_string_literal()
{
	char tmp;
	string literals;
	string tmp_str;
	infile >> tmp;
	literals += tmp;
	char ch_peek = infile.peek();
	//cout << "peek = [" << ch_peek << "]" << endl;

	while (ch_peek != '\n' && ch_peek != '"')
	{
		infile >> noskipws >> tmp;
		literals += tmp;
		ch_peek = infile.peek();
	}
	if (ch_peek == '"') {
		infile >> tmp;
		literals += tmp;
	}
	//getline(infile, tmp_str, '"');
	
	//cout << "tmp_str = " << tmp_str << endl;
	//literals += tmp_str;
	if (literals[literals.size() - 1] == '"') {
		token_list[index] = "literal";
		lexeme_list[index] = literals;
		attribute[index] = literals;
	}
	else {
		token_list[index] = "UNKNOWN";
		lexeme_list[index] = literals;
		attribute[index] = "ERROR, UNKNOWN TOKEN";
	}
	index++;

}
