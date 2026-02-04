# --- SETTINGS ---
# Compilador e Flags
CC = gcc
# Flags: C11 Standard + Strict Warning levels para segurança de memória e boas práticas
CFLAGS = -W -Wall -Wshadow -pedantic -std=c11

# Binário final
EXE = sistema_carrinho.exe

# Objetos do projeto
OBJ = main.o clientes.o produtos.o utils.o

# --- TARGETS ---

# Default build
all: $(EXE)

# Linkagem final
$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE)

# --- COMPILAÇÃO E DEPENDÊNCIAS ---

main.o: main.c clientes.h produtos.h utils.h
	$(CC) $(CFLAGS) -c main.c -o main.o

# Clientes depende de produtos.h devido ao struct de Carrinho
clientes.o: clientes.c clientes.h produtos.h utils.h
	$(CC) $(CFLAGS) -c clientes.c -o clientes.o

produtos.o: produtos.c produtos.h utils.h
	$(CC) $(CFLAGS) -c produtos.c -o produtos.o

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c -o utils.o

# --- UTILS ---

# Limpeza de artefatos de build (Requer Git Bash ou compatível com POSIX no Windows)
clean:
	rm -f *.o $(EXE)

# Build & Run helper
run: all
	./$(EXE)