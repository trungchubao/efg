#ifdef __COUNT_H__
#else
#define __COUNT_H__

typedef struct node{
	int id;
	struct node* left;
	struct node* right;
} Tree;

Tree* root;
int model_count[N_MAX][N_MAX];

int count(char symbol_to_count, int bound);

#endif