#ifndef ARVORE_H_INCLUDED
#define ARVORE_H_INCLUDED

typedef struct arvore Arv;

int RetornaPeso(Arv *a);

Arv* CriaVazia(void);

/*cria um nó raiz e preenche seus atributos
* inputs: o peso desse nó, as sub-arvores da esquerda e da direita
* output: o endereço do nó criado
*/
Arv* CriaNo(int peso, Arv* esq, Arv* dir);

/*cria um nó folha, que representa um caractere
* inputs: o peso dessa folha e o caractere que ele representa
* output: o endereço do nó raiz criado
*/
Arv* CriaFolha(int peso, unsigned char c);

/*Imprime a árvore em notação InOrder (esq, raiz, direita)
* inputs: o endereço da raiz da árvore
* output: nenhum
*/

//void Conta(Arv* arv);
void ImprimeArvCodificada(Arv* arv);

void Imprime(Arv* arv);

/* Libera memória alocada pela estrutura da árvore
as sub-árvores devem ser liberadas antes de se liberar o nó raiz
* inputs: o endereço da raiz da árvore
* output: uma árvore vazia, representada por NULL
*/
Arv* Libera (Arv* arv);

/* Retorna o número de folhas da árvore
* inputs: o endereço da raiz da árvore
* output: o número de folhas
*/
int Folhas(Arv* arv);

/* Retorna a altura da árvore
* inputs: o endereço da raiz da árvore
* output: a altura da árvore
*/
int Altura(Arv* arv);

/*
* Retorna o nó esquerdo do nó passado como parâmetro
* input: o nó
* output: o nó esquerdo do nó passado
* pre-condicao: o nó está inicializado
* pos-condicao: tem-se o endereco do nó esquerdo do nó passado
*/
Arv* AcessarAtributoEsquerda(Arv* arv);

/*
* Retorna o nó direito do nó passado como parâmetro
* input: o nó
* output: o nó direito do nó passado
* pre-condicao: o nó está inicializado
* pos-condicao: tem-se o endereco do nó direito do nó passado
*/
Arv* AcessarAtributoDireita(Arv* arv);

/*
* Retorna o tipo do nó (0 se for raiz e 1 se for folha)
* input: o nó
* output: o tipo do nó
* pre-condicao: o nó está inicializado
* pos-condicao: tem-se o tipo do nó passado
*/
int AcessarTipo(Arv* arv);

/*
* Retorna o caractere do nó folha passado
* input: o nó folha
* output: o caractere gravado no nó folha
* pre-condicao: o nó está inicializado e é um nó folha
* pos-condicao: tem-se o caractere guardado no nó folha passado
*/
unsigned char AcessarCaracter(Arv* arv);

/*
* Faz o atributo 'esquerda' de um nó passado, apontar para um outro nó, também passado
* input: o nó que se quer modificar o atributo e o nó para onde esse atributo vai apontar
* output: nenhum
* pre-condicao: o nó para onde vai apontar deve estar inicializado;
* pos-condicao: o atributo 'esquerda' do nó passado, aponta para o outro nó passado
*/
void GravarAtributoEsquerda(Arv* arv, Arv* esquerda);

/*
* Faz o atributo 'direita' de um nó passado, apontar para um outro nó, também passado
* input: o nó que se quer modificar o atributo e o nó para onde esse atributo vai apontar
* output: nenhum
* pre-condicao: o nó para onde vai apontar deve estar inicializado;
* pos-condicao: o atributo 'direita' do nó passado, aponta para o outro nó passado
*/
void GravarAtributoDireita(Arv* arv, Arv* direita);


#endif // ARVORE_H_INCLUDED
