%{

#define MAXLENGTH 16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grammar.h"

extern int yylineno;
extern char *yytext;

extern int num_automata;

%}

%union {
    int num;
    char ident[MAXLENGTH+1];
    char symbs[2];
    char symb;
}

%token<ident> START
%token END
%token INIT
%token STATE
%token ACC RJT
%token RARROW HYPHEN
%token LBRACKET RBRACKET
%token COLON

%token <ident> NUMBER
%token <symb> SYMBOL

%type <num> cond
%type <num> rhs
%type <symbs> lhs
%type <symb> symbol
%type <num> initial_state
%%

automata
        : {init();} automatonlist 

automatonlist
        : automaton { print_automata(); clear_automata(); }
          automatonlist
        | /* empty */

automaton
        : START initial_state { set_automaton(atoi($1),$2); } 
          state_rules_list END
	;

initial_state
        : INIT STATE COLON NUMBER { $$ = atoi($4); }
	;

state_rules_list
	: state_rules state_rules_list 
	| /* empty */
	;

state_rules
        : STATE NUMBER LBRACKET cond RBRACKET COLON 
          { add_state(atoi($2),$4); } 
          rule_list
	;

cond
	: ACC	{ $$ = 1; }
	| RJT	{ $$ = 0; }
	;

rule_list
	: rule rule_list
        | /* empty */

rule	: lhs RARROW rhs { add_rule($1[0], $1[1], $3); }
	;

lhs	: symbol                { $$[0]=$$[1]=$1; }
        | symbol HYPHEN symbol  { $$[0]=$1; $$[1]=$3; }
	;

symbol  : NUMBER   { $$ = $1[0]; }
        | SYMBOL   { $$ = $1;  }
        | COLON    { $$ = ':'; }
        | LBRACKET { $$ = '['; }
        | RBRACKET { $$ = ']'; }
        | HYPHEN   { $$ = '-'; }
        ;

rhs	: NUMBER { $$ = atoi($1); }
	;

%% 
yyerror(char *s)
{
  fprintf(stderr, "Error: %s (line %d, token \"%s\")\n", s, yylineno, yytext);
}

