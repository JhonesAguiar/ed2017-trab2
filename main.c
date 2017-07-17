#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compacta.h"
#include "descompacta.h"

int main(int agrc, char *argv[])
{
    int x = 1;


    if(/*strcmp(argv[0],"Descompacta")*/ x == 1)
    {
        FILE* arquivoCompactado;
        //Tamanho máximo do bitmap criado para armazenar os bits do texto compactado
        int maxTamBitmap;
        char nomeArquivo[6] = "teste";

        arquivoCompactado = fopen("VENSSO_REQ_20050713.comp", "rb");

        maxTamBitmap = 33554432;
        //maxTamBitmap = 8192;

        if (arquivoCompactado == NULL)
        {
            printf ("Houve um erro ao abrir o arquivo compactado.\n");
            return 1;
        }

        //Função que realiza todo o processo de descompactação
        DescompactaArquivo(arquivoCompactado, maxTamBitmap, nomeArquivo);

        fclose(arquivoCompactado);

        return 0;
    }
    return 666;
}
