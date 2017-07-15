#include <stdio.h>
#include <stdlib.h>

#include "arvore.h"
#include "descompacta.h"
#include "bitmap/bitmap.h"


void CriaArvoreAPartirBits(Arv* arv, bitmap* bm, unsigned int* posicaoAtual, int maxTamVet)
{
    unsigned char caracter;
    unsigned char temp;
    unsigned int posicaoNoCaracter;

    printf("\nPosicao Atual: %d\n", *posicaoAtual);

    if(*posicaoAtual < maxTamVet)
    {
        if(bitmapGetBit(*bm, *posicaoAtual) == 0b00000000)
        {
            GravarAtributoEsquerda(arv, CriaNo(0, NULL, NULL));
            *posicaoAtual = *posicaoAtual + 1;
            CriaArvoreAPartirBits(AcessarAtributoEsquerda(arv), bm, posicaoAtual, maxTamVet);
        }
        else
        {
            *posicaoAtual = *posicaoAtual + 1;
            caracter = 0;

            for(posicaoNoCaracter = 0; posicaoNoCaracter < 8; posicaoNoCaracter = posicaoNoCaracter + 1)
            {
                temp = bitmapGetBit(*bm, *posicaoAtual + posicaoNoCaracter);
                temp = temp<<(7 - posicaoNoCaracter);
                caracter = caracter | temp;
            }

            *posicaoAtual = *posicaoAtual + 8;

            GravarAtributoEsquerda(arv, CriaFolha(0, caracter));
        }

        printf("\nPosicao Atual: %d\n", *posicaoAtual);

        if(*posicaoAtual < maxTamVet)
        {
            if(bitmapGetBit(*bm, *posicaoAtual) == 0b00000000)
            {
                printf("\nTeste\n");
                GravarAtributoDireita(arv, CriaNo(0, NULL, NULL));
                *posicaoAtual = *posicaoAtual + 1;
                CriaArvoreAPartirBits(AcessarAtributoDireita(arv), bm, posicaoAtual, maxTamVet);
            }
            else
            {
                *posicaoAtual = *posicaoAtual + 1;
                caracter = 0;

                for(posicaoNoCaracter = 0; posicaoNoCaracter < 8; posicaoNoCaracter = posicaoNoCaracter + 1)
                {
                    temp = bitmapGetBit(*bm, *posicaoAtual + posicaoNoCaracter);
                    temp = temp<<(7 - posicaoNoCaracter);
                    caracter = caracter | temp;
                }

                *posicaoAtual = *posicaoAtual + 8;

                GravarAtributoDireita(arv, CriaFolha(0, caracter));
            }
        }
    }
}

void CriaTextoAPartirBits(unsigned char* texto, Arv* arv, bitmap bm, int tamanhoTextoCodificado)
{
    unsigned int posicaoAtual;
    unsigned char bitAtual;
    int tamanhoAtualTexto;

    tamanhoAtualTexto = 0;

    Arv* noAtual = arv;

    for(posicaoAtual = 0; posicaoAtual < tamanhoTextoCodificado; posicaoAtual = posicaoAtual + 1)
    {
        printf("Posicao atual: %d\n", posicaoAtual);
        bitAtual = bitmapGetBit(bm, posicaoAtual);

        if(bitAtual == 0b00000000)
        {
            noAtual = AcessarAtributoEsquerda(noAtual);
        }
        else
        {
            noAtual = AcessarAtributoDireita(noAtual);
        }

        if(AcessarTipo(noAtual) == 1)
        {
            texto[tamanhoAtualTexto] = AcessarCaracter(noAtual);
            tamanhoAtualTexto++;
            noAtual = arv;
        }
    }
}

Arv* DescompactaArquivo(unsigned char vetArvore[], unsigned char vetTexto[], unsigned int maxTamBitmap, int maxTamVet, int maxTamTexto, int tamanhoTextoCodificado)
{
    unsigned int posicaoAtual;
    unsigned char* texto;

    Arv* arv = CriaVazia();

    arv = CriaNo(0, NULL, NULL);

    texto = malloc(sizeof(unsigned char)*6);

    bitmap bmArvore = bitmapInit(maxTamBitmap);
    bitmap bmTexto = bitmapInit(maxTamBitmap);

    for(posicaoAtual = 0; posicaoAtual < maxTamVet; posicaoAtual++)
    {
        bitmapAppendLeastSignificantBit(&bmArvore, vetArvore[posicaoAtual]);
    }

    posicaoAtual = 1;

    CriaArvoreAPartirBits(arv, &bmArvore, &posicaoAtual, maxTamVet);

    printf("\n");
    Imprime(arv);

    for(posicaoAtual = 0; posicaoAtual < tamanhoTextoCodificado; posicaoAtual++)
    {
        bitmapAppendLeastSignificantBit(&bmTexto, vetTexto[posicaoAtual]);
    }

    CriaTextoAPartirBits(texto, arv, bmTexto, tamanhoTextoCodificado);

    printf("Texto: %s", texto);

    return arv;
}







