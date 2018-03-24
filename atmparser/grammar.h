/* Data structure */

// Transition rule
typedef struct _rule {
  int id;
  char lhs[2];
  int rhs;
  struct _rule *next;
} rule_t; 

// State
typedef struct {
  int no;
  int accepted;
  rule_t *ruleset;
} state_t;

// Automaton
typedef struct _automaton {
  int start_nt_id;
  int initstate_id;
  int num_states;
  int capacity;
  state_t *states;
} automaton_t;

// Statemap
typedef struct _statemap {
  int state_no;
  int nt_no;
  int accepted;
  int initial;
  struct _statemap *next;
} statemap_t;


/* Functions */
void init();
void set_automaton(int start_nt_id, int initstate_id);
void add_state(int no, int accepted);
void add_rule(char st, char en, int to);

void reg_state(int state_no, int nt_no, int initial);
int search_nt(int state_no, int *nt_no, int *initial);

void print_automata();
void print_nt(int state_no);
char nt_prefix(int initial);

void clear_automata();
void free_statelist(state_t *stlist, int size);
void free_rule(rule_t *rule);
void free_stmap();
