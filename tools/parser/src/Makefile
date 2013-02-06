
SRC_HOME=../../../src
LIB_HOME=../../../lib
CFLAGS=-I$(SRC_HOME) -I../../../lib -I../../../lib/state -I../../../lib/actions -I. 
LFLAGS=-L../../../lib -L../../../lib/state -L../../../lib/actions -lstress \
			-lboost_system -lboost_iostreams -lcrypto -lpthread


all: parseabnf


parseabnf: Parser.o $(SRC_HOME)/ABNFParser.o  $(SRC_HOME)/driver.o $(SRC_HOME)/scanner.o $(SRC_HOME)/parser.o $(SRC_HOME)/symbol.o $(LIB_HOME)/*.o $(LIB_HOME)/actions/*.o $(LIB_HOME)/state/*.o
	g++ $(LFLAGS) -o "$@" $^ $(LFLAGS)
	rm *.o

Parser.o: Parser.cc
	g++ $(CFLAGS) $(LFLAGS) -c $<




clean:
	-rm parseabnf
	
	

build-clean: clean all



