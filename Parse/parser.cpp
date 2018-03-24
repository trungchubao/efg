/*
 * parser.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: trungchubao
 */

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <sstream>

#include "parser.h"
#include "../Constants.h"

using namespace std;

Parser::Parser()
{
	initialize();
}

void Parser::initialize()
{
	//add epsilon into terminals set
	number_of_terminals = 1;
	Symbol s;
	s.id = 0;
	s.is_epsilon = true;
	s.is_terminal = true;
	s.name = EPSILON;
	terminals[0] = s;

	number_of_symbols = 1;
	symbols[0] = s;

	number_of_rules = 0;
	number_of_non_terminals = 0;
	parse_error = ERR_CODE_PARSE_SUCCESS;
}

void Parser::run()
{
	ifstream input_file;
	input_file.open(filename);

	if (input_file.fail())
	{
		cout << "Error: " << strerror(errno);
		parse_error = ERR_CODE_PARSE_FAIL;
		return;
	}

	//Open success
	if (DEBUG_MODE)
	{
		cout << "File open OK!\n" << "Start to parse input file.\n";
	}

	//Read & parse
	string line;
	while (std::getline(input_file, line))
	{
		cout << line << "\n";

		//case: terminal symbol set definition
		if (line.find(KEYWORD_TERMINAL_SET_DEF) != string::npos)
		{
			if (number_of_rules > 0)
			{
				parse_error = ERR_CODE_PARSE_FAIL;
				cout << ERR_MSG_SYMBOL_SET_DEFINITION_AT_WRONG_POSITION;
				return;
			}

			parse_set_def(line, Terminal);
			if (parse_error == ERR_CODE_PARSE_FAIL) return;
		}
		//case: non-terminal symbol set definition
		else if (line.find(KEYWORD_NON_TERMINAL_SET_DEF) != string::npos)
		{
			if (number_of_rules > 0)
			{
				parse_error = ERR_CODE_PARSE_FAIL;
				cout << ERR_MSG_SYMBOL_SET_DEFINITION_AT_WRONG_POSITION;
				return;
			}

			parse_set_def(line, Nonterminal);
			if (parse_error == ERR_CODE_PARSE_FAIL) return;
		}
		//case: rules
		else
		{
			parse_rule(line);
			if (parse_error == ERR_CODE_PARSE_FAIL) return;
		}
	}

	input_file.close();
	if (DEBUG_MODE)
	{
		cout << "number of rules: " << number_of_rules << "\n";
		cout << "number of symbols: " << number_of_symbols << "\n";
		cout << "number of terminal symbols: " << number_of_terminals << "\n";
		cout << "number of non-terminal symbols: " << number_of_non_terminals << "\n";
	}

	if (number_of_symbols > N_MAX_SYMBOL)
	{
		parse_error = ERR_CODE_PARSE_FAIL;
		cout << ERR_MSG_NUMBER_OF_SYMBOLS_TOO_MANY;
		return;
	}

	if (number_of_rules > N_MAX_RULE)
	{
		parse_error = ERR_CODE_PARSE_FAIL;
		cout << ERR_MSG_NUMBER_OF_RULES_TOO_MANY;
		return;
	}

	if (DEBUG_MODE)
	{
		cout << "Parse Success!\n";
	}
}

int Parser::find_index(string symbol)
{
	for (int i=0; i<number_of_symbols; i++)
	{
		if (symbols[i].name == symbol)
		{
			return symbols[i].id;
		}
	}
	return -1;
}

string Parser::atrim(string line)
{
	unsigned pos = line.find(' ');
	while (pos < line.length())
	{
		line.erase(pos, 1);
		pos = line.find(" ");
	}
	return line;
}

bool Parser::include_undefined_symbol(string line)
{
	for (unsigned i=0; i<line.length(); i++)
	{
		if (SET_ALPHABET.find(line[i]) == string::npos)
		{
			return true;
		}
	}
	return false;
}

