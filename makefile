# Nome do executável final
MAIN := main

# Lista de objetos (arquivos .o que serão gerados a partir dos .cpp)
# Note que não incluímos os .hpp aqui, apenas os arquivos que possuem código fonte executável
OBJECTS := main.o src/Avaliador.o src/Estruturas.o

# Compilador e Flags
CC := g++
# -Wall -Wextra: Ativa avisos de possíveis erros no código
# -std=c++17: Necessário se for usar recursos modernos
# -g: Permite depuração (útil para encontrar erros de memória)
FLAGS := -Wall -Wextra -std=c++17 -g

# Identificação do Sistema Operacional para comandos de terminal
ifeq ($(OS), Windows_NT)
    OUTPUTMAIN := $(MAIN).exe
    RM := del /q /f
    # Comando específico para limpar arquivos dentro de subpastas no Windows
    RM_SRC := del /q /f src\*.o
    EXEC := $(OUTPUTMAIN)
else
    OUTPUTMAIN := $(MAIN).out
    RM := rm -f
    RM_SRC := rm -f src/*.o
    EXEC := ./$(OUTPUTMAIN)
endif

# Alvo principal: Compila tudo
all: $(OUTPUTMAIN)
	@echo Compilacao completa com sucesso!

# Linkagem: Une os objetos para criar o executável
$(OUTPUTMAIN): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(OUTPUTMAIN)

# Regra para compilar o main.o
# Ele depende do main.cpp e de todos os headers incluídos
main.o: main.cpp Bibliotecas/Estruturas.hpp Bibliotecas/Parser_JobInfo.hpp \
         Bibliotecas/Parser_Operation.hpp Bibliotecas/Parser_Setup.hpp \
         Bibliotecas/Avaliador.hpp
	$(CC) $(FLAGS) -c main.cpp -o main.o

# Regra para compilar o Avaliador.o
src/Avaliador.o: src/Avaliador.cpp Bibliotecas/Avaliador.hpp Bibliotecas/Estruturas.hpp
	$(CC) $(FLAGS) -c src/Avaliador.cpp -o src/Avaliador.o

# Regra para compilar o Instancias.o
src/Estruturas.o: Bibliotecas/Estruturas.hpp
	$(CC) $(FLAGS) -c src/Estruturas.cpp -o src/Estruturas.o

# Limpeza de arquivos temporários (Adaptada para funcionar em Windows e Linux)
clean:
	-$(RM) main.o
	-$(RM_SRC)
	-$(RM) $(OUTPUTMAIN)
	@echo Limpeza de arquivos .o e executaveis completa!

# Atalho para compilar e executar
run: all
	$(EXEC)