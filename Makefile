CC = gcc
CFLAGS = -I. -g -Werror

FLEXLIB = -lfl
BISONLIB = -lfl -ly
LIB	= -lfl -ly

Object = LexChecker.o \
		 SyntaxChecker.o \
		 utility.o \
		 SemanticChecker.o \
		 MC.o \
		 Optimize.o \
		 IR.o \
		 AST.o \
		 SymbolsTable.o \
		 syntax.tab.o \
		 main.o

cmm: $(Object) Makefile
	$(CC) $(CFLAGS) $(Object) $(LIB) -o cmm

syntax.tab.o: syntax.tab.c
	$(CC) $(CFLAGS) syntax.tab.c $(BISONLIB) -c

syntax.tab.c: syntax.y lex.yy.c
	bison -t -d -v syntax.y

#LexicalAnalyser: lex.yy.c $(Object)
#	$(CC) $(CFLAGS) $(Object) lex.yy.c $(FLEXLIB) -o cmm_lex

lex.yy.c: lexical.l
	flex lexical.l

clean:
	rm -f lex.yy.c syntax.tab.c syntax.output syntax.tab.h
	rm -f *.o
	rm -f cmm
