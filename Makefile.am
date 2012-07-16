# Makefile for stegotorus
# Copyright 2011 Nick Mathewson, George Kadianakis, Zack Weinberg
# See LICENSE for other credits and copying information

ACLOCAL_AMFLAGS = -I m4 --install

AM_CXXFLAGS = -Werror -Wall -Wextra -Wformat=2 -std=c++11
AM_CPPFLAGS = -I. -I$(srcdir)/src -D_FORTIFY_SOURCE=2 $(lib_CPPFLAGS)

noinst_PROGRAMS     = scraper_tester

scraper_tester_SOURCES = \
	src/util_tmp.cc \
	src/crypt.cc \
	src/base64.cc \
	src/payloads.cc \
	src/payload_scraper.cc \
	src/main.cc

#unittests_SOURCES = \
	src/test/tinytest.cc \
	src/test/unittest.cc \
	$(UTGROUPS)

#nodist_unittests_SOURCES = unitgrplist.cc

#tltester_SOURCES = src/test/tltester.cc


