#include "include_header.h"
#include "parse.h"
#include "count.h"

int count_rec(int symbol_id, int b){
   //printf("abcdef: %d:%d:%d\n",symbol_id, b, model_count[symbol_id][b]);
   if (model_count[symbol_id][b] == -1){
   for (int i=0; i<number_of_rules; i++)
      if (rules[i].lhs_id == symbol_id){
      	if (rules[i].type == 1){
      		if (model_count[symbol_id][b] == -1) model_count[symbol_id][b] = 0;
      		for (int u=1; u<b; u++){
      		   //printf("%c %c %d %d %d\n", symbols[rules[i].rhs1_id].name, symbols[rules[i].rhs2_id].name, u, b-u, model_count[symbol_id][b]);
      		   model_count[symbol_id][b] += count_rec(rules[i].rhs1_id, u)*count_rec(rules[i].rhs2_id, b-u);
      		}
      	}
      	else if (rules[i].type == 2){
      		//printf("%c %d\n",symbols[rules[i].rhs1_id].name, b);
      		if (model_count[symbol_id][b] == -1) model_count[symbol_id][b] = 0;
      		if (b == 1){
      			model_count[symbol_id][b] += 1;
      		}
      	}
      }
  }
     return model_count[symbol_id][b];
}

int count(char symbol_to_count, int bound){
	int symbol = find_index(symbol_to_count);

	//initialize
	for (int i=0; i<number_of_symbols; i++)
		for (int j=0; j<=bound; j++)
			model_count[i][j] = -1;

	for (int i=0; i<number_of_symbols; i++){
		model_count[i][0] = 0;
		if (symbols[i].is_terminal == true){
			model_count[i][1] = 1;
			for (int j=2; j<=bound; j++) model_count[i][j] = 0;
		}
	}
	model_count[symbol][bound] = count_rec(symbol, bound);

	return model_count[symbol][bound];
}