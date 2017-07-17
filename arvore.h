#ifndef ARVORE_H_INCLUDED
#define ARVORE_H_INCLUDED

typedef struct arvore Arv;

int RetornaPeso(Arv *a);

Arv* CriaVazia(void);

/*cria um n� raiz e preenche seus atributos
* inputs: o peso desse n�, as sub-arvores da esquerda e da direita
* output: o endere�o do n� criado
*/
Arv* CriaNo(int peso, Arv* esq, Arv* dir);

/*cria um n� folha, que representa um caractere
* inputs: o peso dessa folha e o caractere que ele representa
* output: o endere�o do n� raiz criado
*/
Arv* CriaFolha(int peso, unsigned char c);

/*Imprime a �rvore em nota��o InOrder (esq, raiz, direita)
* inputs: o endere�o da raiz da �rvore
* output: nenhum
*/

//void Conta(Arv* arv);
void ImprimeArvCodificada(Arv* arv);

void Imprime(Arv* arv);

/* Libera mem�ria alocada pela estrutura da �rvore
as sub-�rvores devem ser liberadas antes de se liberar o n� raiz
* inputs: o endere�o da raiz da �rvore
* output: uma �rvore vazia, representada por NULL
*/
Arv* Libera (Arv* arv);

/* Retorna o n�mero de folhas da �rvore
* inputs: o endere�o da raiz da �rvore
* output: o n�mero de folhas
*/
int Folhas(Arv* arv);

/* Retorna a altura da �rvore
* inputs: o endere�o da raiz da �rvore
* output: a altura da �rvore
*/
int Altura(Arv* arv);

/*
* Retorna o n� esquerdo do n� passado como par�metro
* input: o n�
* output: o n� esquerdo do n� passado
* pre-condicao: o n� est� inicializado
* pos-condicao: tem-se o endereco do n� esquerdo do n� passado
*/
Arv* AcessarAtributoEsquerda(Arv* arv);

/*
* Retorna o n� direito do n� passado como par�metro
* input: o n�
* output: o n� direito do n� passado
* pre-condicao: o n� est� inicializado
* pos-condicao: tem-se o endereco do n� direito do n� passado
*/
Arv* AcessarAtributoDireita(Arv* arv);

/*
* Retorna o tipo do n� (0 se for raiz e 1 se for folha)
* input: o n�
* output: o tipo do n�
* pre-condicao: o n� est� inicializado
* pos-condicao: tem-se o tipo do n� passado
*/
int AcessarTipo(Arv* arv);

/*
* Retorna o caractere do n� folha passado
* input: o n� folha
* output: o caractere gravado no n� folha
* pre-condicao: o n� est� inicializado e � um n� folha
* pos-condicao: tem-se o caractere guardado no n� folha passado
*/
unsigned char AcessarCaracter(Arv* arv);

/*
* Faz o atributo 'esquerda' de um n� passado, apontar para um outro n�, tamb�m passado
* input: o n� que se quer modificar o atributo e o n� para onde esse atributo vai apontar
* output: nenhum
* pre-condicao: o n� para onde vai apontar deve estar inicializado;
* pos-condicao: o atributo 'esquerda' do n� passado, aponta para o outro n� passado
*/
void GravarAtributoEsquerda(Arv* arv, Arv* esquerda);

/*
* Faz o atributo 'direita' de um n� passado, apontar para um outro n�, tamb�m passado
* input: o n� que se quer modificar o atributo e o n� para onde esse atributo vai apontar
* output: nenhum
* pre-condicao: o n� para onde vai apontar deve estar inicializado;
* pos-condicao: o atributo 'direita' do n� passado, aponta para o outro n� passado
*/
void GravarAtributoDireita(Arv* arv, Arv* direita);


#endif // ARVORE_H_INCLUDED
