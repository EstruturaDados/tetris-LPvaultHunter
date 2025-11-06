

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ------------------ ESTRUTURA DA PECA ------------------
typedef struct {
    char nome;
    int id;
} Peca;

// LPvaultHUnter//
// ------------------ FILA CIRCULAR ------------------
typedef struct {
    Peca itens[TAM_FILA];
    int inicio, fim, quantidade;
} Fila;

// ------------------ PILHA ------------------
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// ------------------ FUNCOES AUXILIARES ------------------
char gerarTipoPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    return tipos[rand() % 4];
}

Peca gerarPeca(int *contador) {
    Peca nova;
    nova.nome = gerarTipoPeca();
    nova.id = (*contador)++;
    return nova;
}

// ------------------ OPERACOES DA FILA ------------------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

int filaCheia(Fila *f) {
    return f->quantidade == TAM_FILA;
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->quantidade++;
}

Peca desenfileirar(Fila *f) {
    Peca p = {'-', -1};
    if (filaVazia(f)) return p;
    p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
    return p;
}

// ------------------ OPERACOES DA PILHA ------------------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

void empilhar(Pilha *p, Peca item) {
    if (pilhaCheia(p)) return;
    p->itens[++p->topo] = item;
}

Peca desempilhar(Pilha *p) {
    Peca vazia = {'-', -1};
    if (pilhaVazia(p)) return vazia;
    return p->itens[p->topo--];
}

// ------------------ EXIBICAO ------------------
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n========================================\n");
    printf("Estado atual:\n\n");

    printf("Fila de pecas\t");
    int idx = f->inicio;
    for (int i = 0; i < f->quantidade; i++) {
        Peca atual = f->itens[idx];
        printf("[%c %d] ", atual.nome, atual.id);
        idx = (idx + 1) % TAM_FILA;
    }
    printf("\n");

    printf("Pilha de reserva (Topo -> base):\t");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
    }
    printf("\n========================================\n");
}

// ------------------ TROCAS ------------------
void trocarPecaAtual(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\nNao ha pecas suficientes para troca.\n");
        return;
    }

    int posFila = f->inicio;
    Peca temp = f->itens[posFila];
    f->itens[posFila] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("\nAcao: Peca da frente trocada com o topo da pilha.\n");
}

void trocarMultiplas(Fila *f, Pilha *p) {
    if (f->quantidade < 3 || p->topo < 2) {
        printf("\nNao ha pecas suficientes para a troca multipla.\n");
        return;
    }

    int idxFila = f->inicio;
    for (int i = 0; i < 3; i++) {
        Peca temp = f->itens[(idxFila + i) % TAM_FILA];
        f->itens[(idxFila + i) % TAM_FILA] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }

    printf("\nAcao: troca realizada entre os 3 primeiros da fila e as 3 da pilha.\n");
}

// ------------------ PROGRAMA PRINCIPAL ------------------
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int contador = 0;

    // Inicializa a fila cheia
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(&contador));
    }

    int opcao;
    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpcoes disponiveis:\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para a pilha de reserva\n");
        printf("3 - Usar peca da pilha de reserva\n");
        printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar peca
                if (!filaVazia(&fila)) {
                    Peca usada = desenfileirar(&fila);
                    printf("\nPeca [%c %d] jogada.\n", usada.nome, usada.id);
                    enfileirar(&fila, gerarPeca(&contador));
                } else {
                    printf("\nFila vazia.\n");
                }
                break;
            }

            case 2: { // Reservar peca
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("\nPeca [%c %d] movida para a pilha.\n", reservada.nome, reservada.id);
                    enfileirar(&fila, gerarPeca(&contador));
                } else {
                    printf("\nOperacao invalida (fila vazia ou pilha cheia).\n");
                }
                break;
            }

            case 3: { // Usar peca da pilha
                if (!pilhaVazia(&pilha)) {
                    Peca usada = desempilhar(&pilha);
                    printf("\nPeca [%c %d] usada da pilha.\n", usada.nome, usada.id);
                } else {
                    printf("\nPilha vazia.\n");
                }
                break;
            }

            case 4:
                trocarPecaAtual(&fila, &pilha);
                break;

            case 5:
                trocarMultiplas(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando o programa...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}

/* LPvaultHUnter/*