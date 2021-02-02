bin_PROGRAMS = filter_mis_output.out

AM_CXXFLAGS = -I$(srcdir)/finter -I$(srcdir)/finter/include $(BOOST_CPPFLAGS) -ggdb -Wall -std=c++17

filter_mis_output_out_SOURCES = filter_mis_output/cargs.cc filter_mis_output/cargs.h filter_mis_output/main.cc filter_mis_output/utilities.cc filter_mis_output/utilities.h
filter_mis_output_out_LDADD = $(BOOST_LDFLAGS) -lfinter -lz -lbz2 -lboost_program_options
dist_doc_DATA = README
ACLOCAL_AMFLAGS = -I m4
