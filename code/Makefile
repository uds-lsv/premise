OBJECTS=embeddings.o densebitvec.o sparsebitvec.o main.o bitcollection.o database.o bitfactory.o mdlalgo.o main_routines.o graph.o rule.o mdlmath.o ruletable.o premiseparameters.o
PROGNAME=Premise
CXX=g++

CXXFLAGS= -std=c++14 -ggdb -Wshadow -Wall -pedantic -pipe -O3 -fopenmp -march=native

.PHONY: all clean profile

all: ${PROGNAME}

profile: ${PROGNAME}Profile

%.o: %.cpp %.h
	${CXX} ${CXXFLAGS} -c $<


%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $<


${PROGNAME}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o $@ -lstdc++fs


${PROGNAME}Profile: ${OBJECTS}
	${CXX} ${CXXFLAGS} -pg -rdynamic ${OBJECTS} -o $@ -lstdc++fs


clean:
	rm -f ${OBJECTS} ${PROGNAME}


