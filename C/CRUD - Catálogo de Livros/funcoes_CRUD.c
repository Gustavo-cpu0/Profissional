#include "estrutura_livros.h"

/* 
Funções de CRUD: 
C - Create (Criar)
R - Read (Ler)
U - Update (Atualizar)
D - Delete (Deletar)
*/

void adicionar_livro() { 

  /*
  A função é separada em:
  Tipo de dado que ela retorna: Void (Neste caso uso o void pois ela não retorna nada);
  Nome da função;
  Parametros da função que ficam dentro dos parenteses (Neste caso minha função de inserção não necessita de Parametros);
  */
  
  /*
  Nestá função eu uso o fopen para criar os arquivo.bin e adicionar para realizar a persistencia, não uso nenhum parametro,
  nem preciso de um return, já que minha função é apenas a execução do bloco de codigo de coleta de dados e guardar no determinado arquivo
  */
    
  // Por algum motivo o fgets não funciona direito se eu não limpar o buffer antes:
  limpar_buffer();
    
  // Aqui eu crio o struct e chamo de livro:
  Livros livro;

  printf("%s%s==============================================%s\n", NEGRITO, CIANO, NORMAL);
  printf("%s%s               ADICIONAR LIVROS           %s\n", NEGRITO, AZUL, NORMAL);
  printf("%s%s==============================================%s\n\n", NEGRITO, CIANO, NORMAL);
  
  // Aqui inicio a coleta de dados para guardar no .bin:
  printf("%s%sDigite o nome do livro:%s ", NEGRITO, AZUL, NORMAL);
  fgets(livro.nome, sizeof(livro.nome), stdin);

  printf("%s%sDigite o nome do autor:%s ", NEGRITO, AZUL, NORMAL);
  fgets(livro.autor, sizeof(livro.autor), stdin);

  printf("%s%sDigite o nome da editora:%s ", NEGRITO, AZUL, NORMAL);
  fgets(livro.editora, sizeof(livro.editora), stdin);
  // Aqui chamo a função checar_inteiro que serve para checar se o numero digitado é inteiro e retorna o numero para guardar na    variavel, fiz uma função parecido com o input do python, mas com uma validação de dados:
  livro.paginas = checar_inteiro("Digite o número de páginas: ");
  livro.ano = checar_inteiro("Digite o ano de publicação: ");
  
  // Aqui chamo a função gerar_id que gera um id unico e crescente para cada livro:
  livro.id = criar_id();
  // Tratamento de erro para caso o id seja = -1:
  if (livro.id == -1) {
      printf("\nErro ao gerar ID!\n");
      return;
  }

  // Aqui uso o fopen que serve para abrir ou criar um arquivo.bin:
  FILE *banco_dados = fopen("banco_livros.bin", "ab"); // Aqui uso o ab = append (no caso sempre adicionar livros e não sobrescrever) e o b = binario (ele vai salvar os dados em binario).
    
  // Caso ele não cosniga abrir trata o erro com a mensagem:
  if (banco_dados == NULL) {
    printf("%sErro ao abrir o arquivo.%s\n", VERMELHO, NORMAL);
    return;
  }
    
  // Aqui eu escrevo os dados digitados no .bin: 
  fwrite(&livro, sizeof(Livros), 1, banco_dados);
    
  // Fecho o arquivo:
  fclose(banco_dados);
    
  // Aqui é só uma mensagem de conrfimação para o usuario ver que está tudo inserido corretamente:
  printf("%s%s ✅  Livro inserido com sucesso!%s\n", NEGRITO, VERDE, NORMAL);
  pausar();
}

