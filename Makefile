# Executavel
BINFOLDER := bin/
# .hpp
INCFOLDER := inc/
# .cpp
SRCFOLDER := src/
# .o
OBJFOLDER := obj/

CC := g++

CFLAGS := -W -Wall -ansi -pedantic -std=c++14 -lwiringPi -lpthread

# Qualquer coisa de extensão .cpp
SRCFILES := $(wildcard src/*.cpp)

# SRCFILES transforma o .cpp em .o para a pasta obj
all: create_folders $(SRCFILES:src/%.cpp=obj/%.o)
	$(CC) $(CFLAGS) $(OBJFOLDER)*.o -o $(BINFOLDER)prog

# Compila o programa classe por classe.
$(OBJFOLDER)%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -I./$(INCFOLDER)

create_folders:
	@mkdir -p $(OBJFOLDER) $(BINFOLDER)
run: $(BINFOLDER)prog
	$(BINFOLDER)prog $(CROSS) $(PORT) $(CENTRALSERVERIP) $(CENTRALSERVERPORT)

.PHONY: clean
clean:
	rm -rf $(OBJFOLDER)*
	rm -rf $(BINFOLDER)*