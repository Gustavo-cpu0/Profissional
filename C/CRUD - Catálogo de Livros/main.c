#include "estrutura_livros.h"

int main() {
  int escolha, esc_bd;

  while (1) {
    limpar_tela();
    
    escolha = menu();

    switch (escolha) {

      case 1:
        limpar_tela();
        listar_livros();
        break;
      
      case 2:
        limpar_tela();
        adicionar_livro();
        break;
      case 3:
        limpar_tela();
        atualizar_livro();
        break;
      case 4:
        limpar_tela();
        apagar_livro();
        break;
      case 0:
        printf("%s%sObrigado por usar o programa!%s\n", NEGRITO, VERDE, NORMAL);
        return 0;
      
      default:
        printf("%s%sOpção inválida!%s\n", NEGRITO, VERMELHO, NORMAL);
    }
      
  }



return 0;
}