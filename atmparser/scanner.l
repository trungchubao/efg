%{
#include <stdio.h>
#include <string.h>

#define MAXLENGTH 16

#include "y.tab.h"

%}

CONTROL      ([\000-\040\177])
Symbol       ([\041-\176])

%option yylineno

%%

x([0-9]|[1-9][0-9]*)    {
                        strcpy(yylval.ident, yytext+1);
                        return START;
                        }

initial                 return INIT;
state                   return STATE;
accept                  return ACC;
reject                  return RJT;
end                     return END;
"->"	                return RARROW;
":"                     return COLON;
"["		        return LBRACKET;
"]"		        return RBRACKET;
"-"                     return HYPHEN;

[0-9]|[1-9][0-9]*       {
                        strcpy(yylval.ident, yytext);
	                return NUMBER;
                        };

[ \t\n]                 ;
{CONTROL}               ;
.                       {
                        yylval.symb = yytext[0];
                        return SYMBOL;
                        }

%%

main(int argc, char *argv[]) {
    FILE *fp;

    if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "cannot open file: %s\n", argv[1]);
        exit(1);
    }

    yyin = fp;
    yyparse();
}
