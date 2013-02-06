#LFLAGS=--header-file=scanner.hh --outfile=scanner.cc
BUILT_SOURCES = parser.h
AM_LFLAGS= --header-file=scanner.h --outfile=scanner.cc
AM_YFLAGS= -d -p parser

LDADD=  ../lib/libstress.a ../lib/actions/libactions.a ../lib/state/libstate.a \
	-lc -lboost_system -lboost_iostreams -lcrypto -lpthread -lboost_system

#AM_CPPFLAGS=-I../lib -I../lib/state -I../lib/actions 
AM_CPPFLAGS=-I@top_srcdir@/lib -I@top_srcdir@/lib/state -I@top_srcdir@/lib/actions
AM_LDFLAGS=-L../lib -L../lib/state -L../lib/actions -lc -lboost_system \
				-lboost_iostreams -lcrypto -lpthread -lboost_system

scanner.hh scanner.cc: scanner.ll
	$(LEX) --header-file=scanner.h --outfile=scanner.cc scanner.ll
parser.cc parser.hh position.hh location.hh stack.hh: parser.yy
	$(YACC) -o  parser.cc parser.yy


bin_PROGRAMS = stress 

stress_SOURCES =	parser.yy scanner.ll main.cc \
						ABNFParser.cc \
                  AnomalyInjector.cc \
                  InjectorPerformer.cc \
						driver.cc \
						symbol.cc \
						log.cc  \
						dot-output.cc \
						xml-output.cc\
						output.cc  \
						txt-output.cc \
						abstract-injector-performer.h \
						abstract-injector-performer.cc \
						injector-performer-factory.h \
						injector-performer-factory.cc
#stress_HEADERS =  ABNFParser.h \
#						driver.h \
#                  AnomalyInjector.h \
#                  InjectorPerformer.h \
#						symbol.h \
#						log.h   \
#						output.h   \
#						main.cc

#ABNFParser_SOURCES = parser.yy scanner.ll ABNFParser.cc \
#							driver.cc \
#							symbol.cc #\
#							#scanner.cc \
#							#parser.cc
#ABNFParser_HEADERS = ABNFParser.h \
#							driver.h \
#							symbol.h #\
#							#scanner.hh \
#							#parser.hh

stressdir = $(sysconfdir)/stress
#ABNFParserdir = $(sysconfdir)/stress