void listar_livros() {
    
    /*
    Nesta função eu apenas leio o arquivo e exibo os dados na tela, não preciso 
    de parametros, nem preciso de um return, já que minha função é apenas a 
    execução do bloco de codigo de leitura e exibição dos dados.
    */

    // Aqui eu uso o fopen para abrir o arquivo.bin e ler os dados:
    FILE *banco_dados = fopen("banco_livros.bin", "rb");

    // Caso ele não consiga abrir trata o erro com a mensagem:
    if (banco_dados == NULL) {
        printf("%s%sNenhum livro cadastrado.%s\n", NEGRITO, VERMELHO, NORMAL);
        return;
    }

    // Faço tratamento caso queria exibir o banco_livros vazio:
    fseek(banco_dados, 0, SEEK_END);
    if (ftell(banco_dados) == 0) {
        printf("%s%sArquivo vazio!%s\n", NEGRITO, VERMELHO, NORMAL);
        fclose(banco_dados);
        return;
    }

    // Volto o ponteiro para o inicio do arquivo por precaução:
    rewind(banco_dados);

    Livros livro;
    printf("%s%s==============================================%s\n", NEGRITO, CIANO, NORMAL);
    printf("%s%s              LIVROS CADASTRADOS           %s\n", NEGRITO, AZUL, NORMAL);
    printf("%s%s==============================================%s\n\n", NEGRITO, CIANO, NORMAL);

    // Aqui eu uso o fread para ler os dados do .bin e exibir na tela:
    while (fread(&livro, sizeof(Livros), 1, banco_dados) == 1) {
        
        // Aqui faço o tratamento de de \n se tiverem na inserção das strings:
        livro.nome[strcspn(livro.nome, "\n")] = '\0';
        livro.autor[strcspn(livro.autor, "\n")] = '\0';
        livro.editora[strcspn(livro.editora, "\n")] = '\0';

        printf("%s==============================================%s\n", CIANO, NORMAL);
        printf("%s%s📘 Nome:%s %s\n", NEGRITO, AZUL, NORMAL, livro.nome);
        printf("%s%s🆔 ID:%s %d\n", NEGRITO, AZUL, NORMAL, livro.id);
        printf("%s%s📝 Autor:%s %s\n", NEGRITO, VERDE, NORMAL, livro.autor);
        printf("%s%s🏢 Editora:%s %s\n", NEGRITO, AZUL, NORMAL, livro.editora);
        printf("%s%s📏 Páginas:%s %d\n", NEGRITO, AMARELO, NORMAL, livro.paginas);
        printf("%s%s📅 Ano:%s %d\n", NEGRITO, ROXO, NORMAL, livro.ano);
        printf("%s==============================================%s\n\n", CIANO, NORMAL);
    }

    // Pauso, limpo um possivel /n do buffer e limpo a tela:
    pausar();
    getchar();
    limpar_tela();
    // Aqui eu fecho o arquivo:
    fclose(banco_dados);

}

