/*
 * Constants.h
 *
 *  Created on: Apr 10, 2017
 *      Author: trungchubao
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <list>
#include <string>

using namespace std;

static bool DEBUG_MODE = true;
//Error messages
static string ERR_MSG_NO_INPUT_FILE = "Error: input file is not specified.\n";
static string ERR_MSG_NO_BOUND = "Error: bound is not specified.\n";
static string ERR_MSG_NO_SYMBOL_TO_COUNT = "Error: symbol to count is not specified.\n";
static string ERR_MSG_INVALID_COUNT_EXACT_LENGTH_PARAM = "Error: count_exact_length parameter is not set correctly.\n";
static string ERR_MSG_INVALID_BRUTE_FORCE_COUNT_PARAM = "Error: brute_force_count parameter is not set correctly.\n";
static string ERR_MSG_SYMBOL_SET_DEFINITION_AT_WRONG_POSITION = "Error: symbol set definition should be placed before rule definitions.\n";
static string ERR_MSG_NUMBER_OF_SYMBOLS_TOO_MANY = "Error: number of symbols exceeds the limit.\n";
static string ERR_MSG_NUMBER_OF_RULES_TOO_MANY = "Error: number of rules exceeds the limit.\n";
static string ERR_MSG_CONSTANT_STRING_INCLUDE_UNDEFINED_TERMINAL = "Error: constant string includes undefined terminals.\n";
static string ERR_MSG_UNDEFINED_SYMBOL = "Error: undefined symbols are used in rules.\n";

//Error codes
static int ERR_CODE_PARSE_FAIL = 1;
static int ERR_CODE_PARSE_SUCCESS = 0;

static const int N_MAX_SYMBOL = 300;
static const int N_MAX_RULE = 1000;
static const int N_MAX_BOUND = 10000;
static const int N_MAX_RIGHT_HAND_SIDE = 10;
static const int BUFF_LENGTH = 100;
static const int N_MAX_DERIVATION_LENGTH = 1000;
static const int N_MAX_NO_OF_RULE_AS_LHS = 100;
static const int N_MAX_DERIVATION_QUEUE = 6000;

//patterns
static string SYMBOL_SET_DEF_OPEN = "(|";
static string SYMBOL_SET_DEF_CLOSE = "|)";
static string SYMBOL_CONSTANT_STRING_OPEN = "\"";
static string SYMBOL_CONSTANT_STRING_CLOSE = "\"";

//keywords
static string KEYWORD_TERMINAL_SET_DEF = "Terminal";
static string KEYWORD_NON_TERMINAL_SET_DEF = "Non-terminal";

//sets & special symbols
static string SET_ALPHABET = "?<>/\"\\&%!^':;[]{}()<>|.=+-*,abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_~";
static string DELIMITER = ",";
static string EPSILON = "<>";
static char CONCAT = '.';
static char RULE_SPLIT = '|';

#endif /* CONSTANTS_H_ */
