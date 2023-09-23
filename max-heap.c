#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define THREAD_MIN 1
#define THREAD_MAX 8
#define SHOW_DECREASE_MAX_STEPS 1
#define MAX_HEAP_SIZE (1024 * 1024)
#define RAND_RANGE 10  // Só pra teste, no trabalho vai ter que deletar essa pra fazer com o RAND_MAX

typedef struct
{
    float chave;
    int valor;
} par_t;

int n, k, nThreads;
float *input;
par_t *output;
par_t **heaps;
int* tamHeaps;

void drawHeapTree(par_t heap[], int size, int nLevels) // FIX ME!
{
    int offset = 0;
    int space = (int)pow(2, nLevels - 1);

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

void swap(par_t *a, par_t *b)
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

    printf("Inserido --> %1f\n", heap[last].chave);

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
            printf("    ~~~~~~~~~~~~~~~~~~~~~~~~~DECREASE MAX!\n");
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

void *acharKMenores(void *arg) 
{
    int threadNum = (int)arg;
    int indexPrimElem = threadNum * (n/nThreads);  // Cada thread vai ter n/nthreads elementos
    int indexUltElem;                              // A multiplicação é pra ver em qual dos blocos ela está

    // Checa se é a última thread
    if (threadNum == nThreads - 1)
        indexUltElem = n - 1;
    else
    {
        if (((threadNum + 1) * (n/nThreads)) < n)
            indexUltElem = (threadNum + 1) * (n/nThreads) - 1;
        else
            indexUltElem = n - 1;
    }

    // // PARTE 1 - Insere todos os valores de input até k na heap S
    // for (int i = indexPrimElem; i < k; i++)
    //     insert(heaps[threadNum], &tamHeaps[i], input[i], i);

    // // PARTE 2 - Agora checa o resto do vetor para ver se tem menores
    // for (int i = k; i < indexUltElem; i++)
    //     decreaseMax(heaps[threadNum], tamHeaps[i], input[i], i);

    for (int i = indexPrimElem; i <= indexUltElem; i++)
    {
        if (tamHeaps[threadNum] < k)
            insert(heaps[threadNum], &tamHeaps[threadNum], input[i], i);
        else
            decreaseMax(heaps[threadNum], tamHeaps[threadNum], input[i], i);            
    }

    printf("Thread %d tem a heap:\n", threadNum);
    drawHeapTree(heaps[threadNum], tamHeaps[threadNum], k);
}

int main(int argc, char* argv[])
{                        
    clock_t startTime, endTime;
    double elapsedTime;
    int num;

    // Tratando a entrada
    if (argc != 4) 
    {
        printf("Forma de execução: %s <Quantidade de números aleatórios> <Quantidade de K menores> <Quantidade de threads>\n" , argv[0]); 
        exit(EXIT_FAILURE);
    } 
    else 
    {
        nThreads = atoi(argv[3]);

        if (nThreads < THREAD_MIN || nThreads > THREAD_MAX) 
        {
            printf("Forma de execução: %s <Quantidade de números aleatórios> <Quantidade de K menores> <Quantidade de threads>\n" , argv[0]); 
            printf("<Quantidade de threads> tem que ser entre %d e %d\n", THREAD_MIN, THREAD_MAX);
            exit(EXIT_FAILURE);
        }  

        n = atoi(argv[1]);
        k = atoi(argv[2]);
        
        if (k < 0 || k > n) 
        {
            printf("Forma de execução: %s <Quantidade de números aleatórios> <Quantidade de K menores> <Quantidade de threads>\n" , argv[0]); 
            printf("<Quantidade de K menores> tem que ser entre 1 e <Quantidade de números aleatórios>\n");
            exit(EXIT_FAILURE);
        }       
    }

    // Aloca espaços
    pthread_t threads[nThreads];
    input = malloc(n * sizeof(par_t));
    heaps = malloc(nThreads * sizeof(par_t *));
    output = malloc(k * sizeof(par_t));
    tamHeaps = malloc(nThreads * sizeof(int));

    // Randomiza a SEED
    srand(time(NULL));

    // Cria o vetor v
    geraNaleatorios(input, n);

    // Aloca espaço para cada heap
    for (int i = 0; i < nThreads; i++)
    {
        tamHeaps[i] = 0;
        heaps[i] = malloc(k * sizeof(par_t));
    }

    // Printa vetor de aleatórios
    printf("Vetor de aleatórios:\n");
    for (int i = 0; i < n; i++)
        printf("%0.f ", input[i]);
    printf("\n");

    // Pega o tempo inicial de exec do algoritmo
    startTime = clock(); 
    
    // Cria as threads
    num = 0;
    for (int i = 0; i < nThreads; i++)
    {
        // Testa se criou as threads
        if (pthread_create(&threads[i], NULL, acharKMenores, (void*)num) != 0)
        {
            perror("Erro ao criar threads");
            exit(EXIT_FAILURE);
        }

        num++; 
    }   

    // Aguarda as threads terminarem a execução
    for (int i = 0; i < nThreads; i++)
    {
        // Testa se chegaram ao fim
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Erro ao aguardas as threads\n");
            exit(EXIT_FAILURE);
        }
    }

    // Printa K menores
    // printf("\nK menores:\n");
    // for (int i = 0; i < k; i++)
    //     printf("%0.f ", output[i].chave);
    // printf("\n");

    // Pega o tempo final de exec do algoritmo
    endTime = clock();

    // Printando o tempo que levou
    elapsedTime = ((double)(endTime - startTime)) / CLOCKS_PER_SEC * 1000.0;
    printf("\nO algoritmo demorou: \n%.3f milissegundos para executar.\nE a vazão foi de %.3f MOPS\n", elapsedTime, (n/elapsedTime)/1000);

    for (int i = 0; i < nThreads; i++)
        free(heaps[i]);
    free(heaps);
    free(tamHeaps);

    free(input);
    free(output);

    return 0;
}
