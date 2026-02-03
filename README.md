# Trabalho-Pratico (CRUD em C)

Projeto desenvolvido para a disciplina de Estrutura de Dados da Universidade de Brasília (UnB). O sistema implementa o gerenciamento de uma loja utilizando **Listas Simplesmente Encadeadas** e persistência de dados em arquivos de texto.

## 📋 Funcionalidades

### 1. Gestão de Clientes
- Cadastro com validação de CPF (Algoritmo oficial).
- Listagem, Busca, Edição e Remoção.
- Estrutura de dados: Lista Encadeada Dinâmica.

### 2. Gestão de Produtos
- Cadastro de itens com controle de estoque.
- Listagem e Busca por código.
- Ordenação automática por código (Bubble Sort trocando conteúdo).

### 3. Modo Compra (Carrinho)
- Sistema relacional: Clientes possuem uma sub-lista de compras.
- Adição e remoção de itens no carrinho.
- **Finalizar Compra**: Baixa automática no estoque e cálculo do valor total.
- Persistência relacional: Salva vínculos CPF <-> Produto em `carrinhos.txt`.

### 4. Diferenciais Técnicos
- **Persistência de Dados**: Carregamento e salvamento automático (`.txt` e `.csv`).
- **Memory Safety**: Wrapper `alloc_check` para garantir alocação de memória.
- **Garbage Collection**: Limpeza manual de memória Heap ao encerrar o programa.
- **Interface**: Menus interativos via terminal.

---

## 🚀 Como Compilar e Rodar

Certifique-se de ter o compilador **GCC** instalado.

### Via Makefile (Recomendado)
O projeto conta com um script de automação para compilação e limpeza.
```bash
make       # Apenas compila o projeto
make run   # Compila e executa automaticamente
make clean # Remove arquivos temporários (.o) e o executável
```

### Compilação Manual (Caso não tenha Make)
Caso prefira compilar manualmente via GCC:

**No Windows (MinGW/Git Bash)**
```bash
gcc main.c clientes.c produtos.c utils.c -o sistema_carrinho.exe
./sistema_carrinho.exe
```

**No Linux / MacOS**
```bash
gcc main.c clientes.c produtos.c utils.c -o sistema_carrinho
./sistema_carrinho
```

---

## 📂 Estrutura do Projeto

- **`main.c`**: Ponto de entrada, menus e loop principal.
- **`clientes.c/h`**: Lógica de clientes e carrinho de compras.
- **`produtos.c/h`**: Lógica de estoque e produtos.
- **`utils.c/h`**: Ferramentas auxiliares (Validação CPF, UI, Memória).
- **`Makefile`**: Script de automação de build e gerenciamento de dependências.
- **`.txt`**: Arquivos gerados automaticamente para banco de dados.

---

## 👥 Autores

- **Paulo Vinicius**
- **Arthur Luiz**
- **Ryan Lira**