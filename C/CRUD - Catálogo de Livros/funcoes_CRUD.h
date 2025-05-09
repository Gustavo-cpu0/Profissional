#ifndef FUNCOES_CRUD_H
#define FUNCOES_CRUD_H
// Prototipos das funções de CRUD:
// Função para adicionar novo livro (coleta dados e salva em binário)
void adicionar_livro();
// Lista todos os livros do arquivo binário formatados
void listar_livros();
// Remove livro selecionado por ID
void apagar_livro();
// Edita livro selecionado por ID (atualiza campos)
void atualizar_livro();
#endif