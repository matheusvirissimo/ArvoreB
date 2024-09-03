#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>

#define MAX 10
#define t 2 // o "t" é dinâmico. Por enquanto só testes

typedef struct no{
    int n; // quantidade de chaves
    int chave[MAX]; // números contidos na chaves
    bool folha; // se é folha
    struct no *filho[MAX+1] // apontando para o próximo
} ArvB;



// Fazendo a busca normal
ArvB* buscaArvoreB(ArvB* arvore, int numeroBuscado){ // k é a chave que buscamos. O tipo é ArvB porque vamos retornar o nó
    int i = 0; // está igual a 1, mas é 0

    while(i <= arvore->n && numeroBuscado > arvore->chave[i]){ 
        // enquanto "i" não for maior que a quantidade de elementos e o número buscado não for maior que o número analisado (está ordenado)
        i++;
    }

    if(i <= arvore->n && numeroBuscado == arvore->chave[i]){
        // se o numeroBuscado for igual o da chave
        return arvore->chave[i]; // retornamos o valor da chave, pois encontramos o valor
    }

    if(arvore->folha == true){ // chegamos ao fim da árvore
        return NULL; // não encontramos o valor
    }else{ // não encontramos nesse nível ainda
        return buscaArvoreB(arvore->filho[i], numeroBuscado); // chama recursivamente o próximo nó
    }

}

// split de um nó;

void splitChildrenArvoreB(ArvB* arvore, int i){

}

// Fazendo inserção não cheia na árvore B
void insereNaoCheioArvoreB(ArvB* arvore, int k){
    int i = arvore->n; // o 'i' será igual a quantidade de chaves presentes na chave

    if(arvore->folha){ // se o nó for folha
        while(i >= 0 && k < arvore->chave[i]){ // enquanto não chegar na última chave e o número a ser inserido for menor que o buscado
            arvore->chave[i+1] = arvore->chave[i]; // a próxima chave vai receber a atual
            i = i - 1; // estamos percorrendo as chaves de trás para frente

        }
            arvore->chave[i+1] = k; // o valor a ser inserido vai sendo copiado na próxima chave
            arvore->n = arvore->n+1; // estamos aumentando a quantidade de chaves presentes no nó;
            escrita(arvore); // função em binário para escrever em binário a árvore atual
    }else{ // se o nó NÃO for folha
        while(i >= 0 && k < arvore->chave[i]){ // enquanto não chegar no começo e o número inserido for menor que a chave atual
            i--;

        }
        i++;
        leitura(arvore->filho[i]); // função para ler, em binário, o filho do nó
        if(arvore->filho[i]->n == 2*t-1){ // se a quantidade de chaves do nó for igual a 2t-1, ou seja, o máximo permitido
            splitChildArvoreB(arvore, i); // faz um split do nó
            if(k > arvore->chave[i]){
                i++;
            }

            insereNaoCheioArvoreB(arvore->filho[i], k); // vai para o próximo nó na inserção
        }

    }

    return;
}