bool Parser::include_undefined_terminal_symbol(string str)
{
	for (unsigned i=0; i<str.length(); i++)
	{
		int found = 0;
		for (int j=0; j<number_of_terminals; j++)
			if (terminals[j].name.find(str[i]) != std::string::npos)
			{
				found = 1;
				break;
			}
		if (found == 0) return true;
	}
	return false;
}

void Parser::parse_symbols_set(string line, bool is_terminal)
{
	string symbol_name;
	Symbol s;
	int index;

	unsigned pos1 = 0;
	unsigned pos2 = line.find(DELIMITER);
	if (pos2 == string::npos) pos2 = line.length();

	//loop to last of the line
	while (pos2 != string::npos)
	{
		symbol_name = line.substr(pos1,pos2 - pos1);
		index = find_index(symbol_name);
		if (index == -1)
		{
			s = *(new Symbol());
			s.is_terminal = is_terminal;
			s.is_epsilon = false;
			s.id = number_of_symbols;
			s.name = symbol_name;
			symbols[number_of_symbols] = s;
			number_of_symbols++;
		}
		else
		{
			s = symbols[index];
		}

		if (is_terminal)
		{
			terminals[number_of_terminals] = s;
			number_of_terminals++;
		}
		else
		{
			non_terminals[number_of_non_terminals] = s;
			number_of_non_terminals++;
		}
		if (pos2 >= line.length()) break;
		line.erase(pos2, 1);
		pos1 = pos2;
		pos2 = line.find(DELIMITER);
		if (pos2 == string::npos) pos2 = line.length();
	}
}

void Parser::parse_set_def(string line, Symbol_Type type)
{
	Symbol terminal_s;
	line = atrim(line);

	switch (type)
	{
	case Terminal:
		if (DEBUG_MODE)
		{
			cout << "Parse terminal set definition ...\n";
		}

		//start by adding the delimiter as a terminal symbol
		terminal_s = *(new Symbol());
		terminal_s.id = number_of_symbols;
		number_of_symbols++;
		terminal_s.is_terminal = true;
		terminal_s.is_epsilon = false;
		terminal_s.name = DELIMITER;
		terminals[number_of_terminals] = terminal_s;
		number_of_terminals++;

		//erase keyword and "="
		line.erase(0, KEYWORD_TERMINAL_SET_DEF.length() + 1);

		//erase SYMBOL_SET_DEF_OPEN and SYMBOL_SET_DEF_CLOSE
		line.erase(0, SYMBOL_SET_DEF_OPEN.length());
		line.erase(line.length() - SYMBOL_SET_DEF_CLOSE.length(), SYMBOL_SET_DEF_CLOSE.length());

		if (include_undefined_symbol(line))
		{
			parse_error = ERR_CODE_PARSE_FAIL;
			if (DEBUG_MODE)
			{
				cout << "Error: undefined symbols included in terminal symbols set definition.\n";
			}
			return;
		}

		parse_symbols_set(line, true);
		break;
	case Nonterminal:
		if (DEBUG_MODE)
		{
			cout << "Parse non terminal set definition ...\n";
		}

		//erase keyword and "="
		line.erase(0, KEYWORD_NON_TERMINAL_SET_DEF.length() + 1);

		//erase SYMBOL_SET_DEF_OPEN and SYMBOL_SET_DEF_CLOSE
		line.erase(0, SYMBOL_SET_DEF_OPEN.length());
		line.erase(line.length() - SYMBOL_SET_DEF_CLOSE.length(), SYMBOL_SET_DEF_CLOSE.length());

		if (include_undefined_symbol(line))
		{
			parse_error = ERR_CODE_PARSE_FAIL;
			if (DEBUG_MODE)
			{
				cout << "Error: undefined symbols included in non-terminal symbols set definition.\n";
			}
			return;
		}

		parse_symbols_set(line, false);
		break;
	default:
		break;
	}
}

