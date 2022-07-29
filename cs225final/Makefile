EXENAME = airports
OBJS = main.o airports.o airports_graph.o
TEST_OBJS = test.o airports.o airports_graph.o

CXX = clang++
CXXFLAGS = $(CS225) -std=c++14 -stdlib=libc++ -c -g -O3 -Wall -Wextra -pedantic
LD = clang++
LDFLAGS = -std=c++14 -stdlib=libc++ -lc++abi -lm

# Custom Clang version enforcement Makefile rule:
ccred=$(shell echo -e "\033[0;31m")
ccyellow=$(shell echo -e "\033[0;33m")
ccend=$(shell echo -e "\033[0m")

IS_EWS=$(shell hostname | grep "ews.illinois.edu")
IS_CORRECT_CLANG=$(shell clang -v 2>&1 | grep "version 6")
ifneq ($(strip $(IS_EWS)),)
ifeq ($(strip $(IS_CORRECT_CLANG)),)
CLANG_VERSION_MSG = $(error $(ccred) On EWS, please run 'module load llvm/6.0.1' first when running CS225 assignments. $(ccend))
endif
else
CLANG_VERSION_MSG = $(warning $(ccyellow) Looks like you are not on EWS. Be sure to test on EWS before the deadline. $(ccend))
endif

.PHONY: all test clean output_msg

all : $(EXENAME)

output_msg: ; $(CLANG_VERSION_MSG)

$(EXENAME): output_msg $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o: main.cpp airports.h airports_graph.h
	$(CXX) $(CXXFLAGS) main.cpp

airports.o: airports.cpp airports.h
	$(CXX) $(CXXFLAGS) airports.cpp

airports_graph.o: airports_graph.cpp airports_graph.h airports.h
	$(CXX) $(CXXFLAGS) airports_graph.cpp

test: output_msg catchmain.o $(TEST_OBJS)
	$(LD) $(TEST_OBJS) catchmain.o $(LDFLAGS) -o test

catchmain.o : cs225/catch/catchmain.cpp cs225/catch/catch.hpp
	$(CXX) $(CXXFLAGS) cs225/catch/catchmain.cpp

test.o: tests/test.cpp cs225/catch/catch.hpp airports.h airports_graph.h
	$(CXX) $(CXXFLAGS) tests/test.cpp

clean:
	-rm -f *.o $(EXENAME) test