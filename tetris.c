#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// --------------------------------------------------------
// Estrutura de uma peça
// --------------------------------------------------------
typedef struct {
    char nome;
    int id;
} Peca;

int contadorID = 0; // IDs crescentes

// --------------------------------------------------------
// Gera uma peça automaticamente
// --------------------------------------------------------
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.nome = tipos[rand() % 4];
    p.id = contadorID++;
    return p;
}

// --------------------------------------------------------
// ENQUEUE — insere peça no fim da fila (circular)
// --------------------------------------------------------
int enqueue(Peca fila[], int *fim, int *total, Peca nova) {
    if (*total == TAM_FILA)
        return 0;

    fila[*fim] = nova;
    *fim = (*fim + 1) % TAM_FILA;
    (*total)++;
    return 1;
}

// --------------------------------------------------------
// DEQUEUE — remove peça da frente da fila
// --------------------------------------------------------
Peca dequeue(Peca fila[], int *inicio, int *total, int *ok) {
    Peca removida = {'X', -1};
    *ok = 0;

    if (*total == 0)
        return removida;

    removida = fila[*inicio];
    *inicio = (*inicio + 1) % TAM_FILA;
    (*total)--;
    *ok = 1;

    return removida;
}

// --------------------------------------------------------
// PUSH — coloca peça no topo da pilha
// --------------------------------------------------------
int push(Peca pilha[], int *topo, Peca p) {
    if (*topo == TAM_PILHA)
        return 0;

    pilha[*topo] = p;
    (*topo)++;
    return 1;
}

// --------------------------------------------------------
// POP — remove peça do topo da pilha
// --------------------------------------------------------
Peca pop(Peca pilha[], int *topo, int *ok) {
    Peca removida = {'X', -1};
    *ok = 0;

    if (*topo == 0)
        return removida;

    (*topo)--;
    removida = pilha[*topo];
    *ok = 1;

    return removida;
}

// --------------------------------------------------------
// Exibe estado atual da fila e da pilha
// --------------------------------------------------------
void exibirEstado(Peca fila[], int inicio, int total,
                  Peca pilha[], int topo) {

    printf("\n=========================================\n");
    printf("            ESTADO ATUAL\n");
    printf("=========================================\n");

    // Fila
    printf("Fila: ");
    int pos = inicio;
    for (int i = 0; i < total; i++) {
        printf("[%c %d] ", fila[pos].nome, fila[pos].id);
        pos = (pos + 1) % TAM_FILA;
    }
    printf("\n");

    // Pilha
    printf("Pilha reserva (Topo -> Base): ");
    if (topo == 0) {
        printf("(vazia)");
    } else {
        for (int i = topo - 1; i >= 0; i--)
            printf("[%c %d] ", pilha[i].nome, pilha[i].id);
    }
    printf("\n=========================================\n");
}

// --------------------------------------------------------
// Troca a peça da FRENTE da fila com o TOPO da pilha
// --------------------------------------------------------
void trocarPeçaAtual(Peca fila[], int inicio, int totalFila,
                     Peca pilha[], int topoPilha) {

    if (totalFila == 0 || topoPilha == 0) {
        printf("\nNão é possível trocar: falta peça.\n");
        return;
    }

    Peca temp = fila[inicio];
    fila[inicio] = pilha[topoPilha - 1];
    pilha[topoPilha - 1] = temp;

    printf("\nTroca concluída (topo da pilha ↔ frente da fila).\n");
}

// --------------------------------------------------------
// Troca múltipla (3 peças da fila ↔ 3 peças da pilha)
// --------------------------------------------------------
void trocaMultipla(Peca fila[], int inicio, int totalFila,
                   Peca pilha[], int topoPilha) {

    if (totalFila < 3 || topoPilha < 3) {
        printf("\nTroca múltipla NÃO realizada: quantidade insuficiente.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int posFila = (inicio + i) % TAM_FILA;

        Peca temp = fila[posFila];
        fila[posFila] = pilha[topoPilha - 1 - i];
        pilha[topoPilha - 1 - i] = temp;
    }

    printf("\nTroca múltipla concluída (3×3).\n");
}

// --------------------------------------------------------
// PROGRAMA PRINCIPAL
// --------------------------------------------------------
int main() {
    srand(time(NULL));

    Peca fila[TAM_FILA];
    Peca pilha[TAM_PILHA];

    int inicio = 0, fim = 0, totalFila = 0;
    int topo = 0;

    // Preencher fila inicial
    for (int i = 0; i < TAM_FILA; i++)
        enqueue(fila, &fim, &totalFila, gerarPeca());

    int opcao;

    do {
        exibirEstado(fila, inicio, totalFila, pilha, topo);

        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Trocar peça atual (fila ↔ pilha)\n");
        printf("5 - Troca múltipla 3×3\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        int ok;

        switch (opcao) {

        case 1: {
            Peca jogada = dequeue(fila, &inicio, &totalFila, &ok);
            if (ok)
                printf("\nPeça jogada: [%c %d]\n", jogada.nome, jogada.id);

            enqueue(fila, &fim, &totalFila, gerarPeca());
            break;
        }

        case 2: {
            Peca p = dequeue(fila, &inicio, &totalFila, &ok);

            if (ok) {
                if (push(pilha, &topo, p))
                    printf("\nPeça [%c %d] reservada.\n", p.nome, p.id);
                else
                    printf("\nPilha cheia! Não é possível reservar.\n");
            }

            enqueue(fila, &fim, &totalFila, gerarPeca());
            break;
        }

        case 3: {
            Peca usada = pop(pilha, &topo, &ok);
            if (ok)
                printf("\nPeça usada: [%c %d]\n", usada.nome, usada.id);

            enqueue(fila, &fim, &totalFila, gerarPeca());
            break;
        }

        case 4:
            trocarPeçaAtual(fila, inicio, totalFila, pilha, topo);
            break;

        case 5:
            trocaMultipla(fila, inicio, totalFila, pilha, topo);
            break;

        case 0:
            printf("\nEncerrando...\n");
            break;

        default:
            printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
