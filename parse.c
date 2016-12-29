#include "include_header.h"
#include "parse.h"

int parse(char* filename){
	FILE *f = fopen(filename, "r");

	if (f != NULL){
		char buf[BUFF_LENGTH];
		number_of_symbols = 0;
		number_of_rules = 0;

		while (fgets(buf, sizeof(buf), f) != NULL){
			printf("Line %d: %s\n", number_of_rules, buf);
			char lhs = -1;
			char rhs1 = -1;
			char rhs2 = -1;
			int lhs_id;
			int rhs1_id;
			int rhs2_id;
			int type_of_rule;

			// parse symbols
			for(int i=0; i<strlen(buf); i++){
				if (lhs == -1 && is_upper_case_letter(buf[i]) == true) {lhs = buf[i];}
				else if (rhs1 == -1 && (is_upper_case_letter(buf[i]) == true || is_lower_case_letter(buf[i]) == true)) {rhs1 = buf[i];}
				else if (rhs2 == -1 && (is_upper_case_letter(buf[i]) == true || is_lower_case_letter(buf[i]) == true)) {rhs2 = buf[i];}
			}

			lhs_id = find_index(lhs);
			type_of_rule = NON_TERMINAL_RULE;

			if (lhs_id == -1 && lhs != -1){
				symbols[number_of_symbols].name = lhs;
				symbols[number_of_symbols].id = number_of_symbols;
				symbols[number_of_symbols].is_terminal = is_lower_case_letter(lhs);
				lhs_id = symbols[number_of_symbols].id;
				number_of_symbols++;
			}

			rhs1_id = find_index(rhs1);
			if (rhs1_id == -1 && rhs1 != -1){
				symbols[number_of_symbols].name = rhs1;
				symbols[number_of_symbols].id = number_of_symbols;
				symbols[number_of_symbols].is_terminal = is_lower_case_letter(rhs1);
				if (symbols[number_of_symbols].is_terminal == true) type_of_rule = TERMINAL_RULE;
				rhs1_id = symbols[number_of_symbols].id;
				number_of_symbols++;
			}

			rhs2_id = find_index(rhs2);
			if (rhs2_id == -1 && rhs2 != -1){
				symbols[number_of_symbols].name = rhs2;
				symbols[number_of_symbols].id = number_of_symbols;
				symbols[number_of_symbols].is_terminal = is_lower_case_letter(rhs2);
				rhs2_id = symbols[number_of_symbols].id;
				number_of_symbols++;
			}

			// parse rules
			rules[number_of_rules].lhs_id = lhs_id;
			rules[number_of_rules].rhs1_id = rhs1_id;
			rules[number_of_rules].rhs2_id = rhs2_id;
			rules[number_of_rules].type = type_of_rule;
			number_of_rules++;

		}

		fclose(f);

		printf("number of rules: %d\n", number_of_rules);
		printf("number of symbols: %d\n", number_of_symbols);

		if (number_of_symbols > N_MAX){
			printf("Error: number of symbols exceeds the limit (=%d).\n", N_MAX);
			return 1;
		}

		if (number_of_rules > N_MAX_RULE){
			printf("Error: number of rules exceeds the limit (=%d).\n", N_MAX_RULE);
			return 1;
		}

		return 0;
	}
}

bool is_lower_case_letter(char c){
	if (c >= 'a' && c <= 'z') return true;
	return false;
}

bool is_upper_case_letter(char c){
	if (c >= 'A' && c <= 'Z') return true;
	return false;
}

int find_index(char c){
	for(int i=0; i<number_of_symbols; i++) if (symbols[i].name == c) return symbols[i].id;
	return -1;
}