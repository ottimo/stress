CFLAGS=	-g \
		 	-I.\
			-I/usr/include/wireshark \
			-I/usr/lib/glib-2.0/include \
			-I/usr/local/include/glib-2.0 \
			-I/usr/include/glib-2.0 \
			-I/usr/lib/x86_64-linux-gnu/glib-2.0/include


LIBS=  -lwsutil -lwiretap -lwireshark -lglib-2.0 
#LIBS=  -lwsutil -lwiretap -lwireshark -lglib-2.0 -ldissectors -lintl

LDFLAGS=-L/usr/lib/wireshark

FLAGS= -g $(CFLAGS) $(LDFLAGS) $(LIBS)

all: dissect

dissect: main.o util.o  model.o dissector.o fileh.o
	g++ $(FLAGS) -o "$@" $^
	rm *.o

fileh.o: fileh.cpp fileh.h 
	g++ $(FLAGS)  -c $<

util.o: util.cpp util.h 
	g++ $(FLAGS)  -c $<

model.o: model.cpp model.h fileh.h util.h
	g++ $(FLAGS)  -c $<

dissector.o: dissector.cpp dissector.h model.h fileh.h util.h
	g++ $(FLAGS)  -c $<

main.o: main.cpp dissector.h fileh.h 
	g++ $(FLAGS) -c $<


clean:
	-rm dissect
	
	 
build-clean: clean all
