#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arvore.h"
#include "descompacta.h"
#include "bitmap/bitmap.h"


/*
 * A partir de uma sequencia de bits, gera a arvore de codifica��o correspondente
 * inputs: um ponteiro para uma arvore (que s� possui um n�); um ponteiro para o bitmap que cont�m a sequ�ncia de bits;
           o index para o percorrimento do bitmap; a quantidade de bits na sequ�ncia de bits passada
 * output: nenhum
 * pre-condicao: a arvore est� corretamente inicializada; o bitmap est� corretamente inicializado e preenchido;
 * pos-condicao: tem-se uma arvore de codifica��o
*/
void CriaArvoreAPartirBits(Arv* arv, bitmap* bm, int* posicaoAtual, short int qtdBitsArvore)
{
    unsigned char caracter;
    unsigned char temp;
    int posicaoNoCaracter;

    //Para de ler ap�s alcan�ar a quantidade de bits informadas no arquivo compactado
    if(*posicaoAtual < qtdBitsArvore)
    {
        //Se o bit lido for 0, � um n� raiz, caso contr�rio, um n� folha
        if(bitmapGetBit(*bm, *posicaoAtual) == 0b00000000)
        {
            //Cria um n� raiz nulo a esquerda do n� atual
            GravarAtributoEsquerda(arv, CriaNo(0, NULL, NULL));
            *posicaoAtual = *posicaoAtual + 1;
            CriaArvoreAPartirBits(AcessarAtributoEsquerda(arv), bm, posicaoAtual, qtdBitsArvore);
        }
        else
        {
            *posicaoAtual = *posicaoAtual + 1;
            caracter = 0b00000000;

            //L� os bits correspondentes ao caracter armazenado na folha
            for(posicaoNoCaracter = 0; posicaoNoCaracter < 8; posicaoNoCaracter = posicaoNoCaracter + 1)
            {
                temp = bitmapGetBit(*bm, *posicaoAtual + posicaoNoCaracter);
                temp = temp<<(7 - posicaoNoCaracter);
                caracter = caracter | temp;
            }
            //Um caractere tem 8 bits, logo pula 8 bits ap�s ler um caractere
            *posicaoAtual = *posicaoAtual + 8;

            //Cria um n� folha a esquerda do n� atual
            GravarAtributoEsquerda(arv, CriaFolha(1, caracter));
        }

        //Repete o mesmo processo, por�m para o ramo direito da �rvore
        if(*posicaoAtual < qtdBitsArvore)
        {
            if(bitmapGetBit(*bm, *posicaoAtual) == 0b00000000)
            {
                GravarAtributoDireita(arv, CriaNo(0, NULL, NULL));
                *posicaoAtual = *posicaoAtual + 1;
                CriaArvoreAPartirBits(AcessarAtributoDireita(arv), bm, posicaoAtual, qtdBitsArvore);
            }
            else
            {
                *posicaoAtual = *posicaoAtual + 1;
                caracter = 0b00000000;

                for(posicaoNoCaracter = 0; posicaoNoCaracter < 8; posicaoNoCaracter = posicaoNoCaracter + 1)
                {
                    temp = bitmapGetBit(*bm, *posicaoAtual + posicaoNoCaracter);
                    temp = temp<<(7 - posicaoNoCaracter);
                    caracter = caracter | temp;
                }

                *posicaoAtual = *posicaoAtual + 8;

                GravarAtributoDireita(arv, CriaFolha(1, caracter));
            }
        }
    }
}

/*
 * L� no arquivo compactado a extens�o que o arquivo descompactado dever� ter
 * inputs: um ponteiro para o arquivo compactado
 * output: um ponteiro para a string contendo a extens�o
 * pre-condicao: o arquivo compactado foi acessado corretamente
 * pos-condicao: tem-se a exten��o do arquivo
*/
char* LeExtensaoArquivo(FILE* arquivoCompactado)
{
    unsigned char tamanhoExtensao[1];

    fread(tamanhoExtensao, sizeof(unsigned char), 1, arquivoCompactado);

    char* extensao = (char*)malloc(sizeof(char) * (((int)tamanhoExtensao[0]/8) + 1));

    fread(extensao, sizeof(char), ((int)tamanhoExtensao[0]/8), arquivoCompactado);

    extensao[tamanhoExtensao[0]/8] = '\0';

    return extensao;
}