void apagar_livro() {
    /*
     Para Deletar os arquivos em C, como não existe uma função direta que apague eu preciso fazer uma gambiarra:
    Nesta "gambiarra" eu identifico qual livro eu quero deletar, e faço um arquivo.bin copia, onde eu jogo
    todos os dados do original na copia, menos o que desejo apagar, após isso eu apago o original e renomeio
    o arquivo temporario com o mesmo nome do original assim "deletando" o livro escolhido.
    */

    int continuar = 1;

    while (continuar == 1) {
        // Por algum motivo o fgets não funciona direito se eu não limpar o buffer antes:
        limpar_buffer();

        int id_delete;
        int encontrado = 0;

        // Aqui novamente eu uso o fopen para abrir o arquivo.bin e ler os dados:
        FILE *banco_dados = fopen("banco_livros.bin", "rb");
        // Tratamento caso não tenha nenhum livro cadastrado:
        if (banco_dados == NULL) {
            printf("%s%sErro: Nenhum livro cadastrado ou arquivo não encontrado.%s\n", NEGRITO, VERMELHO, NORMAL);
            pausar();
            return;
        }

        printf("%s%s==============================================%s\n", NEGRITO, CIANO, NORMAL);
        printf("%s%s          APAGAR LIVRO POR ID           %s\n", NEGRITO, AZUL, NORMAL);
        printf("%s%s==============================================%s\n\n", NEGRITO, CIANO, NORMAL);

        // Criando o livro para percorrer o .bin e exibir os nomes e id:
        Livros livro;

        while (fread(&livro, sizeof(Livros), 1, banco_dados)) {
            // Tratamento novamente:
            livro.nome[strcspn(livro.nome, "\n")] = '\0';
            // O -5 e o -30 é apenas uma configuração de espaço de exibição dessa forma meu print sai alinhado:
            printf("%s%s🆔 ID: %-5d%s | %s📘 %-30s%s\n", NEGRITO, AZUL, livro.id, NORMAL, AMARELO, livro.nome, NORMAL);
        }
        // Aqui eu volto o ponteiro para o inicio do banco_dados, ja que percorri o .bin inteiro para exibir os nomes e id:
        rewind(banco_dados);

        // Crio o arquivo temporario com tratamento de erro:
        FILE *temporario = fopen("temp.bin", "wb");
        if (temporario == NULL) {
            printf("%s%sErro ao criar arquivo temporário.%s\n", NEGRITO, VERMELHO, NORMAL);
            fclose(banco_dados);
            pausar();
            return;
        }

        // Uso a mesma função para checar se o numero digitado é inteiro:
        id_delete = checar_inteiro("Digite o ID do livro que deseja apagar: ");

        // Percorre o .bin, e escreve no arquivo temporario apenas o que não é para apagar:
        while (fread(&livro, sizeof(Livros), 1, banco_dados)) {
            if (livro.id != id_delete) {
                fwrite(&livro, sizeof(Livros), 1, temporario);
            }
            else {
                encontrado = 1;
                // Aqui eu mostro o livro que será apagado:
                printf("\n%s%s⚠️ Livro selecionado para exclusão:%s\n", NEGRITO, VERMELHO, NORMAL);
                printf("%s%s📘 Título:%s %s\n", NEGRITO, AZUL, NORMAL, livro.nome);
                printf("%sPressione ENTER para CONTINUAR%s\n", AMARELO, NORMAL);
                getchar(); // Espera o ENTER do usuário
            }
        }

        // Fecho os arquivos:
        fclose(banco_dados);
        fclose(temporario);

        // Removo o original e renomeia o temporário:
        remove("banco_livros.bin");
        rename("temp.bin", "banco_livros.bin");


        if (encontrado) {
            limpar_tela();
            printf("%s%s==============================================%s\n", NEGRITO, VERDE, NORMAL);
            printf("%s%s✅ Livro deletado com sucesso!%s\n", NEGRITO, VERDE, NORMAL);
            printf("%s%s==============================================%s\n", NEGRITO, VERDE, NORMAL);
        }
        else {
            printf("%s%s==============================================%s\n", NEGRITO, VERMELHO, NORMAL);
            printf("%s%s Livro não encontrado ! %s\n", NEGRITO, VERMELHO, NORMAL);
            printf("%s%s==============================================%s\n\n", NEGRITO, VERMELHO, NORMAL);
        }

        // Pergunta se quer continuar apagando:
        char opcao;
        limpar_buffer();
        printf("%sDeseja apagar outro livro? (s/n): %s", AMARELO, NORMAL);
        scanf(" %c", &opcao);
        limpar_tela();
        continuar = (opcao == 's' || opcao == 'S');
    }
}

