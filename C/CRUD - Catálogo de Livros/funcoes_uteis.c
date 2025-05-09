#include "estrutura_livros.h"

int menu() {
    int esc;

   
    printf("%s==============================================%s\n", CIANO, NORMAL);  
    printf("%s%s             BIBLIOTECA VIRTUAL             %s\n", NEGRITO, AZUL, NORMAL);
    printf("%s==============================================%s\n\n", CIANO, NORMAL);

    // Exibe as opções do menu, cada uma com uma cor diferente:
    printf("%s1.%s Listar Livros\n", VERDE, NORMAL);
    printf("%s2.%s Adicionar Livros\n", VERDE, NORMAL);
    printf("%s3.%s Atualizar um Livro\n", VERDE, NORMAL);
    printf("%s4.%s Apagar um Livro\n", VERDE, NORMAL);
    printf("%s0. Sair do programa%s\n\n", VERMELHO, NORMAL);

    // Solicita a escolha do usuário:
    printf("%sEscolha --> %s", AMARELO, NORMAL);
    scanf("%d", &esc);

    return esc;
}

void limpar_tela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int criar_id() {
    
    int id = 1;
    FILE *salvar_id = fopen("salvar_id.bin", "rb+");

    // Caso 1: Arquivo salvar_id existe e contém um ID válido
    if (salvar_id != NULL && fread(&id, sizeof(int), 1, salvar_id) == 1) {
        id++;
        rewind(salvar_id); // Prepara para sobrescrever o ID.
    }
    // Caso 2: Arquivo salvar_id não existe ou está vazio/corrompido:
    else {
        // Fecha salvar_id se estiver aberto (para evitar vazamento de recursos):
        if (salvar_id) {
            fclose(salvar_id);
            salvar_id = NULL;
        }
        // Busca o maior ID no banco_livros.bin
        FILE *banco = fopen("banco_livros.bin", "rb");
        if (banco) {
            Livros livro;
            while (fread(&livro, sizeof(Livros), 1, banco)) {
                if (livro.id >= id) {
                    id = livro.id + 1;
                }
            }
            fclose(banco);
        }
        // Reabre/cria salvar_id em modo escrita (sobrescreve)
        salvar_id = fopen("salvar_id.bin", "wb+");
        if (salvar_id == NULL) {
            printf("Erro crítico: não foi possível criar salvar_id.bin\n");
            return -1;
        }
    }

    // Salva o novo ID (sobrescrevendo o anterior)
    fwrite(&id, sizeof(int), 1, salvar_id);
    fclose(salvar_id);
    
    return id;
}

void limpar_buffer () {
    while (getchar() != '\n');
}

void pausar() {
    printf("%sPressione ENTER para voltar ao MENU%s\n", AMARELO, NORMAL);
    getchar(); // Espera o ENTER do usuário
}

int checar_inteiro(const char *texto) {
    int numero;
    char entrada[TAMANHO_ARRAY];

    while (1) {
        printf("%s%s%s%s", NEGRITO, AZUL, texto, NORMAL);
        fgets(entrada, sizeof(entrada), stdin);

        // Remove a quebra de linha do final (se existir)
        entrada[strcspn(entrada, "\n")] = '\0';

        // Verifica se a string está vazia (apenas ENTER)
        if (entrada[0] == '\0') {
            printf("%s%sDigite um número válido!%s\n", NEGRITO, VERMELHO, NORMAL);
            continue;
        }

        // Verifica se todos os caracteres são dígitos
        int inteiro = 1;
        for (int cont = 0; entrada[cont] != '\0'; cont++) {
            if (!isdigit(entrada[cont])) {
                inteiro = 0;
                break;
            }
        }

        if (inteiro == 1) {
            sscanf(entrada, "%d", &numero);
            break;
        } else {
            printf("%s%sDigite um número válido!%s\n", NEGRITO, VERMELHO, NORMAL);
        }
    }

    return numero;
}