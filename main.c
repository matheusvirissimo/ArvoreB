#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>

#define MAX 10
#define t 2 // o "t" é dinâmico. Por enquanto só testes. "T" é um valor de controle para quantidades de chaves
typedef struct no{
    int n; // quantidade de chaves
    int chave[MAX]; // números contidos na chaves
    bool folha; // se é folha
    struct no *filho[MAX+1] // apontando para o próximo
} ArvB;

// Funções binárias
void escreverBinario(ArvB *arvore){
    FILE *file;
    file = fopen("arvore.dat", "wb");
    if(file == NULL){
        printf("O arquivo nao foi aberto ;(\n");
        return;
    }

    int quantidadeRegistros = fwrite(arvore, sizeof(ArvB), 0, file);
    printf("Foram escritos %d nos na arvore!\n", quantidadeRegistros);
    fclose(file);
}

void lerBinario(ArvB *arvore){
    FILE *file;
    file = fopen("teste.dat", "rb");
    if(file == NULL){
        printf("O arquivo nao foi aberto ;(\n");
        return;
    }

    // mostrar o tamanho total do arquivo
    fseek(file, 0, SEEK_END);
    int tamArquivo = ftell(file)/sizeof(ArvB); 
    rewind(file); // volta o ponteiro pro inicio

    int quantidadeRegistrosLidos;
    quantidadeRegistrosLidos = fread(arvore, sizeof(ArvB), tamArquivo, file);
    printf("\nForam lidos %d nons na arvore\n", quantidadeRegistrosLidos);
    fclose(file);
    return; 
}
// Funções da árvore B


// criar nó na árvore
ArvB* criarNoArvoreB(ArvB** raiz){
    ArvB* x = (ArvB*) malloc(sizeof(ArvB)); // aloca no 
    x->folha = true; // o nó é folha
    x->n = 0; // inicia sem nenhuma chave
    escrever(x); // escreve no disco binário
    *raiz = x; // o novo nó vai ser a raiz
    return x; // retorna o novo nó
}


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
void splitChildrenArvoreB(ArvB* arvore, int i){ // recebe como parâmetro o nó a ser dividido e em qual chave isso irá acontecer
    ArvB* z = (ArvB*) malloc(sizeof(ArvB)); // aloca memória para o nó z;
    // ArvB* y = (ArvB*) malloc(sizeof(ArvB)); não precisa dar malloc no "z" porque dá vazamento de memória se fizer isso

    ArvB* y = arvore->filho[i]; // y vai ser o i-ésimo filho desse nó, ou seja, o nó que será dividido
    z->folha = y->folha; // z vai receber de y se ele é folha (ou não)

    z->n = t - 1; // DEFINE A QUANTIDADE DE CHAVES PRESENTES NO NÓ. z terá t-1 chaves (metade das chaves de y)

    for(int j = 0; j <= t - 1; j++){ // copia as últimas t-1 chaves de y para z (agora os novos nós possuem a mesma quantidade de chaves)
        z->chave[j] = y->chave[j+t]; // atribui as últimas chaves de "y" a "z", separnado o nó "y"
    }

    if(y->folha == false){ // se "y" não for folha, ele é um nó interno
        for(int j = 0; j <= t; j++){ // copia os filhos de "y" a partir de j+t para "z"
            z->filho[j] = y->filho[j+t]; // atribui os filhos correspondentes de "y" para "z"
        }
    }

    y->n = t-1; // reduz o número de chaves, pois elas já foram divididas entre "y" e "z"
    
    // Agora ajusta os ponteiros do filho do nó pai
    for(int j = (arvore->n + 1); j >= i + 1; j--){ // move os ponteiros dos filhos da árvore uma posição a direita, pois deve abrir-se espaço para o novo nó z
        arvore->filho[j+1] = arvore->filho[j]; // reajusta os filhos da árvore movendo para a direita
    }

    arvore->filho[i+1] = z; // coloca como o filho i+1 da árvore, ou seja, é o filho da direita

    // fazer a movimentação das chaves para incluir a mediana do nó cheio no nó pai (árvore)
    for(int j = arvore->n; j >= i; j--){ // faz a movimentação chaves uma posição à direita, abrindo espaço para a nova chave
        arvore->chave[j+1] = arvore->chave[j]; // a próxima chave recebe a anterior, fazendo deslocamento
    }

    arvore->chave[i] = y->chave[t]; // a mediana das chaves sobe para o nó-pai
    arvore->n = arvore->n + 1; // o número de chaves em x aumenta em 1 (inclusão da chave do filho que foi pro pai)

    escrever(arvore);
    escrever(y);
    escrever(z);

}

// Fazendo inserção não cheia na árvore B
void insereNaoCheioArvoreB(ArvB* arvore, int k){
    int i = arvore->n; // o 'i' será igual a quantidade de chaves presentes na chave

    if(arvore->folha){ // se o nó for folha
        while(i >= 0 && k < arvore->chave[i]){ // enquanto não chegar na última chave e o número a ser inserido for menor que o buscado
            arvore->chave[i+1] = arvore->chave[i]; // faz o deslocamento das chaves para a direita
            i = i - 1; // estamos percorrendo as chaves de trás para frente
        }
        
        arvore->chave[i+1] = k; // insere a nova chave
        arvore->n = arvore->n+1; // estamos aumentando a quantidade de chaves presentes no nó;
        escrita(arvore); // função em binário para escrever em binário a árvore atual
    }else{ // se o nó NÃO for folha
        while(i >= 0 && k < arvore->chave[i]){ // enquanto não chegar no começo e o número inserido for menor que a chave atual
            i--;

        }

        i++; // corrige o valor do "i" após a iteração
        leitura(arvore->filho[i]); // função para ler, em binário, o filho do nó

        if(arvore->filho[i]->n == 2*t-1){ // se a quantidade de chaves do nó for igual a 2t-1, ou seja, verifica se o nó filho está cheio
            splitChildrenArvoreB(arvore, i); // se está cheio, faz a divisão do nó (split)
            
            if(k > arvore->chave[i]){
                i++; // se a chave inserida for maior que a chave do nó pai, faz o ajuste do "i" para apontar para o filho correto
            }

            insereNaoCheioArvoreB(arvore->filho[i], k); // faz recursivamente a inserção no nó filho até que ele não esteja mais cheio
        }

    }

    return;
}

void insereArvoreB(ArvB* arvore, int k){ // k é a chave a ser inserida
    if(arvore->n == 2*t-1){ // se o nó já estiver com a quantidade máxima de chaves permitidas
        ArvB* s = (ArvB*) malloc(sizeof(ArvB));
        s->folha = false; // o novo nó vai ser uma um nó raiz
        s->n = 0; // inicializa sem nenhuma chave
        s->filho[0] = arvore; // o filho de "s" vai ser a própria árvore original
        splitChildrenArvoreB(s,0); // faz a divisão do nó raiz para poder inserir futuramente
        insereNaoCheioArvoreB(s, k); // insere a chave k no novo nó
    }else{
        insereNaoCheioArvoreB(arvore, k); // caso esteja vazio, insere direto na folha
    }

}