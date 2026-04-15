# Nome do executável
MAIN := main

# Objetos a serem gerados. 
# Adicionei os nomes baseados nos seus arquivos atuais e na lógica de Setup que vamos implementar.
OBJECTS := main.o 

# Flags de compilação para C++
FLAGS := -Wall -Wextra -std=c++17 -g

# Definição do compilador
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

# Gerando o arquivo executável final
# Como você está usando apenas arquivos .hpp (templates/headers inline), 
# muitas vezes basta compilar o main.cpp que inclui todos eles.
$(OUTPUTMAIN): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(OUTPUTMAIN)

# Compilando o main.o
# Adicionei as dependências corretas de acordo com as suas pastas
main.o: main.cpp ./Bibliotecas/Estruturas.hpp ./Bibliotecas/Parser_JobInfo.hpp ./Bibliotecas/Parser_Operation.hpp ./Bibliotecas/Graph_Builder.hpp
	$(CC) $(FLAGS) -c main.cpp

# Limpeza dos arquivos temporários
clean:
	$(RM) $(OBJECTS)
	$(RM) $(OUTPUTMAIN)
	@echo Limpeza completa!!!

# Comando para compilar e rodar direto
run: all
	./$(OUTPUTMAIN)