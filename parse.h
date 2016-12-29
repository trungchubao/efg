#ifdef __PARSE_H__
#else
#define __PARSE_H__

#define N_MAX 10000             // maximum number of symbols
#define N_MAX_RULE 50            // maximum number of rules
#define BUFF_LENGTH 100			 // length of the buffer to read rules
#define NON_TERMINAL_RULE 1     // type of non-terminal rule is 1
#define TERMINAL_RULE 2			 // type of terminal rule is 2

typedef enum {false, true} bool;

typedef struct {
   bool is_terminal;             // 0: terminal symbol, 1: non-terminal symbol
   int id;
   char name;					 // upper case: non terminal, lower case: terminal
} Symbol;

typedef struct {
	int type;                    // 1: A->BC; 2: A->a
	int lhs_id;                  // left hand side
	int rhs1_id;                 // right hand side
	int rhs2_id;                 // type = 2 -> rhs_id2 = -1
} Rule;

Symbol symbols[N_MAX];
int number_of_symbols;

Rule rules[N_MAX_RULE];
int number_of_rules;

int parse(char* filename);
bool is_upper_case_letter(char c);
bool is_lower_case_letter(char c);
int find_index(char c);
#endif