/*
 * Le a sequencia de bits lida no arquivo compactado que corresponde a arvore de codifica��o e armazena na vari�vel com endere�o passado
 * inputs: um ponteiro para o arquivo compactado, o endere�o de mem�ria da vari�vel que vai armazenar a quantidade de bits que a �rvore possui al�m
           do endere�o de mem�ria da vari�vel que vai armazenar o bitmap da arvore
 * output: nenhum
 * pre-condicao: o arquivo compactado foi acessado corretamente, e o bitmap esta inicializado
 * pos-condicao: tem-se um bitmap contendo a sequencia de bits correspondentes a arvore de codifica��o e a quantidade de bits
*/
void LeArvore(FILE* arquivoCompactado, short int* qtdBitsArvore, bitmap* bmArvore)
{
    unsigned int i;
    unsigned int j;
    //quantidade de bits que tem a �rvore de codifica��o
    short int tamanhoArvore[1];
    //Tamanho que o vetor que vai armazenar os caracteres correspondentes a �rvore vai ter (j� que n�o � poss�vel ler bit a bit)
    int tamanhoVetorArvore;

    //Le no arquivo compactado a quantidade de bits que tem a �rvore de codifica��o
    fread(tamanhoArvore, sizeof(short int), 1, arquivoCompactado);

    *qtdBitsArvore = tamanhoArvore[0];

    //Se o tamanho da �rvore n�o for m�ltiplo de 8, l� um caractere a mais, ignorando os bits que vierem ap�s se completar a quantidade de bits
    //correspondete ao tamanho da �rvore
    if((int)tamanhoArvore[0] % 8 != 0)
    {
        tamanhoVetorArvore = tamanhoArvore[0]/8 + 1;
    }
    else
    {
        tamanhoVetorArvore = tamanhoArvore[0]/8;
    }

    unsigned char arvore[tamanhoVetorArvore + 1];

    fread(arvore, sizeof(char), tamanhoVetorArvore, arquivoCompactado);

    for(i = 0; i < tamanhoVetorArvore; i++)
    {
        for(j = 0; j < 8; j++)
        {
            if((i * 8) + j >= tamanhoArvore[0])
            {
                break;
            }

            unsigned char temp;
            //P�e o bit que se quer como o bit menos significante do caractere, para que possa ser corretamente inserido no bitmap
            temp = arvore[i]>>(7 - j);

            bitmapAppendLeastSignificantBit(bmArvore, temp);
        }
    }
}

/*
 * L� no arquivo compactado o tamanho (em bits) do texto armazenado
 * inputs: o endere�o de memoria do arquivo compactado
 * output: o tamanho do texto
 * pre-condicao: o arquivo compactado foi acessado corretamente
 * pos-condicao: tem-se o tamanho do texto
*/
int LeTamanhoTexto(FILE* arquivoCompactado)
{
    int tamanhoTexto[1];

    fread(tamanhoTexto, sizeof(int), 1, arquivoCompactado);

    return tamanhoTexto[0];
}

/*
 * Grava em um arquivo o texto descompactado
 * inputs: o endere�o de mem�ria do arquivo descompactado; o texto a ser gravadado e a quantidade de caracteres desse texto
 * output: a quantidade de caracteres gravados
 * pre-condicao: o arquivo compactado foi acessado corretamente
 * pos-condicao: foi criado (ao adicionado ao arquivo, caso ele ja tenha sido criado antes) o texto descompactado
*/
int GravaTextoNoArquivoDescompactado(FILE* arquivoDescompactado, unsigned char texto[], int tamanhoTexto)
{
    int i;
    int qtdBytesEscritos = 0;

    //Escreve byte a byte o texto decodificado no arquivo
    for(i = 0; i < tamanhoTexto; i++)
    {
        qtdBytesEscritos += fwrite(&texto[i], sizeof(unsigned char), 1, arquivoDescompactado);
    }

    return qtdBytesEscritos;
}

