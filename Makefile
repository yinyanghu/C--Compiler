CC = gcc
CFLAGS = -I. -Werror

LEXLIB = -lfl

Object = LexChecker.o \
		 utility.o

LexicalAnalyser: lex.yy.c $(Object)
	$(CC) $(CFLAGS) $(Object) lex.yy.c $(LEXLIB) -o C

lex.yy.c: lexical.l
	flex lexical.l

clean:
	rm lex.yy.c
	rm *.o
	rm C
