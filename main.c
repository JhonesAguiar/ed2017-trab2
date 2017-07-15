#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compacta.h"
#include "descompacta.h"

int main(int agrc, char *argv[])
{
    int x = 1;
    if(/*strcmp(argv[0],"./Compacta")*/x == 0)
    {
        FILE *entrada;

        /*
                if (argv[1] == NULL){
                    printf("Nenhum arquivo de entrada foi informado!");
                    return 1;
                }else{
                    entrada = fopen(argv[1], "rb");
                }

                if (entrada == NULL) {
                   printf ("Houve um erro ao abrir o arquivo de Entrada.\n");
                   return 1;
                }
        */
        entrada = fopen("entrada", "rb");

        if (entrada == NULL)
        {
            printf ("Houve um erro ao abrir o arquivo de Entrada.\n");
            return 1;
        }

        int v[256];
        inicia_vetor(v);
        ler_caracteres(entrada,v);

        /*
        int i;

        for(i=0; i<256; i++)
        {
            printf("%i- ",i);
            printf("%c: ",i);
            printf("%i\n",v[i]);
        }
        */

        gera_arvore(v);


        return 0;
    }
    if(/*strcmp(argv[0],"Descompacta")*/ x == 1)
    {
        unsigned char vetArvore[60];
        unsigned char vetTexto[14];

        Arv* arv = CriaVazia();

        vetArvore[0] = 0;
        vetArvore[1] = 0;
        vetArvore[2] = 1;
        vetArvore[3] = 0;
        vetArvore[4] = 1;
        vetArvore[5] = 1;
        vetArvore[6] = 0;
        vetArvore[7] = 1;
        vetArvore[8] = 1;
        vetArvore[9] = 1;
        vetArvore[10] = 1;
        vetArvore[11] = 1;
        vetArvore[12] = 0;
        vetArvore[13] = 1;
        vetArvore[14] = 1;
        vetArvore[15] = 0;
        vetArvore[16] = 1;
        vetArvore[17] = 1;
        vetArvore[18] = 0;
        vetArvore[19] = 1;
        vetArvore[20] = 0;
        vetArvore[21] = 0;
        vetArvore[22] = 1;
        vetArvore[23] = 0;
        vetArvore[24] = 1;
        vetArvore[25] = 1;
        vetArvore[26] = 0;
        vetArvore[27] = 0;
        vetArvore[28] = 1;
        vetArvore[29] = 0;
        vetArvore[30] = 1;
        vetArvore[31] = 1;
        vetArvore[32] = 0;
        vetArvore[33] = 1;
        vetArvore[34] = 1;
        vetArvore[35] = 1;
        vetArvore[36] = 0;
        vetArvore[37] = 0;
        vetArvore[38] = 1;
        vetArvore[39] = 1;
        vetArvore[40] = 0;
        vetArvore[41] = 1;
        vetArvore[42] = 0;
        vetArvore[43] = 0;
        vetArvore[44] = 1;
        vetArvore[45] = 0;
        vetArvore[46] = 0;
        vetArvore[47] = 0;
        vetArvore[48] = 0;
        vetArvore[49] = 0;
        vetArvore[50] = 1;
        vetArvore[51] = 0;
        vetArvore[52] = 1;
        vetArvore[53] = 1;
        vetArvore[54] = 0;
        vetArvore[55] = 0;
        vetArvore[56] = 0;
        vetArvore[57] = 1;
        vetArvore[58] = 0;

        vetTexto[0] = 1;
        vetTexto[1] = 1;
        vetTexto[2] = 1;
        vetTexto[3] = 0;
        vetTexto[4] = 1;
        vetTexto[5] = 1;
        vetTexto[6] = 0;
        vetTexto[7] = 1;
        vetTexto[8] = 0;
        vetTexto[9] = 0;
        vetTexto[10] = 1;
        vetTexto[11] = 0;
        vetTexto[12] = 0;

        arv = DescompactaArquivo(vetArvore, vetTexto, 1024, 59, 13, 13);
        return 0;
    }
    return 666;
}
