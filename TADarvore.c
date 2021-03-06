#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap/bitmap.h"
#include "arvore.h"

typedef struct no No;
struct no
{
    int peso;
};

typedef struct folha Folha;
struct folha
{
    int peso;
    unsigned char c;
};

struct arvore
{
    Arv *esquerda;
    Arv *direita;
    void *info;
    int tipo;    //0 para no, 1 para folha.
};

Arv* CriaVazia()
{
    return NULL;
}

int RetornaPeso(Arv *a)
{
    if(a->tipo == 1) return ((Folha*)a->info)->peso;
    else if(a->tipo == 0) return ((No*)a->info)->peso;
    else return -1;
}

Arv* CriaNo(int peso, Arv* esquerda, Arv* direita)
{
    Arv* a = (Arv*)malloc(sizeof(Arv));
    a->info = (No*)malloc(sizeof(No));

    a->tipo = 0;

    ((No*)a->info)->peso = peso;
    a->esquerda = esquerda;
    a->direita = direita;

    return a;
}

Arv* CriaFolha(int peso, unsigned char c)
{
    Arv* a = (Arv*)malloc(sizeof(Arv));
    a->info = (Folha*)malloc(sizeof(Folha));

    a->tipo = 1;

    ((Folha*)a->info)->peso = peso;
    ((Folha*)a->info)->c = c;

    a->esquerda = NULL;
    a->direita = NULL;

    return a;
}
/*
void Conta(Arv* arv)
{
    if(arv != NULL)
    {
        if(arv->tipo == 0)
        {
            printf("%d ", ((No*)arv->info)->peso);
            Conta(arv->esquerda);

            Conta(arv->direita);
        }
        else
        {
            printf("%d ", ((Folha*)arv->info)->peso);
            printf("%c ", ((Folha*)arv->info)->c);
        }
    }
}
*/

void Imprime(Arv* arv)
{
    if(arv != NULL)
    {
        if(arv->tipo == 0)
        {
            printf("%d \n", ((No*)arv->info)->peso);
            printf("Esquerda: ");
            Imprime(arv->esquerda);
            printf(" Direita: ");
            Imprime(arv->direita);
        }
        else
        {
            printf("%d ", ((Folha*)arv->info)->peso);
            printf("Caracter ");
            printf("%c ", ((Folha*)arv->info)->c);
        }
    }
}

void ImprimeArvCodificada(Arv* arv)
{
    if(arv != NULL)
    {
        if(arv->tipo == 0)
        {
            printf("0 ");
            ImprimeArvCodificada(arv->esquerda);
            ImprimeArvCodificada(arv->direita);
        }
        else
        {
            printf("1 ");
            printf("%i ", ((Folha*)arv->info)->c);
        }
    }
}

Arv* Libera(Arv* arv)
{
    if(arv != NULL)
    {
        if(arv->tipo == 0)
        {
            Libera(arv->esquerda);
            Libera(arv->direita);
            free((No*)arv->info);
            free(arv);
        }
        else
        {
            free((Folha*)arv->info);
            free(arv);
        }
    }

    return NULL;
}

/*
float RealizaOperacao(int a, int b, char operacao)
{
    float resultado = 0;

    if(operacao == '+')
    {
        resultado = a + b;

        return resultado;
    }

    if(operacao == '-')
    {
        resultado = a - b;

        return resultado;
    }

    if(operacao == '*')
    {
        resultado = a * b;

    return resultado;
    }

    if(operacao == '/')
    {
        resultado = a / b;

        return resultado;
    }

    return resultado;
}
*/

/*
float Avalia(Arv* arv)
{
    float resultado = 0;
    float num1 = 0;
    float num2 = 0;

    if(arv != NULL)
    {
        if(arv->tipo == 0)
        {
            num1 = Avalia(((No*)arv->info)->esquerda);
            num2 = Avalia(((No*)arv->info)->direita);

            resultado = RealizaOperacao(num1, num2, ((No*)arv->info)->operador);
        }
        else
        {
            resultado = ((Folha*)arv->info)->num;
        }
    }

    return resultado;
}
*/

int Folhas(Arv* arv)
{
    int qtd = 0;

    if(arv != NULL)
    {
        if(arv->tipo == 0)
        {
            qtd += Folhas(arv->esquerda);
            qtd += Folhas(arv->direita);
        }
        else
        {
            qtd += 1;
        }
    }

    return qtd;
}

int Altura(Arv* arv)
{
    int altura = 0;
    int alturaTemp1 = 0;
    int alturaTemp2 = 0;

    if(arv != NULL)
    {
        if(arv->tipo == 0)
        {
            alturaTemp1 += 1 + Altura(arv->esquerda);
            alturaTemp2 += 1 + Altura(arv->direita);

            if(alturaTemp1 > alturaTemp2)
            {
                altura += alturaTemp1;
            }
            else
            {
                altura += alturaTemp2;
            }
        }
    }
    else
    {
        altura = -1;
    }

    return altura;
}

int BuscaCaracter(Arv *arv, void *bm, unsigned char c){
    bitmap *bmcod = (bitmap*)bm;
    if(arv->tipo == 0){
        if(BuscaCaracter(arv->esquerda,bmcod,c) == 1){
            bitmapAppendLeastSignificantBit(bmcod,0b00000000);
            return 1;
        }else if(BuscaCaracter(arv->direita,bmcod,c) == 1){
            bitmapAppendLeastSignificantBit(bmcod,0b00000001);
            return 1;
        }
    }else{
        if(((Folha*)arv->info)->c == c) return 1;
        else return 0;
    }
    return 0;
}
/*
    char aux[8]="";
    if(arv != NULL)
    {
        if(arv->tipo == 0)
        {
            aux[i]='0';
            aux[i+1]='\0';
            i++;

            BuscaCaracter(arv->esquerda,aux,c,i);
            i--;

            aux[i]='1';
            aux[i+1]='\0';
            i++;

            BuscaCaracter(arv->direita,aux,c,i);
            i--;
        }
        else
        {
            if((((Folha*)arv->info)->c) == c){
                strcpy(cod,aux);
            }
        }
    }
*/

Arv* AcessarAtributoEsquerda(Arv* arv)
{
    return arv->esquerda;
}

Arv* AcessarAtributoDireita(Arv* arv)
{
    return arv->direita;
}

int AcessarTipo(Arv* arv)
{
    return arv->tipo;
}

unsigned int AcessarCaracter(Arv* arv)
{
    return ((Folha*)arv->info)->c;
}

void GravarAtributoEsquerda(Arv* arv, Arv* esquerda)
{
    arv->esquerda = esquerda;
}

void GravarAtributoDireita(Arv* arv, Arv* direita)
{
    arv->direita = direita;
}
