CC=g++ -Wall -Wextra -O3 -g
CFLAGS= -Wl,--no-as-needed,-lpthread
LDFLAGS=`root-config --glibs --cflags` -lTMVA `correction config --cflags --ldflags --rpath` -D_GLIBCXX_USE_CXX11_ABI=0

SOURCES= correctionTester.cc src/JetEnergyCorrections.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=testJEC

all: 
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)
	
clean:
	rm -rf *o $(EXECUTABLE)
