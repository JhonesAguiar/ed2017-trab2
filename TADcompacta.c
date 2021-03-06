#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compacta.h"
#include "arvore.h"

//Zera todos as posi��es de um vetor de 256 posi��o, Generalizar isso?
void inicia_vetor(int *v)
{
    int i;
    for(i=0; i<256; i++)
    {
        v[i]=0;
    }
}

//L� todo o arquivo de entrada
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

/*Nem todos os caracteres (bytes) podem ser usados no arquivo, essa fun��o conta quantos realmente est�o sendo usados
Ou seja, conta quandas posi��es do vetor de 256 posi��es s�o diferentes de 0*/
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

//Essa fun��o vai gerar um vetor onde cada posi��o do vetor � uma folha da arvore de codifica��o,
//A �rvore vai ser gerada a partir desse vetor

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

//fun��o que vai fazer a compara��o do peso, e vai ser utilizada pela qsort para ordenar o vetor de forma crescente
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

//por enquanto a fun��o que faz tudo e nada simultaneamente.
void gera_arvore(int *v)
{
    //int i;
    int k,peso;
    Arv **folhas;
    k = caracteres_usados(v);

    folhas = gera_folhas(v,k);

    qsort(folhas,k,sizeof(Arv*),compara);

    int ultm=k-1;
    while(ultm > 0){
        peso = RetornaPeso(folhas[0]) + RetornaPeso(folhas[1]);

        Arv *a = CriaNo(peso,folhas[0],folhas[1]);

        ultm = RemoveArvVet(folhas,ultm);
        ultm = RemoveArvVet(folhas,ultm);

        ultm++;
        folhas[ultm] = a;

        qsort(folhas,ultm+1,sizeof(Arv*),compara);
    }

    ImprimeArvCodificada(folhas[0]);
/*
        for(i=0;i<k;i++){
            printf("%i: ",i+1);
            ImprimeArvCodificada(folhas[i]);
            printf("\n");
        }
*/

}
