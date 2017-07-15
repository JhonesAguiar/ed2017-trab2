#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compacta.h"
#include "arvore.h"
#include "bitmap/bitmap.h"

//Zera todos as posições de um vetor de 256 posição, Generalizar isso?
void inicia_vetor(int *v)
{
    int i;
    for(i=0; i<256; i++)
    {
        v[i]=0;
    }
}

//Lê todo o arquivo de entrada
void ler_caracteres(FILE *arq, int *v)
{
    unsigned char c[1];
    int i;
    while(!feof(arq))
    {
        //fread vai ler byte a byte do arquivo binario
        i=fread(c,sizeof(c),1,arq);
        if(i == 1) v[c[0]]++;
        //conta quantas vezes aquele byte se repete no arquivo
    }
}

/*Nem todos os caracteres (bytes) podem ser usados no arquivo, essa função conta quantos realmente estão sendo usados
Ou seja, conta quandas posições do vetor de 256 posições são diferentes de 0*/
int caracteres_usados(int *v)
{
    int i,k=256;
    for(i=0; i<256; i++)
    {
        if(v[i] == 0)
        {
            k--;
        }
    }
    return k;
}

//Essa função vai gerar um vetor onde cada posição do vetor é uma folha da arvore de codificação,
//A árvore vai ser gerada a partir desse vetor

Arv** gera_folhas(int *v, int k)
{
    int i,j=0;
    Arv **folhas = (Arv**)malloc(k*sizeof(Arv*));

    for(i=0; i<256; i++)
    {
        if(v[i] != 0)
        {
            Arv *a = CriaFolha(v[i],i);
            folhas[j] = a;
            j++;
        }
    }
    return folhas;
}

//função que vai fazer a comparação do peso, e vai ser utilizada pela qsort para ordenar o vetor de forma crescente
int compara (const void* v1, const void* v2)
{
    Arv *a1 = *(Arv**)v1;
    Arv *a2 = *(Arv**)v2;

    int pesoA1, pesoA2;
    pesoA1 = RetornaPeso(a1);
    pesoA2 = RetornaPeso(a2);

    if (pesoA1 > pesoA2) return 1;
    else if (pesoA1 < pesoA2) return -1;
    else return 0;
}

int RemoveArvVet(Arv **arv,int ultm){
    int i;
    ultm --;
    for(i=0;i<=ultm;i++){
        arv[i] = arv[i+1];
    }
    arv[ultm+1]=0;
    return ultm;
}


char* ExtensaoArquivo(char *nome) {
    char *dot = strrchr(nome, '.');
    if(!dot || dot == nome) return "";
    return dot;
}

char* NomeArquivo(char *nome) {
    char *dot = strrchr (nome, '.');
    if (dot != NULL)
        *dot = '\0';
    return nome;
}

void escreve_caracteres(FILE *arq, void *info, int i)
{
    fwrite(info,1,i,arq);
}

void CodificarArvore(Arv* arv, bitmap *bm)
{
    if(arv != NULL)
    {
        if(AcessarTipo(arv) == 0)
        {
            bitmapAppendLeastSignificantBit(bm,0b00000000);
            CodificarArvore(AcessarAtributoEsquerda(arv),bm);
            CodificarArvore(AcessarAtributoDireita(arv),bm);
        }
        else
        {
            bitmapAppendLeastSignificantBit(bm,0x00000001);
            unsigned int c = AcessarCaracter(arv);
            int i;

            for (i=0;i<8;i++)
            {
                if (c&0b10000000){
                    bitmapAppendLeastSignificantBit(bm,0x00000001);
                }
                else{
                    bitmapAppendLeastSignificantBit(bm,0x00000000);
                }
                c<<=1;
            }
        }
    }
}

//por enquanto a função que faz tudo e nada simultaneamente.
void gera_arvore(int *v, char *nomeArquivo)
{
    FILE *saida;
    char extensao[10];
    strcpy(extensao,ExtensaoArquivo(nomeArquivo));

    char *nome = NomeArquivo(nomeArquivo);
    nome = strcat(nome,".comp");

    saida = fopen(nome, "wb");

    if (saida == NULL)
    {
        printf ("Houve um erro ao criar o arquivo de Saída.\n");
    }

    int tamNome = strlen(extensao);
    tamNome *= 8;

    escreve_caracteres(saida,&tamNome,1);
    escreve_caracteres(saida,extensao,tamNome/8);



    //int i;
    int k,peso,tamArvore;
    Arv **folhas;
    k = caracteres_usados(v);
    tamArvore = k*9;

    folhas = gera_folhas(v,k);

    qsort(folhas,k,sizeof(Arv*),compara);

    int ultm=k-1;
    while(ultm > 0){
        peso = RetornaPeso(folhas[0]) + RetornaPeso(folhas[1]);

        Arv *a = CriaNo(peso,folhas[0],folhas[1]);
        tamArvore++;

        ultm = RemoveArvVet(folhas,ultm);
        ultm = RemoveArvVet(folhas,ultm);

        ultm++;
        folhas[ultm] = a;

        qsort(folhas,ultm+1,sizeof(Arv*),compara);
    }

    Arv *arvCodificacao = folhas[0];
    free(folhas);

    ImprimeArvCodificada(arvCodificacao);

    tamArvore = (tamArvore+15)/8;
    printf("\n%i",tamArvore);
    tamArvore*=8;
    bitmap arvore = bitmapInit(tamArvore);

    CodificarArvore(arvCodificacao,&arvore);

    escreve_caracteres(saida,&tamArvore,1);
    escreve_caracteres(saida,arvore.contents,tamArvore/8);




/*
        for(i=0;i<k;i++){
            printf("%i: ",i+1);
            ImprimeArvCodificada(folhas[i]);
            printf("\n");
        }
*/

}