void atualizar_livro() {
    /*
    Aqui nesta função faço parecido com a função de deletar, usando a mesma "gambiarra" de criar um arquivo temporario,
    para escrever nele os dados do original, mas em vez de não escrever o livro que desejo apagar, eu escrevo ele com
    as atualizações feitas pelo usuario.
    */

    int continuar = 1;

    while (continuar == 1) {
        // Por algum motivo o fgets não funciona direito se eu não limpar o buffer antes:
        limpar_buffer();
        limpar_tela();

        int id_atualizar;
        int encontrado = 0;
        int opcao;

        // Novamente uso o fopen para abrir o arquivo.bin e ler os dados:
        FILE *banco_livros = fopen("banco_livros.bin", "rb");
        if (banco_livros == NULL) {
            printf("%s%sErro: Nenhum livro cadastrado.%s\n", NEGRITO, VERMELHO, NORMAL);
            pausar();
            return;
        }

        printf("%s%s==============================================%s\n", NEGRITO, CIANO, NORMAL);
        printf("%s%s          ATUALIZAR LIVRO POR ID           %s\n", NEGRITO, AZUL, NORMAL);
        printf("%s%s==============================================%s\n\n", NEGRITO, CIANO, NORMAL);

        // Novamente crio o livro para percorrer o .bin e exibir os nomes e id:
        Livros livro;
        while (fread(&livro, sizeof(Livros), 1, banco_livros)) {
            livro.nome[strcspn(livro.nome, "\n")] = '\0';
            printf("%s%s🆔 ID: %-5d%s | %s📘 %-30s%s\n", NEGRITO, AZUL, livro.id, NORMAL, AMARELO, livro.nome, NORMAL);
        }

        // Tratamento do ponteiro, fazendo voltar para o inicio do arquivo:
        rewind(banco_livros);
        // Usando a mesma função para checar se o numero digitado é inteiro:
        id_atualizar = checar_inteiro("\nDigite o ID do livro que deseja atualizar (0 para cancelar): ");

        if(id_atualizar == 0) {
            fclose(banco_livros);
            return;
        }

        // Aqui eu crio o arquivo temporario com tratamento de erro:
        FILE *temporario = fopen("temp.bin", "wb");
        if (temporario == NULL) {
            printf("%s%sErro ao criar arquivo temporário.%s\n", NEGRITO, VERMELHO, NORMAL);
            fclose(banco_livros);
            pausar();
            return;
        }

        // Crio o livro_editado para guardar as atualizações:
        Livros livro_editado;
        // Crio a variavel editado para saber se o usuario fez alguma edição:
        int editado = 0;

        // Percorre o .bin, e escreve no arquivo temporario os livros com as atualizações:
        while (fread(&livro, sizeof(Livros), 1, banco_livros)) {
            // Checo se o livro.id é igual ao id_atualizar:
            if (livro.id == id_atualizar) {
                encontrado = 1;
                livro_editado = livro;

                limpar_tela();
                printf("%s%s==============================================%s\n", NEGRITO, CIANO, NORMAL);
                printf("%s%s          EDITANDO LIVRO: %s%-30s%s\n", NEGRITO, AZUL, AMARELO, livro.nome, NORMAL);
                printf("%s%s==============================================%s\n\n", NEGRITO, CIANO, NORMAL);

                /* Aqui crio um menu para o usuario escolher o que ele deseja atualizar */
                do {
                    printf("%s%s📖 Informações atuais:%s\n", NEGRITO, CIANO, NORMAL);
                    printf("%sTítulo:%s %s\n", AZUL, NORMAL, livro_editado.nome);
                    printf("%sAutor:%s %s\n", AZUL, NORMAL, livro_editado.autor);
                    printf("%sEditora:%s %s\n", AZUL, NORMAL, livro_editado.editora);
                    printf("%sPáginas:%s %d\n", AZUL, NORMAL, livro_editado.paginas);
                    printf("%sAno:%s %d\n\n", AZUL, NORMAL, livro_editado.ano);

                    printf("%s%sO que deseja atualizar?%s\n", NEGRITO, CIANO, NORMAL);
                    printf("%s1.%s Título\n", VERDE, NORMAL);
                    printf("%s2.%s Autor\n", VERDE, NORMAL);
                    printf("%s3.%s Editora\n", VERDE, NORMAL);
                    printf("%s4.%s Páginas\n", VERDE, NORMAL);
                    printf("%s5.%s Ano\n", VERDE, NORMAL);
                    printf("%s6.%s Ver livro atualizado\n", CIANO, NORMAL);
                    printf("%s0.%s Finalizar edição\n\n", VERMELHO, NORMAL);

                    // Novamente uso a função checar_inteiro para checar se o numero digitado é inteiro:
                    opcao = checar_inteiro("Escolha uma opção: ");

                    // Limpa a tela após cada seleção para manter organizado
                    limpar_tela();
                    printf("%s%s==============================================%s\n", NEGRITO, CIANO, NORMAL);
                    printf("%s%s          EDITANDO LIVRO: %s%-30s%s\n", NEGRITO, AZUL, AMARELO, livro.nome, NORMAL);
                    printf("%s%s==============================================%s\n\n", NEGRITO, CIANO, NORMAL);

                    // Aqui eu uso o switch para o usuario escolher o que ele deseja atualizar:
                    switch(opcao) {
                        case 1:
                            printf("%sNovo título:%s ", AZUL, NORMAL);
                            fgets(livro_editado.nome, sizeof(livro_editado.nome), stdin);
                            livro_editado.nome[strcspn(livro_editado.nome, "\n")] = '\0';
                            editado = 1;
                            break;
                        case 2:
                            printf("%sNovo autor:%s ", AZUL, NORMAL);
                            fgets(livro_editado.autor, sizeof(livro_editado.autor), stdin);
                            livro_editado.autor[strcspn(livro_editado.autor, "\n")] = '\0';
                            editado = 1;
                            break;
                        case 3:
                            printf("%sNova editora:%s ", AZUL, NORMAL);
                            fgets(livro_editado.editora, sizeof(livro_editado.editora), stdin);
                            livro_editado.editora[strcspn(livro_editado.editora, "\n")] = '\0';
                            editado = 1;
                            break;
                        case 4:
                            livro_editado.paginas = checar_inteiro("Novo número de páginas: ");
                            editado = 1;
                            break;
                        case 5:
                            livro_editado.ano = checar_inteiro("Novo ano: ");
                            editado = 1;
                            break;
                        case 6:
                            printf("%s%s📖 Versão atualizada do livro:%s\n\n", NEGRITO, VERDE, NORMAL);
                            printf("%sTítulo:%s %s\n", AZUL, NORMAL, livro_editado.nome);
                            printf("%sAutor:%s %s\n", AZUL, NORMAL, livro_editado.autor);
                            printf("%sEditora:%s %s\n", AZUL, NORMAL, livro_editado.editora);
                            printf("%sPáginas:%s %d\n", AZUL, NORMAL, livro_editado.paginas);
                            printf("%sAno:%s %d\n\n", AZUL, NORMAL, livro_editado.ano);
                            pausar();
                            limpar_tela();
                            break;
                        case 0:
                            printf("%sEdição finalizada.%s\n", VERDE, NORMAL);
                            break;
                        default:
                            printf("%sOpção inválida!%s\n", VERMELHO, NORMAL);
                    }

                } while (opcao != 0);

                // Escreve o livro editado no arquivo temporário:
                fwrite(&livro_editado, sizeof(Livros), 1, temporario);
            }
            else {
                fwrite(&livro, sizeof(Livros), 1, temporario);
            }
        }

        // Fecho os arquivos:
        fclose(banco_livros);
        fclose(temporario);

        // Remove o original e renomeia o temporário:
        remove("banco_livros.bin");
        rename("temp.bin", "banco_livros.bin");

        // Feedback para o usuário:
        limpar_tela();
        printf("%s%s==============================================%s\n", NEGRITO, CIANO, NORMAL);
        if (encontrado) {
            if (editado) {
                printf("%s%s✅ Livro atualizado com sucesso!%s\n", NEGRITO, VERDE, NORMAL);
            } else {
                printf("%s%sℹ️ Nenhuma alteração foi feita.%s\n", NEGRITO, AZUL, NORMAL);
            }
        } else {
            printf("%s%s❌ Nenhum livro encontrado com o ID %d.%s\n", NEGRITO, VERMELHO, id_atualizar, NORMAL);
        }
        printf("%s%s==============================================%s\n", NEGRITO, CIANO, NORMAL);

        // Pergunto se deseja atualizar outro livro:
        char resposta;
        printf("\n%sDeseja atualizar outro livro? (s/n): %s", AMARELO, NORMAL);
        scanf(" %c", &resposta);
        limpar_buffer();

        continuar = (resposta == 's' || resposta == 'S');
    }
}