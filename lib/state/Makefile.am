noinst_LIBRARIES = libstate.a

libstate_a_SOURCES = 	ok-state.cc \
								state.cc \
								fail-state.h \
								fail-state.cc \
								value-state.h \
								value-state.cc \
								value-hex-state.h \
								value-hex-state.cc \
								value-string-state.h \
								value-string-state.cc \
								value-int-state.h \
								value-int-state.cc \
								value-bin-state.h \
								value-bin-state.cc \
								read-packet-state.h \
								read-packet-state.cc \
								send-packet-state.h \
								send-packet-state.cc \
								monitor-report.h \
								monitor-report.cc
#libstate_a_HEADERS = 	ok-state.h \
#								state.h 


AM_CPPFLAGS = -I@top_srcdir@/lib

libstate_aLDFLAGS = -shared -lboost_system -lboost_iostreams -lcrypto

libstate_adir = $(sysconfdir)/stress