/*
 * L� no arquivo compactado o texto a ser descompactado e ap�s isso percorre a �rvore de codifica��o para achar os caracteres correspondentes
   a essa sequ�ncia de bits e grava em uma string
 * inputs: o endere�o de mem�ria do arquivo descompactado; o endere�o de mem�ria do bitmap que armazenar� a sequ�ncia de bits correspondente ao texto;
           a quantidade de caracteres que ser�o lidos do arquivo; um ponteiro para a vari�vel que vai armazenar o texto;
           a arvore de codifica��o; o endere�o da vari�vel que vai armazenar a quantidade de caracteres j� decodificados; o tamanho alocado para o bitmap que cont�m o texto compactado;
           o endere�o da vari�vel que vai armazenar a quantidade de caracteres que n�o foram lidos (considerando o numero de caracteres a serem lidos, passado como par�metro);
           a posi��o do bitmap a partir da qual v�o ser armazenados novos bits; o bitmap contendo os bits que n�o foram lidos (considerando o numero de caracteres a serem lidos, passado como par�metro)
 * output: nenhum
 * pre-condicao: o arquivo compactado foi acessado corretamente; a quantidade de caracteres a serem lidos n�o ultrapassa a quantidade de caracteres restantes
                 no arquivo; os bitmaps passados est�o inicializados; os numeros passados s�o v�lidos
 * pos-condicao: tem-se uma determinada quantidade de bits decodificados
*/
void LeTexto(FILE* arquivoCompactado, bitmap* bm, int qtdBitsASeremLidos, unsigned char* textoDescompactado, Arv* arv, int* tamanhoAtualTexto, int maxTamBitmap, int* qtdBitsExcedentes, int posicaoInicialBm, bitmap* bmBitsExcedentes)
{
    Arv* noAtual;
    int i;
    int j;
    int quantidadeEmByteDeCaracteresASeremLidos;

    *qtdBitsExcedentes = qtdBitsASeremLidos;

    *tamanhoAtualTexto = 0;

    //Se a quantidade de bits a serem lidos n�o for multiplo de 8, a quantidade de bytes a ser lido vai ser o resta da divis�o inteira de
    //da quantidade de bits por 8, somado a 1

    if(qtdBitsASeremLidos % 8 != 0)
    {
        quantidadeEmByteDeCaracteresASeremLidos = (qtdBitsASeremLidos / 8) + 1;
    }
    else
    {
        quantidadeEmByteDeCaracteresASeremLidos = qtdBitsASeremLidos / 8;
    }

    unsigned char* texto = (unsigned char*)malloc(sizeof(unsigned char) * (quantidadeEmByteDeCaracteresASeremLidos + 1));

    noAtual = arv;
    //Decodifica os bits restantes da execu��o anterior
    for(i = 0; i < posicaoInicialBm; i++)
    {
        unsigned char bit;

        bit = bitmapGetBit(*bm, i);

        if(bit == 0b00000000)
        {
            noAtual = AcessarAtributoEsquerda(noAtual);
        }
        else
        {
            noAtual = AcessarAtributoDireita(noAtual);
        }
        //Se o n� atual for uma folha, grava o caracter da folha em uma string
        if(AcessarTipo(noAtual) == 1)
        {
            textoDescompactado[*tamanhoAtualTexto] = AcessarCaracter(noAtual);
            *tamanhoAtualTexto = *tamanhoAtualTexto + 1;
            noAtual = arv;
        }
    }

    //L� novos bits no arquivo e os decodifica
    for(i = 0; i < quantidadeEmByteDeCaracteresASeremLidos; i++)
    {
        //Le um caracter (8 bits) do arquivo compactado
        fread(&texto[i], sizeof(unsigned char), 1, arquivoCompactado);

        for(j = 0; j < 8; j++)
        {
            unsigned char temp;
            unsigned char bit;
            //Quantidade de bits j� lidos - 1, ou seja, a posi��o do bit atual no bitmap
            int posicao = (i * 8) + j;

            //Encerra a fun��o se o numero de bites j� lidos ultrapassa a quantidade de bits que deveriam ser lidos
            //Nesse caso, grava-se em um bitmap esses bits que sobraram, para que possam ser decodificados na pr�xima execu��o
            if(posicao >= qtdBitsASeremLidos)
            {
                for(j = j; j < 8; j++)
                {
                    unsigned char b;

                    b = texto[i]>>(7 - j);

                    bitmapAppendLeastSignificantBit(bmBitsExcedentes, b);

                    *qtdBitsExcedentes = *qtdBitsExcedentes - 1;
                }
                free(texto);
                return;
            }
            //Deixa o bit que se quer como o �ltimo bit do caractere
            temp = texto[i]>>(7 - j);

            bitmapAppendLeastSignificantBit(bm, temp);

            *qtdBitsExcedentes = *qtdBitsExcedentes - 1;

            bit = bitmapGetBit(*bm, posicao + posicaoInicialBm);

            if(bit == 0b00000000)
            {
                noAtual = AcessarAtributoEsquerda(noAtual);
            }
            else
            {
                noAtual = AcessarAtributoDireita(noAtual);
            }

            //Se o n� atual for uma folha, grava o caracter da folha em uma string
            if(AcessarTipo(noAtual) == 1)
            {
                textoDescompactado[*tamanhoAtualTexto] = AcessarCaracter(noAtual);
                *tamanhoAtualTexto = *tamanhoAtualTexto + 1;
                noAtual = arv;

                //Ao ler um novo caractere, encerra a fun��o caso a quantidade de caracteres j� lidos seja igual ao bitmap - 24
                //Nesse caso, grava-se em um bitmap esses bits que sobraram do �ltimo caractere lido, para que possam ser decodificados na pr�xima execu��o
                if(bitmapGetLength(*bm) >= (maxTamBitmap - 24) && (qtdBitsASeremLidos == maxTamBitmap))
                {
                    j++;
                    for(j = j; j < 8; j++)
                    {
                        unsigned char b;

                        b = texto[i]>>(7 - j);

                        bitmapAppendLeastSignificantBit(bmBitsExcedentes, b);

                        *qtdBitsExcedentes = *qtdBitsExcedentes - 1;
                    }
                    free(texto);
                    return;
                }
            }
        }
    }
    free(texto);
    return;
}


