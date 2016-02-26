.SUFFIXES:
.SUFFIXES: .o .c
.SUFFIXES: .o .cpp


.cpp.o :
	g++ -c -g -o $@ $<

.c.o :
	gcc -c -g -o $@ $<

SOURCES= \
complex.o \
dcolors.o \
dfcalc.o \
dfnoise.o \
dfourier.o \
dfroutin.o \
dtimepnt.o \
dtimestr.o \
dtnames.o \
dperipnt.o \
str.o \
\
main.o


file: $(SOURCES)
	c++ -o $@ $(SOURCES) -lm


clean:
	rm -f *.o file


