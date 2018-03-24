/*
 * counter.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: trungchubao
 */

#include <iostream>
#include <gmp.h>
#include <gmpxx.h>
#include "counter.h"

Counter::Counter()
{
	bound = -1;
	symbol_name = "";
	parser = NULL;
}

void Counter::initialize()
{
	for (int i=0; i<parser->number_of_symbols; i++)
		for (int j=0; j<=bound; j++)
			model_count[i][j] = -1;

	for (int i=0; i<parser->number_of_symbols; i++)
	{
		//terminals
		if (parser->symbols[i].is_terminal)
		{
			for (int u=0; u<=bound; u++) model_count[i][u] = 0;
			if (parser->symbols[i].is_epsilon) model_count[i][0] = 1;
			else model_count[i][parser->symbols[i].name.length()] = 1;
		}
	}
}

mpz_class Counter::count(string count_exact_length)
{
	cout << "Counting ...\n";
	int symbol_id = parser->find_index(symbol_name);
	if (symbol_id == -1)
	{
		cout << "Error: symbol to count is undefined!\n";
		return -1;
	}

	initialize();
	for (int lb=0; lb<=bound; lb++)	model_count[symbol_id][lb] = count_recursive(symbol_id, lb);

	mpz_class result = 0;
	if (count_exact_length == "no")
	{
		for (int lb = 0; lb <= bound; lb++) result = result + model_count[symbol_id][lb];
	}
	else
	{
		result = model_count[symbol_id][bound];
	}
	return result;
}

void Counter::count_brute_force(string count_exact_length)
{
	int head, tail;
	int symbol_id;
	int N_MAX_QUEUE = N_MAX_DERIVATION_QUEUE;

	symbol_id = parser->find_index(symbol_name);
	head = -1;
	tail = 0;
	Derivation der;
	der.length = 1;
	der.sym[0] = parser->symbols[symbol_id];
	der.min_length = 1;
	queue[tail] = der;
	while (head != tail)
	{
		//cout << "HEAD:" << head << "; TAIL:" << tail << "\n";
		head++;
		if (head > N_MAX_QUEUE-1) head = 0;
		// get HEAD
		Derivation u = queue[head];

		// propagate
		bool found = false;
		for (int i=0; i<u.length; i++)
		{
			for (int j=0; j<u.sym[i].no_of_rule; j++)
			{
				int min_length_prev = u.sym[i].min_length;
				int min_length_pos = 0;
				Rule current_rule = parser->rules[u.sym[i].rule_list[j]];
				for (int v=0; v<current_rule.number_of_rhs; v++) min_length_pos += parser->symbols[current_rule.rhs[v]].min_length;
				if (u.min_length - min_length_prev + min_length_pos <= bound)
				{
					Derivation d;
					d.min_length = u.min_length - min_length_prev + min_length_pos;
					d.length = u.length - 1 + current_rule.number_of_rhs;
					for (int t=0; t<i; t++) d.sym[t] = u.sym[t];
					for (int t=i; t<i+current_rule.number_of_rhs; t++) d.sym[t] = parser->symbols[current_rule.rhs[t-i]];
					for (int t=i+current_rule.number_of_rhs; t<d.length; t++) d.sym[t] = u.sym[t-current_rule.number_of_rhs+1];

					bool terminated = true;
					string s="";
					for (int t=0; t<d.length; t++)
					{
						s+=d.sym[t].name;
						if (d.sym[t].is_terminal == false)
						{
							terminated = false;
							break;
						}
					}

					if (terminated)
					{
						if (s.length() == bound)
						{
							list_count[lst_count_length] = s;
							lst_count_length++;
						}
					}
					// push to queue
					tail++;
					if (tail > N_MAX_QUEUE-1) tail = 0;
					queue[tail] = d;
					found = true;
				}
			}

			if (found == true) break;
		}
	}
}

void Counter::print_result()
{
	int distinct_list_count[N_MAX_DERIVATION_QUEUE][2];
	int distinct_lst_count_length = 0;
	int considered_already[N_MAX_DERIVATION_QUEUE] = {0};

	for (int i=0; i<lst_count_length; i++)
	{
		if (considered_already[i] == 1) continue;
		considered_already[i] = 1;
		distinct_list_count[distinct_lst_count_length][0] = i;
		distinct_list_count[distinct_lst_count_length][1] = 1;
		for (int j = i+1; j<lst_count_length; j++) if (list_count[i] == list_count[j])
		{
			considered_already[j] = 1;
			distinct_list_count[distinct_lst_count_length][1]++;
		}
		distinct_lst_count_length++;
	}

	cout << "Total number of strings is: " << lst_count_length << "\n";
	cout << "Total number of strings DISTINCT is: " << distinct_lst_count_length << "\n";
	for (int i=0; i<distinct_lst_count_length; i++)
	{
		cout << list_count[distinct_list_count[i][0]] << ":" << distinct_list_count[i][1] << "\n";
	}
}

mpz_class Counter::count_long_rule_recursive(array <int, N_MAX_RIGHT_HAND_SIDE> symbol_list, int length_list, int length_bound)
{
	mpz_class result = 1;

	if (length_bound == 0)
	{
		for (int i=0; i<length_list; i++)
		{
			mpz_class count_part = count_recursive(symbol_list[i],0);
			result = result * count_part;
			if (count_part == 0) break;
		}
		return result;
	}

	if (length_list == 1)
	{
		result = count_recursive(symbol_list[0], length_bound);
		return result;
	}

	result = 0;
	for (int u=0; u<=length_bound; u++)
	{
		mpz_class x, y;
		x = count_recursive(symbol_list[length_list-1], u);
		if (x == 0) continue;

		y = count_long_rule_recursive(symbol_list, length_list - 1, length_bound - u);
		result = result + x*y;
	}
	return result;
}

mpz_class Counter::count_recursive(int symbol_id, int b)
{
	//cout << parser->symbols[symbol_id].name << " of length " << b << "current count: " << model_count[symbol_id][b] << "\n";
	if (model_count[symbol_id][b] == -1)
	{
		for (int i=0; i<parser->number_of_rules; i++)
			if (parser->rules[i].lhs_id == symbol_id)
			{
				switch(parser->rules[i].number_of_rhs)
				{
				case 1:
					if (model_count[symbol_id][b] == -1) model_count[symbol_id][b] = 0;
					model_count[symbol_id][b] += count_recursive(parser->rules[i].rhs[0], b);
					break;
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
					if (model_count[symbol_id][b] == -1) model_count[symbol_id][b] = 0;
					model_count[symbol_id][b] += count_long_rule_recursive(parser->rules[i].rhs, parser->rules[i].number_of_rhs, b);
					break;
				default:
					break;
				}
			}
	}
	//cout << parser->symbols[symbol_id].name << " of length " << b << "final count: " << model_count[symbol_id][b] << "\n";
	return model_count[symbol_id][b];
}
