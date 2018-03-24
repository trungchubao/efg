/*
 * counter.h
 *
 *  Created on: Mar 22, 2017
 *      Author: trungchubao
 */

#ifndef COUNT_COUNTER_H_
#define COUNT_COUNTER_H_
#include <gmp.h>
#include <gmpxx.h>
#include "../Parse/parser.h"
#include "../Constants.h"

class Derivation
{
public:
	int length;//number of symbols
	Symbol sym[N_MAX_DERIVATION_LENGTH];//symbols in derivation
	int min_length;//minimum feasible length
};

class Counter
{
public:
	Parser *parser;
	mpz_class model_count[N_MAX_BOUND][N_MAX_BOUND];
	string symbol_name;
	int bound;
	array<Derivation, N_MAX_DERIVATION_QUEUE> queue;
	array<string, N_MAX_DERIVATION_QUEUE> list_count;
	int lst_count_length, exact_count;

	Counter();
	void initialize();
	mpz_class count(string count_exact_length);
	void count_brute_force(string count_exact_length);
	mpz_class count_recursive(int symbol_id, int bound);
	mpz_class count_long_rule_recursive(array <int, N_MAX_RIGHT_HAND_SIDE> symbol_list, int length_list, int length_bound);
	void print_result();
};

#endif /* COUNT_COUNTER_H_ */