void DescompactaArquivo(FILE* arquivoCompactado, int maxTamBitmap, char nomeArquivoCompactado[])
{
    //Variavel para auxiliar na cria��o da arvore
    int posicaoAtual;
    //Extensao do arquivo
    char* extensao;
    short int qtdBitsArvore;
    //Texto j� decodificado
    unsigned char* texto;
    int tamanhoTexto;
    int tamanhoTotalNomeArquivo;
    //Quantidade de bits que sobraram da �ltima execu��o
    int qtdBitsExcedentes = 0;
    int tamanhoTotalTexto = 0;
    int tamanhoAtualTexto = 0;

    Arv* arv = CriaVazia();

    arv = CriaNo(0, NULL, NULL);

    bitmap bmArvore = bitmapInit(4096);

    //O primeiro n� sempre ser� um n� raiz, logo n�o � necess�rio ler o primeiro bit do bitmap da �rvore
    posicaoAtual = 1;

    extensao = LeExtensaoArquivo(arquivoCompactado);

    LeArvore(arquivoCompactado, &qtdBitsArvore, &bmArvore);

    CriaArvoreAPartirBits(arv, &bmArvore, &posicaoAtual, qtdBitsArvore);

    tamanhoTotalNomeArquivo = strlen(nomeArquivoCompactado) + strlen(extensao) + 1;

    char nomeCompletoArquivo[tamanhoTotalNomeArquivo];

    nomeCompletoArquivo[0] = '\0';

    //Concatena o nome do arquivo com sua extens�o
    strcat(nomeCompletoArquivo, nomeArquivoCompactado);

    strcat(nomeCompletoArquivo, extensao);

    //Ponteiro para o arquivo descompactado, onde ser� escrito o texto
    FILE* arquivoDescompactado = fopen(nomeCompletoArquivo, "wb");

    tamanhoTexto = LeTamanhoTexto(arquivoCompactado);

    printf("TAMANHO DO TEXTO COMPACTADO: %d bits\n", tamanhoTexto);

    tamanhoAtualTexto = 0;

    //Verifica se o texto � maior que o tamanho alocado para o bitmap. Caso seja, deve-se realizar multiplas repeti��es, respeitando o tamanho m�ximo
    //do bitmap, at� que todo o arquivo tenha sido decodificado
    if(tamanhoTexto <= maxTamBitmap)
    {
        texto = (unsigned char*)malloc(sizeof(unsigned char) * ((maxTamBitmap) + 2));

        bitmap bmTexto = bitmapInit(maxTamBitmap);
        bitmap bmBitsExcedentes = bitmapInit(64);

        LeTexto(arquivoCompactado, &bmTexto, tamanhoTexto, texto, arv, &tamanhoAtualTexto, maxTamBitmap, &qtdBitsExcedentes, 0, &bmBitsExcedentes);

        tamanhoTotalTexto += GravaTextoNoArquivoDescompactado(arquivoDescompactado, texto, tamanhoAtualTexto);

        free(texto);
        free(bitmapGetContents(bmTexto));
        free(bitmapGetContents(bmBitsExcedentes));
    }
    else
    {
        int i;
        int qtdRepeticoesParaEscreverNoArquivo;
        int qtdBitsRestantes = tamanhoTexto;

        //Se o tamanho do texto n�o for m�ltiplo do tamanho do bitmap, ser� necess�rio mais uma execu�ao (que ser� interrompida ap�s escrever os caracteres restantes)
        if(tamanhoTexto % maxTamBitmap == 0)
        {
            qtdRepeticoesParaEscreverNoArquivo = tamanhoTexto / maxTamBitmap;
        }
        else
        {
            qtdRepeticoesParaEscreverNoArquivo = (tamanhoTexto / maxTamBitmap) + 1;
        }

        //Bitmap que vai conter os bits que sobraram do �ltimo ciclo
        bitmap bmBitsExcedentes = bitmapInit(64);

        for(i = 0; i < qtdRepeticoesParaEscreverNoArquivo; i++)
        {
            texto = (unsigned char*)malloc(sizeof(unsigned char) * ((maxTamBitmap) + 2));
            //Bitmap com o texto compactado
            bitmap bmTexto = bitmapInit(maxTamBitmap);

            int posicaoInicialBm = bitmapGetLength(bmBitsExcedentes);

            //Se tiver restado algum bit na execu��o anterior, p�e eles no bitmap do texto
            if(posicaoInicialBm > 0)
            {
                int j;

                for(j = 0; j < posicaoInicialBm; j++)
                {
                    bitmapAppendLeastSignificantBit(&bmTexto, bitmapGetBit(bmBitsExcedentes, j));
                }
            }

            //Libera o bitmap contendo os bits excedentes do ciclo anterior e cria um novo
            free(bitmapGetContents(bmBitsExcedentes));
            bmBitsExcedentes = bitmapInit(64);

            //Se n�o houverem mais bits a serem lidos, interrompe o la�o de repeti��o
            if(qtdBitsRestantes == 0)
            {
                free(bitmapGetContents(bmTexto));
                free(texto);
                break;
            }

            //Se restarem mais bits para serem lidos do que o bitmap consegue guardar, l� somente a quantidade de bits suficientes para preencher o bitmap
            //Caso restem menos, l� todos os bits restantes
            if((int)qtdBitsRestantes <= maxTamBitmap)
            {
                //Le a quantidade de bits restantes
                LeTexto(arquivoCompactado, &bmTexto, qtdBitsRestantes, texto, arv, &tamanhoAtualTexto, maxTamBitmap, &qtdBitsExcedentes, posicaoInicialBm, &bmBitsExcedentes);

                tamanhoTotalTexto += GravaTextoNoArquivoDescompactado(arquivoDescompactado, texto, tamanhoAtualTexto);

                //Como a quantidade de bits a serem lidos � menor que o bitmap, s� ser� necess�rio ler mais algum bit caso tenha sobrado no momento de leitura
                //dos bits
                qtdBitsRestantes = qtdBitsExcedentes;

                if(qtdBitsExcedentes <= 0)
                {
                    free(bitmapGetContents(bmTexto));
                    free(texto);
                    break;
                }
            }
            else
            {
                //Le a quantidade m�xima de bits que o bitmap consegue guardar
                LeTexto(arquivoCompactado, &bmTexto, maxTamBitmap, texto, arv, &tamanhoAtualTexto, maxTamBitmap, &qtdBitsExcedentes, posicaoInicialBm, &bmBitsExcedentes);

                tamanhoTotalTexto += GravaTextoNoArquivoDescompactado(arquivoDescompactado, texto, tamanhoAtualTexto);

                //A quantidade restante vai ser subtra�da do tamanho do bitmap, que � a quantidade de bits que foram ordenados a serem lidos,
                //e vai ser somado dos bits que n�o foram lidos nessa fun��o de leitura
                qtdBitsRestantes = qtdBitsRestantes - maxTamBitmap + qtdBitsExcedentes;
            }

            //Se tiver chegado no �ltimo ciclo previsto e ainda restarem bits, come�a um novo ciclo
            if((i == (qtdRepeticoesParaEscreverNoArquivo - 1)) && qtdBitsRestantes > 0)
            {
                qtdRepeticoesParaEscreverNoArquivo++;
            }

            free(texto);
            free(bitmapGetContents(bmTexto));
        }

        free(bitmapGetContents(bmBitsExcedentes));
    }

    //Libera a mem�ria alocada restante
    free(bitmapGetContents(bmArvore));
    free(extensao);
    Libera(arv);
    fclose(arquivoDescompactado);

    printf("Tamanho total do texto: %d bytes\n", tamanhoTotalTexto);
}







