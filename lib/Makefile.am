noinst_LIBRARIES = libstress.a

libstress_a_SOURCES = 	composite.cc \
								composite-factory.cc \
								or-composite.cc \
								repeat-composite.cc \
								leaf.cc \
								configurator.cc \
								socket-factory.cc \
								socket.cc \
								tcp-socket.cc \
								udp-socket.cc \
								raw-socket.cc \
								test-control.cc \
								test-case-manager.h \
								test-case-manager.cc \
								string-leaf.h \
								string-leaf.cc \
								bin-leaf.h \
								bin-leaf.cc \
								dec-leaf.h \
								dec-leaf.cc \
								hex-leaf.h \
								hex-leaf.cc \
								monitor.cc \
								monitor.h \
								abstract-monitor.h \
								abstract-monitor.cc \
								r-t-t-monitor.h \
								r-t-t-monitor.cc \
								sanity-check-monitor.h \
								sanity-check-monitor.cc \
								tcp-server-socket.h \
								tcp-server-socket.cc \
								ts-generator.h \
								ts-generator.cc

#libstress_a_HEADERS = 	composite-factory.h \
#								composite.h \
#								or-composite.h \
#								repeat-composite.h \
#								leaf.h \
#								configurator.h \
#								branch.h \
#								socket-factory.h \
#								socket.h \
#								tcp-socket.h \
#								udp-socket.h \
#								raw-socket.h \
#								test-control.h

SUBDIRS = actions state

LDADD = actions/libactions.a state/libstate.a

AM_CPPFLAGS=-Iactions -Istate

libstress_aLDFLAGS = -shared -lboost_system -lboost_iostreams -lcrypto -L./actions -L./state

libstress_adir = $(sysconfdir)/stress
