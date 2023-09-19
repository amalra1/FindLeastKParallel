#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// (c) by W.Zola set/23
//
// EXAMPLE Sequential MAX-Heap,
//                    decreaseMax operation
//                    and others

// para compilar:
// gcc -O3 max-heap.c -o max-heap -lm

#define SHOW_DECREASE_MAX_STEPS 1
#define MAX_HEAP_SIZE (1024 * 1024)
#define RAND_RANGE 10  // Só pra teste, no trabalho vai ter que deletar essa pra fazer com o RAND_MAX

typedef struct
{
    float chave;
    int valor;
} par_t;

void drawHeapTree(par_t heap[], int size, int nLevels) // FIX ME!
{
    int offset = 0;
    int space = (int)pow(2, nLevels - 1);
    // int space = 0;

    int nElements = 1;
    for (int level = 0; level < nLevels; level++)
    {
        // print all elements in this level
        for (int i = offset; i < size && i < (offset + nElements); i++)
            printf("[chave: %.0f, índice: %d]", heap[i].chave, heap[i].valor);
        printf("\n");

        offset += nElements;
        space = nElements - 1;
        nElements *= 2;
    }
}

void swap(par_t *a, par_t *b) //__attribute__((always_inline));
{
    par_t temp = *a;
    *a = *b;
    *b = temp;
}

void maxHeapify(par_t heap[], int tam, int i)
{
    while (1)
    {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < tam && heap[left].chave > heap[largest].chave)
            largest = left;

        if (right < tam && heap[right].chave > heap[largest].chave)
            largest = right;

        if (largest != i)
        {
            swap(&heap[i], &heap[largest]); // Using the swap function
            i = largest;
        }
        else
            break;
    }
}

// #define parent(pos) ( pos/2 ) // SE nao usar posicao 0
#define parent(pos) ((pos - 1) / 2)

void heapifyUp(par_t heap[], int *tam, int pos)
{
    int chave = heap[pos].chave;
    int valor = heap[pos].valor;

    while (pos > 0 && chave > heap[parent(pos)].chave)
    {
        heap[pos].chave = heap[parent(pos)].chave;
        heap[pos].valor = heap[parent(pos)].valor;
        pos = parent(pos);
    }

    heap[pos].chave = chave;
    heap[pos].valor = valor;
}

void insert(par_t heap[], int *tam, float chave, int valor)
{
    *tam += 1;
    int last = *tam - 1;

    heap[last].chave = chave;
    heap[last].valor = valor;
    printf("\n%1f\n", heap[last].chave);
    heapifyUp(heap, tam, last);
}

int isMaxHeap(par_t heap[], int tam)
{
    for (int i = 1; i < tam; i++)
        if (heap[i].chave <= heap[parent(i)].chave)
            continue;
        else
        {
            printf("\nbroke at [%d]=%f\n", i, heap[i].chave);
            printf("father at [%d]=%f\n", parent(i), heap[parent(i)].chave);
            return 0;
        }
    return 1;
}

void decreaseMax(par_t heap[], int tam, float chave, int valor)
{
    if (tam == 0) // Heap is empty
        return;

    if (heap[0].chave > chave)
    {
        heap[0].chave = chave;
        heap[0].valor = valor;

        #if SHOW_DECREASE_MAX_STEPS
            // drawHeapTree( heap, tam, 4 );
            printf("    ~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        #endif

        maxHeapify(heap, tam, 0);
    }
}

void geraNaleatorios(float v[], int n)
{
    for(int i = 0; i < n; i++)
    {
        // Gera dois aleatorios entre 0 e RAND_MAX
	    int a = rand() % RAND_RANGE;  
	    int b = rand() % RAND_RANGE;  
	
        // Junta esses dois e forma um numero para o vetor v
	    float elem = a * 100.0 + b;

        // inserir o valor v na posição p
	    v[i] = elem;
    }
}

int main(int argc, char* argv[])
{
    int n = atoi(argv[1]);  // Linha de comando, quantidade de nums do vetor v
    int nThreads = atoi(argv[3]);  // Linha de comando
    int heapSize = 0;  // Heap vazia
    float v[n]; // = {40, 10, 30, 70, 50, 20, 4, 5, 44, 40, 55, 50}; // GERADO ALEATÓRIO                         // ENTRADA
    int k = atoi(argv[2]);                                                // ENTRADA
    par_t heap[k];                                            // heap S

    // Randomiza a SEED
    srand(time(NULL));

    // Cria o vetor v
    geraNaleatorios(v, n);

    // Printa o vetor v
    for (int i = 0; i < n; i++)
        printf("%0.f ", v[i]);
    printf("\n");

    // // PARTE 1 - Insere todos os valores de v até k na heap S
    // for (int i = 0; i < k; i++)
    // {
    //     printf("insere %d\n", v[i]);
    //     insert(heap, &heapSize, v[i], i);
    //     printf("------Max-Heap Tree------ ");
    //     if (isMaxHeap(heap, heapSize))
    //         printf("é heap!\n");
    //     else
    //         printf("não é heap!\n");

    //     #ifndef SHOW_DECREASE_MAX_STEPS
    //             drawHeapTree(heap, heapSize, 4);
    //     #endif
    // }

    // #ifdef SHOW_DECREASE_MAX_STEPS
    //     drawHeapTree(heap, heapSize, 4);
    // #endif

    // // PARTE 2 - Agora checa o resto do vetor para ver se tem menores
    // for (int i = k; i < n; i++)
    //     decreaseMax(heap, heapSize, v[i], i);

    // #ifdef SHOW_DECREASE_MAX_STEPS
    //     drawHeapTree(heap, heapSize, 4);
    // #endif
    return 0;
}
