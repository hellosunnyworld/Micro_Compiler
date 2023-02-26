compiler: scanner.l parser.y
	@bison -d parser.y
	@flex scanner.l
	@g++ -g lex.yy.c parser.tab.c -lfl -o micro added_struct_function.cpp

clean:
	@rm lex.yy.c micro parser.tab.c parser.tab.h parser.tab.cc parser.tab.hh