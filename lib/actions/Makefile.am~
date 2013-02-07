noinst_LIBRARIES = libactions.a

libactions_a_SOURCES = 	action.cc \
								or-action.cc \
								repeat-action.cc \
								action-factory.cc \
								return-action.cc \
								send-action.cc \
								udp-send-action.cc \
								tcp-send-action.cc \
								raw-send-action.cc \
								raw-read-action.cc \
								read-action.h \
								read-action.cc \
								udp-read-action.h \
								udp-read-action.cc \
								tcp-read-action.h \
								tcp-read-action.cc \
								inject-action.h \
								inject-action.cc \
								number-value.h \
								number-value.cc \
								command-composite.h \
								command-composite.cc \
								command.h \
								command.cc \
								length-command.h \
								length-command.cc \
								string-length-command.h \
								string-length-command.cc \
								memorize-command.h \
								memorize-command.cc \
								skel-command.h \
								skel-command.cc \
								u-pause-command.h \
								u-pause-command.cc \
								include-actions.h \
								variable-command.cc \
								variable-command.h \
								addition-command.cc \
								addition-command.h \
								base64-decode-command.cc \
								base64-decode-command.h \
								base64-encode-command.cc \
								base64-encode-command.h \
								md5-command.cc \
								md5-command.h \
								maskcheck-command.cc \
								maskcheck-command.h \
								valuecheck-command.cc \
								valuecheck-command.h \
								ceildivide-command.cc \
								ceildivide-command.h \
								repeat-command.cc \
								repeat-command.h
#libactions_a_HEADERS = 	action.h \
#								or-action.h \
#								repeat-action.h \
#								action-factory.h \
#								return-action.h \
#								send-action.h \
#								udp-send-action.h \
#								tcp-send-action.h \
#								raw-send-action.h 

AM_CPPFLAGS = -I@top_srcdir@/lib -I@top_srcdir@/lib/state

LDADD = @top_srcdir@/lib/libstate.a

libactions_aLDFLAGS = -shared -lboost_system -lboost_iostreams -lcrypto -L@top_srcdir@/lib/state

libactions_adir = $(sysconfdir)/stress
