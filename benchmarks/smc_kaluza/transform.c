#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

const char const_dir[] = "./smc_big/";
const int MAX_PWD_SIZE = 100;
const int BUFF_SIZE = 256;
const int MAX_NUM_VAR = 1000;
const int MAX_NUM_VAR_IN_ONE_CONSTRAINT = 10;
const int MAX_VAR_LEN = 50;
const int MAX_NUM_RULE = 500;


int is_empty_string (char str[BUFF_SIZE]) {
	for (int i=0; i<strlen(str); i++)
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n') return 0;
	return 1;
}

void main() {
	// Read directory
	DIR* dir;
	struct dirent* in_file;
	FILE *entry_file;
	int file_count = 0;
	int query_found = 0;
	int number_of_rules;
	int number_of_vars;
	int number_of_vars_in_one_constraint;
	int max_number_of_vars = 0;
	int count_wildcard_rule = 0;
	int in_quote;
	int in_regex;
	int constraint_type;
	int has_negation;
	int has_concatenation;
	int has_comparator; //<=>
	int has_constant;
	int has_regex;
	int has_len;
	int has_query;
	int root;
	int graph[MAX_NUM_VAR+1][MAX_NUM_VAR+1]; //graph[i,j] = 1 means var[i] = v[j].smth, graph[i,0] = 1 means var[i] = const, graph[i,j] = 2 means var[i] = var[j]
	int deg_in[MAX_NUM_VAR+1];
	int deg_out[MAX_NUM_VAR+1];
	char *buffer;
	char *var[MAX_NUM_VAR];
	char *rules[MAX_NUM_VAR+1][MAX_NUM_RULE];
	int num_rule[MAX_NUM_VAR+1];
	int  var_in_one_constraint[MAX_NUM_VAR_IN_ONE_CONSTRAINT];
	int queue[MAX_NUM_VAR], in_queue[MAX_NUM_VAR];
	int head, tail;

	if (NULL == (dir = opendir(const_dir))) {
		printf("Read directory failed!\n");

		return;
	}

	while ((in_file = readdir(dir))) {

		// Process current file
		number_of_rules = 0;
		number_of_vars = 1; //start from 1

		if (strcmp(in_file->d_name, ".") == 0 || strcmp(in_file->d_name, "..") == 0) continue;

		char pwd[MAX_PWD_SIZE];
		strcpy(pwd, const_dir);
		strcat(pwd, in_file->d_name);

		entry_file = fopen(pwd, "r");
		if (entry_file == NULL) continue;

		// Initialize graph
		for (int i=0; i<MAX_NUM_VAR+1; i++)
			for (int j=0; j<MAX_NUM_VAR+1; j++) graph[i][j] = 0;

		for (int i=0; i<MAX_NUM_VAR+1; i++) {deg_in[i]=0; deg_out[i]=0; num_rule[i]=0;}
		query_found = 0;
		while (buffer=malloc(sizeof(char)*BUFF_SIZE), fgets(buffer, BUFF_SIZE, entry_file) != NULL)
		{
			if (!is_empty_string(buffer)) {
				number_of_rules++;
				if (buffer[strlen(buffer)-1] == '\n') buffer[strlen(buffer)-1] = 0;

				////////////////////// Start Parsing \\\\\\\\\\\\\\\

				constraint_type = 0;
				has_regex = 0;
				has_constant = 0;
				has_comparator = 0;
				has_concatenation = 0;
				has_negation = 0;
				has_len = 0;
				has_query = 0;
				number_of_vars_in_one_constraint = 0;

				// string starts with '!'
				if (buffer[0] == '!') {
					has_negation = 1;

					int last_bracket = strlen(buffer) - 1;
					int first_bracket = 1;
					while (buffer[last_bracket] != ')') last_bracket--;
					while (buffer[first_bracket] != '(') first_bracket++;

					buffer[last_bracket] = 0;
					buffer = buffer + first_bracket + 1;
				}

				// parse to get all the variables and type of constraint
				char *ptr = buffer;
				while (*ptr != 0) {
					switch (*ptr) {
						case 'l': //len
							ptr+=3;
							has_len = 1;
							break;
						case 'i':
							ptr+=2;
							has_regex = 1;
							break;
						case 'q':
							ptr+=5;
							has_query = 1;
							break;
						case '"':
							has_constant = 1;
							ptr++;
							while (*ptr != '"') {
								ptr++;
							}
							ptr++;
							break;
						case '/':
							ptr++;
							while (*ptr != '/') {
								ptr++;
							}
							ptr++;
							break;
						case '<':
						case '>':
						case '=':
							has_comparator = 1;
							ptr++;
							break;
						case '.':
							has_concatenation = 1;
							ptr++;
							break;
						default:
							if ((*ptr >= 'a' && *ptr <= 'z') || (*ptr >= 'A' && *ptr <= 'Z')) {
								int var_len = 1;
								while (*(ptr+var_len) != ' ' && *(ptr+var_len) != '<' && *(ptr+var_len) != '=' && *(ptr+var_len) != 0 &&
									   *(ptr+var_len) != '>' && *(ptr+var_len) != '.' && *(ptr+var_len) != '(' && *(ptr+var_len) != ')') var_len++;

								var[number_of_vars] = malloc(sizeof(char)*(var_len+1));
								memcpy(var[number_of_vars], ptr, var_len);
								var[number_of_vars][var_len] = 0;

								int existed = -1;
								for (int i=1; i<number_of_vars; i++)
									if (strcmp(var[number_of_vars], var[i]) == 0) {existed = i; break;}

								if (existed == -1) {
									var_in_one_constraint[number_of_vars_in_one_constraint] = number_of_vars;
									number_of_vars_in_one_constraint++;
									number_of_vars++;
								}else {
									var_in_one_constraint[number_of_vars_in_one_constraint] = existed;
									number_of_vars_in_one_constraint++;
								}
								ptr+=var_len;
							}else {
								ptr++;
							}

					}
				}

				// build graph
				if (!has_len) { //skip len constraints
					if (!has_regex) { //skip regex
						if (has_concatenation) {
							if (number_of_vars_in_one_constraint == 3) {
								graph[var_in_one_constraint[0]][var_in_one_constraint[1]] = 1;
								graph[var_in_one_constraint[0]][var_in_one_constraint[2]] = 1;
								deg_out[var_in_one_constraint[0]]+=2;
								deg_in[var_in_one_constraint[1]]+=1;
								deg_in[var_in_one_constraint[2]]+=1;
							}
						} else {
							if (has_comparator) {
								if (has_constant) {
									graph[var_in_one_constraint[0]][0] = 1;
									rules[var_in_one_constraint[0]][num_rule[var_in_one_constraint[0]]] = malloc(sizeof(char)*BUFF_SIZE);
									memcpy(rules[var_in_one_constraint[0]][num_rule[var_in_one_constraint[0]]], buffer, strlen(buffer));
									num_rule[var_in_one_constraint[0]]++;
								} else {
									graph[var_in_one_constraint[0]][var_in_one_constraint[1]] = 2;
									graph[var_in_one_constraint[1]][var_in_one_constraint[0]] = 2;
								}
							} else {
								if (has_query) {
									root = var_in_one_constraint[0];
								}
							}

						}
					}
				}
			}
		}

		fclose(entry_file);
		max_number_of_vars = (max_number_of_vars < number_of_vars ? number_of_vars : max_number_of_vars);
		file_count++;

		// check the relationship in the graph
		int check = 1;
		int file_name_printed = 0;
		for (int i=1; i<number_of_vars; i++)
			if (deg_out[i]>0 && graph[i][0]==1) {
				//if (!file_name_printed) {printf("file_name:%s\n", pwd); file_name_printed=1;}
				//printf("variable name:%s\n", var[i]);
				//for (int j=0; j<num_rule[i]; j++) printf("rule %d: %s\n",j,rules[i][j]);
				check = 0;
				//break;
			} 
		int count=0;
		for (int i=1; i<number_of_vars; i++)
			if (graph[root][i] == 2 && deg_in[i] >0) {
				count++;
			}

		for (int i=1; i<number_of_vars; i++) in_queue[i] = 0;
		queue[0] = root;
		head = 0;
		tail = 0;
		int v;

		while (head <= tail){
			v = queue[head];
			head++;

			for (int u=1; u<number_of_vars; u++)
				if (graph[v][u] == 1) {
					if (!in_queue[u]) {
						tail++;
						queue[tail] = u;
						in_queue[u] = 1;
					} else {
						if (!file_name_printed) {
							printf("FILE NAME: %s\n", pwd);
							file_name_printed = 1;
						}
						printf("var 1: %s, var 2: %s\n", var[v], var[u]);
					}
				} else if (graph[v][u] == 2 && !in_queue[u]) {
					tail++;
					queue[tail] = u;
					in_queue[u] = 1;
				}
		}

	}

	return;
}