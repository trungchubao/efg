/*
 * main.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: trungchubao
 */
////////// EFficient string Generator \\\\\\\\\\
// EFG is a string counter, which receive string constraints as input then
// output the number of strings satisfy that constraints.
// ***Parameter explanation:
//+{-i [input_file]}: to specify name of the file where input CFG is put in.
// This cannot be skipped.

//+{-b [bound]}: to specify length bound to count.
// This cannot be skipped.

//+{-s [symbol_to_count]}: to specify the symbol in the CFG which represents
// the targeted string variable.
// This cannot be skipped

//+{-count_exact_length [yes/no]}: to specify the counting mode, [yes] means
// counting strings of exact length n. Default is [no].

//+{-brute_force_count [yes/no]}: to specify the counting mode, [yes] means
// counting by listing all possible strings. Default is [no].
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\

#include <iostream>
#include <string>
#include <cstring>
#include <gmp.h>
#include <gmpxx.h>
#include "Constants.h"
#include "Parse/parser.h"
#include "Count/counter.h"

using namespace std;

void print_usage()
{
	cout << "Usage: efg -i [input_file] -b [bound] -s [symbol_to_count] -count_exact_length [yes/no] -brute_force_count [yes/no]\n";
}

int main(int argc,char* argv[])
{
	clock_t begin = clock();
	string input_file_name = "";
	int bound = -1;
	string symbol_to_count = "";
	string count_exact_length = "no";
	string brute_force_count = "no";
	Parser *parser = new Parser();
	Counter *counter = new Counter();

	for (int i=0; i<argc; i++)
	{
		if (std::strcmp(argv[i], "-i") == 0 && argc > i+1)
		{
			input_file_name = argv[i+1];
		}
		else if (std::strcmp(argv[i], "-b") == 0 && argc > i+1)
		{
			bound = atoi(argv[i+1]);
		}
		else if (std::strcmp(argv[i], "-s") == 0 && argc > i+1)
		{
			symbol_to_count = argv[i+1];
		}
		else if (std::strcmp(argv[i], "-count_exact_length") == 0 && argc > i+1)
		{
			count_exact_length = argv[i+1];
		}
		else if (std::strcmp(argv[i], "-brute_force_count") == 0 && argc > i+1)
		{
			brute_force_count = argv[i+1];
		}
	}

	//input file
	if (input_file_name == "")
	{
		cout << ERR_MSG_NO_INPUT_FILE;
		print_usage();
		clock_t end_early = clock();
		cout << "Elapsed time: " << double(end_early-begin)*1000/CLOCKS_PER_SEC << " ms\n";
		return 1;
	}

	//bound
	if (bound == -1)
	{
		cout << ERR_MSG_NO_BOUND;
		print_usage();
		clock_t end_early = clock();
		cout << "Elapsed time: " << double(end_early-begin)*1000/CLOCKS_PER_SEC << " ms\n";
		return 1;
	}

	//symbol to count
	if (symbol_to_count == "")
	{
		cout << ERR_MSG_NO_SYMBOL_TO_COUNT;
		print_usage();
		clock_t end_early = clock();
		cout << "Elapsed time: " << double(end_early-begin)*1000/CLOCKS_PER_SEC << " ms\n";
		return 1;
	}

	//count exact length
	if (count_exact_length != "yes" && count_exact_length != "no")
	{
		cout << ERR_MSG_INVALID_COUNT_EXACT_LENGTH_PARAM;
		print_usage();
		clock_t end_early = clock();
		cout << "Elapsed time: " << double(end_early-begin)*1000/CLOCKS_PER_SEC << " ms\n";
		return 1;
	}

	//brute force count
	if (brute_force_count != "yes" && brute_force_count != "no")
	{
		cout << ERR_MSG_INVALID_BRUTE_FORCE_COUNT_PARAM;
		print_usage();
		clock_t end_early = clock();
		cout << "Elapsed time: " << double(end_early-begin)*1000/CLOCKS_PER_SEC << " ms\n";
		return 1;
	}

	//OK to parse
	if (DEBUG_MODE)
	{
		cout << "Parameters OK!\n" << "Start to parse input file.\n";
	}

	parser->filename = input_file_name;
	parser->run();
	if (parser->parse_error == ERR_CODE_PARSE_FAIL)
	{
		cout << "Parse failed!\n";
		clock_t end_early = clock();
		cout << "Elapsed time: " << double(end_early-begin)*1000/CLOCKS_PER_SEC << " ms\n";
		return 1;
	}
	parser->print_out();
	counter->parser = parser;
	counter->symbol_name = symbol_to_count;
	counter->bound = bound;

	mpz_class count_result;

	if (brute_force_count == "no")
	{
		count_result = counter->count(count_exact_length);
		cout << "Counting result for symbol <" << symbol_to_count << "> and length bound " << bound << " is: " << count_result << "\n";
	}
	else
	{
		counter->count_brute_force(count_exact_length);
		counter->print_result();
	}

	clock_t end = clock();
	cout << "Elapsed time: " << double(end-begin)*1000/CLOCKS_PER_SEC << " ms\n";
}
