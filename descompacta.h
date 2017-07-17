#ifndef DESCOMPACTA_H_INCLUDED
#define DESCOMPACTA_H_INCLUDED

#include "arvore.h"
#include "bitmap/bitmap.h"

/*
 * Descompacta um arquivo informado pelo usu�rio, gerando um arquivo de mesmo nome
 * inputs: um ponteiro para o arquivo compactado informado pelo usu�rio, o tamanho m�ximo do bitmap (em bits) para o texto do arquivo e o nome do arquivo
 * output: nenhum
 * pre-condicao: o arquivo informado existe e foi acessado com sucesso;
 * pos-condicao: foi criado um arquivo contendo o texto descompactado
*/
void DescompactaArquivo(FILE* arquivoCompactado, int maxTamBitmap, char nomeArquivoCompactado[]);

#endif // DESCOMPACTA_H_INCLUDED
