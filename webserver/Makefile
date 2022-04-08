
prog = test
FLAGS = -std=c++98 -g 
GCC =  g++

SOURCES =  ConfiGrammar.cpp tokenizer.cpp tokens.cpp Configuration.cpp main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

%.o: %.cpp
	$(GCC) $(FLAGS) -o $@ -c $< 

$(prog) : $(SOURCES:.cpp=.o)
	$(GCC) $(FLAGS) $(SOURCES:.cpp=.o) -o $(prog)

all: $(prog)

clean:
	rm *.o