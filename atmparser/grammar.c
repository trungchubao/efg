#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "grammar.h"

int global_nt_id;        // For assignment of a unique id to a generated non-terminal.

automaton_t automaton;   // The information of the current automaton is stored.
statemap_t  *stmap;      // Maintain the correspondance between states and non-terminal.

#define DEFAULTSIZE 10   // default capacity size of lists of states.


/* --------------------------------------------------------------------*/
void init()
{
  global_nt_id = 0;
  stmap = NULL;
}

/* --------------------------------------------------------------------*/
void set_automaton(int start_nt_id, int initstate_id)
{
  automaton.start_nt_id = start_nt_id;
  automaton.initstate_id = initstate_id;
  automaton.num_states = 0;
  automaton.capacity = DEFAULTSIZE;
  automaton.states = (state_t*) malloc(DEFAULTSIZE*sizeof(state_t));
}

/* --------------------------------------------------------------------*/
void add_state(int no, int accepted)
{
  automaton_t *A = &automaton;

  if(A->capacity <= A->num_states + 1){
    state_t* tmp;
    if((tmp = (state_t*)realloc(A->states, 2*(A->capacity)*sizeof(state_t)))==NULL){
      printf("realloc error\n");
      exit(EXIT_FAILURE);
    } else {
      A->states = tmp;
      A->capacity *= 2;
    }
  }

  A->states[A->num_states].no = no;
  A->states[A->num_states].accepted = accepted;
  A->states[A->num_states].ruleset = NULL;

  int id, initial;
  if(A->initstate_id == no){
    id = A->start_nt_id;
    initial = 1;
  }
  else{
    id = global_nt_id++;
    initial = 0;
  }
  A->num_states++;

  reg_state(no, id, initial);
}

/* --------------------------------------------------------------------*/
void add_rule(char st, char en, int to)
{
  automaton_t *A = &automaton;
  rule_t *tmp = (rule_t*)malloc(sizeof(rule_t));

  tmp->next = A->states[A->num_states-1].ruleset;

  tmp->id = global_nt_id++;
  tmp->lhs[0] = st;
  tmp->lhs[1] = en;
  tmp->rhs = to;

  A->states[A->num_states-1].ruleset = tmp;
}

/* --------------------------------------------------------------------*/
void reg_state(int state_no, int nt_no, int initial)
{
  statemap_t *tmp = (statemap_t*)malloc(sizeof(statemap_t));

  tmp->state_no = state_no;
  tmp->nt_no = nt_no;
  tmp->initial = initial;
  tmp->next = stmap;
  stmap = tmp;
}

/* --------------------------------------------------------------------*/
int search_nt(int state_no, int *nt_no, int *initial)
{
  statemap_t *ptr = stmap;
  while(ptr != NULL){
    if(ptr->state_no == state_no){
      *nt_no = ptr->nt_no;
      *initial = ptr->initial;
      return 1;
    }
    ptr = ptr->next;
  }
  return 0;
}

/* --------------------------------------------------------------------*/
void print_automata()
{
  int j;
  char c;
  automaton_t A = automaton;

  for(j=0; j<A.num_states; j++){
    if(A.states[j].ruleset== NULL && A.states[j].accepted == 0) continue;

    print_nt(A.states[j].no);
    printf(" ->");

    if(A.states[j].accepted == 1) {
      printf(" <>");
      if(A.states[j].ruleset != NULL)
        printf(" |");
      else
        printf("\n");
    }

    rule_t *ptr = A.states[j].ruleset;
    int tmp_nt_no, tmp_accepted, tmp_initial;
    if(ptr != NULL){

      printf(" y%d", ptr->id);
      print_nt(ptr->rhs);

      ptr = ptr->next;
      while(ptr != NULL){
        printf(" | y%d", ptr->id);
        print_nt(ptr->rhs);

        ptr = ptr->next;
      }
      printf("\n");

      ptr = A.states[j].ruleset;
      while(ptr != NULL){
        printf("y%d ->", ptr->id);
        printf(" %c", ptr->lhs[0]);
        for(c=ptr->lhs[0]+1; c<=ptr->lhs[1]; c++){
          printf(" | %c", c);
        }
        printf("\n");
        ptr = ptr->next;
      }
    }
  }
}

/* --------------------------------------------------------------------*/
void print_nt(int state_no)
{
  int tmp_nt_no, tmp_initial;

  if(!search_nt(state_no, &tmp_nt_no, &tmp_initial)){
    printf("State %d : not found\n", state_no);
    exit(1);
  }

  printf("%c%d", nt_prefix(tmp_initial), tmp_nt_no);
}

/* --------------------------------------------------------------------*/
char nt_prefix(int initial)
{
  if(initial) return 'x';
  else return 'y';
}


/* --------------------------------------------------------------------*/
void clear_automata()
{
  free_statelist(automaton.states, automaton.num_states);
}

/* --------------------------------------------------------------------*/
void free_statelist(state_t *stlist, int size)
{
  int i;

  for(i=0; i<size; i++){
    free_rule(stlist[i].ruleset);
  }
  
  free(stlist);
}

/* --------------------------------------------------------------------*/
void free_rule(rule_t *rule)
{
  rule_t *ptr;

  if(rule == NULL) return;

  ptr = rule->next;
  free(rule);
  free_rule(ptr);
}

/* --------------------------------------------------------------------*/
void free_stmap(statemap_t *p)
{
  if(p == NULL) return;

  free_stmap(p->next);
  free(p);
}