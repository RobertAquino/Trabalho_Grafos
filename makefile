# Nome do executável
MAIN := main

# Objetos a serem gerados (cada .cpp vira um .o)
OBJECTS := main.o graph.o parser.o

# Flags de compilação para C++
# -Wall e -Wextra mostram avisos úteis do compilador
FLAGS := -Wall -Wextra -std=c++17 -g

# Definição do compilador: para C++, usamos g++
CC := g++

# Ajustando parâmetros ao sistema operacional
ifeq ($(OS), Windows_NT)
    OUTPUTMAIN := main.exe
    RM := del /q /f
else
    OUTPUTMAIN := main.out
    RM := rm -f
endif

# Ponto de compilação principal
all: $(OUTPUTMAIN)
	@echo Compilacao completa!

# Gerando o arquivo executável final ligando os objetos
$(OUTPUTMAIN): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(OUTPUTMAIN)

# Compilando o main.o (depende dos headers para saber as definicoes)
main.o: main.cpp graph.hpp parser.hpp
	$(CC) $(FLAGS) -c main.cpp

# Compilando o graph.o
graph.o: graph.cpp graph.hpp
	$(CC) $(FLAGS) -c graph.cpp

# Compilando o parser.o
parser.o: parser.cpp parser.hpp graph.hpp
	$(CC) $(FLAGS) -c parser.cpp

# Limpeza dos arquivos temporários
clean:
	$(RM) $(OBJECTS)
	$(RM) $(OUTPUTMAIN)
	@echo Limpeza completa!!!

# Comando para compilar e rodar direto
run: all
	./$(OUTPUTMAIN)