void Parser::parse_rule(string line)
{
	line = atrim(line);
	if (include_undefined_symbol(line))
	{
		parse_error = ERR_CODE_PARSE_FAIL;
		cout << "Error: undefined symbols included in rules.\n";
		return;
	}

	string lhs;
	int lhs_index;
	int number_of_rhs;
	stringstream ss;
	string r;
	string sb;

	//left hand side
	unsigned equal_sign_pos = line.find("=");
	lhs = line.substr(0, equal_sign_pos);
	lhs_index = find_index(lhs);

	//right hand side
	line.erase(0, equal_sign_pos + 1);

	//split rules
	ss.str(line);
	while (std::getline(ss, r, RULE_SPLIT))
	{
		rules[number_of_rules].lhs_id = lhs_index;
		number_of_rhs = 0;
		stringstream ss1;
		ss1.str(r);
		while (std::getline(ss1, sb, CONCAT))
		{
			rules[number_of_rules].rhs[number_of_rhs] = find_index(sb);
			if (rules[number_of_rules].rhs[number_of_rhs] == -1)
			{
				int l_open = SYMBOL_CONSTANT_STRING_OPEN.length();
				int l_close = SYMBOL_CONSTANT_STRING_CLOSE.length();

				//check if it is constant string
				if (sb.compare(0, l_open, SYMBOL_CONSTANT_STRING_OPEN) == 0 && sb.compare(sb.length()-l_close, l_close, SYMBOL_CONSTANT_STRING_CLOSE) == 0)
				{
					sb = sb.substr(l_open, sb.length()-l_open-l_close);
					if (include_undefined_terminal_symbol(sb))
					{
						parse_error = ERR_CODE_PARSE_FAIL;
						if (DEBUG_MODE)
						{
							cout << ERR_MSG_CONSTANT_STRING_INCLUDE_UNDEFINED_TERMINAL;
						}
						return;
					}

					Symbol s;
					s.id = number_of_symbols;
					s.is_epsilon = false;
					s.is_terminal = true;
					s.name = sb;
					symbols[number_of_symbols] = s;
					terminals[number_of_terminals] = s;
					number_of_symbols++;
					number_of_terminals++;

					rules[number_of_rules].rhs[number_of_rhs] = s.id;
				}
				else
				{
					parse_error = ERR_CODE_PARSE_FAIL;
					if (DEBUG_MODE)
					{
						cout << sb << "\n";
						cout << ERR_MSG_UNDEFINED_SYMBOL;
					}
					return;
				}
			}
			number_of_rhs++;
		}
		if (number_of_rhs > N_MAX_RIGHT_HAND_SIDE)
		{
			parse_error = ERR_CODE_PARSE_FAIL;
			if (DEBUG_MODE)
			{
				cout << ERR_MSG_NUMBER_OF_SYMBOLS_TOO_MANY << "The limit is " << N_MAX_RIGHT_HAND_SIDE << ".\n";
			}
			return;
		}
		rules[number_of_rules].number_of_rhs = number_of_rhs;
		symbols[lhs_index].rule_list[symbols[lhs_index].no_of_rule] = number_of_rules;
		symbols[lhs_index].no_of_rule++;
		number_of_rules++;
	}
}

void Parser::print_out()
{
	//Terminals
	cout << "TERMINALS:\n";
	for (int i=0; i<number_of_terminals; i++)
	{
		cout << "Terminal " << i << ":" << terminals[i].name << "\n";
	}
	//Non-terminals
	cout << "NON-TERMINALS:\n";
	for (int i=0; i<number_of_non_terminals; i++)
	{
		cout << "Non Terminal " << i << ":" << non_terminals[i].name << "\n";
	}
	//Rules
	cout << "RULES:\n";
	for (int i=0; i<number_of_rules; i++)
	{
		int lhs_id = rules[i].lhs_id;
		cout << symbols[lhs_id].name << ":";
		for (int j = 0; j<rules[i].number_of_rhs; j++)
		{
			int rhs_id = rules[i].rhs[j];
			cout << symbols[rhs_id].name;
			if (j<rules[i].number_of_rhs - 1) cout << CONCAT;
		}
		cout << "\n";
	}
}
