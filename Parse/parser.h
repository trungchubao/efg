/*
 * parser.h
 *
 *  Created on: Mar 22, 2017
 *      Author: trungchubao
 */

#ifndef PARSE_PARSER_H_
#define PARSE_PARSER_H_

#include <string>
#include <array>
#include "../Constants.h"

using namespace std;

enum Symbol_Type {Terminal, Nonterminal};

class Symbol
{
public:
	bool is_terminal;
	bool is_epsilon;
	int id;
	string name;
	int min_length = 1;
	int no_of_rule = 0;
	int rule_list[N_MAX_NO_OF_RULE_AS_LHS];
};

class Rule
{
public:
	int lhs_id;
	array<int, N_MAX_RIGHT_HAND_SIDE> rhs;
	int number_of_rhs;
};

class Parser
{
public:
	string filename;
	int number_of_symbols;
	int number_of_rules;
	int number_of_terminals;
	int number_of_non_terminals;
	int parse_error;
	array<Symbol, N_MAX_SYMBOL> symbols;
	array<Symbol, N_MAX_SYMBOL> non_terminals;
	array<Symbol, N_MAX_SYMBOL> terminals;
	array<Rule, N_MAX_RULE> rules;

	Parser();
	void initialize();
	void run();
	int find_index(string symbol);
	string atrim(string line);
	bool include_undefined_symbol(string line);
	bool include_undefined_terminal_symbol(string str);
	void parse_symbols_set(string line, bool is_terminal);
	void parse_set_def(string line, Symbol_Type type);
	void parse_rule(string line);
	void print_out();
};

#endif /* PARSE_PARSER_H_ */
