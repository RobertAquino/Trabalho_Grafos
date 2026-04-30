MAIN := main
# MANTENHA APENAS O main.o aqui
OBJECTS := main.o 
FLAGS := -Wall -Wextra -std=c++17 -g
CC := g++

ifeq ($(OS), Windows_NT)
    OUTPUTMAIN := main.exe
    RM := del /q /f
    EXEC := $(OUTPUTMAIN)
else
    OUTPUTMAIN := main.out
    RM := rm -f
    EXEC := ./$(OUTPUTMAIN)
endif

all: $(OUTPUTMAIN)
	@echo Compilacao completa!

$(OUTPUTMAIN): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(OUTPUTMAIN)

# A regra de compilação do main agora "carrega" o peso dos .cpp incluídos
main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp -o main.o

clean:
	@if exist main.o del /q /f main.o
	@if exist $(OUTPUTMAIN) del /q /f $(OUTPUTMAIN)

run: all
	$(EXEC)