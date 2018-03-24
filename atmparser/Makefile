C   = gcc
LEX  = lex
YACC = yacc -d
RM   = rm -f

all: atmparser

lex.yy.c: scanner.l
	$(LEX) scanner.l

y.tab.c: parser.y
	$(YACC) parser.y

atmparser: scanner.c y.tab.c y.tab.h grammar.h grammar.c
	$(CC) y.tab.c scanner.c grammar.c -ll -o $@

clean:
	$(RM) *~ y.tab.* scanner.c
