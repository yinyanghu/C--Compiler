CC = gcc
CFLAGS = -I. -Werror -g

FLEXLIB = -lfl
BISONLIB = -lfl -ly
LIB	= -lfl -ly

Object = LexChecker.o \
		 SyntaxChecker.o \
		 utility.o \
		 AST.o \
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
	rm lex.yy.c syntax.tab.c syntax.output
	rm *.o
	rm cmm
