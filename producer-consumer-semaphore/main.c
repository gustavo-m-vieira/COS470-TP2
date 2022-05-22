#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <time.h>

#define M 10000

int *buffer;
sem_t empty, full, threadsProducer, threadsConsumer;
pthread_mutex_t mutex;
int consumedCounter, producedCounter = 0;
int np, nc, n;
int printPrime = 0;

int *initializeVector(int size)
{
  int *vector = (int *)malloc((size + 1) * sizeof(int));
  for (int i = 0; i < size; i++)
  {
    vector[i] = 0;
  }
  return vector;
}

void initializeSemaphores()
{
  sem_init(&empty, 0, n); // inicia o semaforo empty com valor n (tamanho do vetor)
  sem_init(&full, 0, 0);  // inicia o semaforo full com valor 0

  // inicia os semaforos de threads com 0
  sem_init(&threadsProducer, 0, 0);
  sem_init(&threadsConsumer, 0, 0);
}

char *checkPrime(int number)
{
  int i = 0;
  if (number == 1)
  {
    return "Número não é primo\n";
  }
  for (i = 2; i <= number / 2; i++)
  {
    if (number % i == 0)
    {
      return "Número não é primo\n";
    }
  }
  return "Número é primo\n";
}

int findEmptyPosition(int *vector) // posição vazia é igual a ter em seu conteudo o valor 0
{
  int position = -1; // retorna -1 em caso de erro
  for (int i = 0; i < n; i++)
  {
    if (vector[i] == 0)
    {
      position = i;
      break;
    }
  }
  return position;
}

int findFullPosition(int *vector) // retornará uma posição que seu conteudo nao seja 0
{
  int position = -1; // retorna -1 em caso de erro
  for (int i = 0; i < n; i++)
  {
    if (vector[i] != 0)
    {
      position = i;
      break;
    }
  }
  return position;
}

void *producer(void *pno) // código executado pela thread produtora
{
  do
  {
    sem_wait(&empty);                              // Irá aguardar se o valor de empty for 0
    pthread_mutex_lock(&mutex);                    // faz o lock do mutex para adentrar a região crítica
    int emptyPosition = findEmptyPosition(buffer); // encontra uma posição vazia no buffer

    if (emptyPosition == -1)
    {
      printf("Algo de errado ocorreu! Revise o código!");
    }

    buffer[emptyPosition] = rand() % 10000000 + 1; // gera um número aleatório e o insere nessa posição
    producedCounter++;

    pthread_mutex_unlock(&mutex); // desbloqueia o lock
    sem_post(&full);              // aumenta em 1 o valor do semaforo 'full'

  } while (producedCounter < M);

  sem_post(&threadsProducer); // aumenta em 1 o valor do semáforo 'threadsProducer', indicando que pode finalizar as threads produtoras
}

void *consumer(void *cno)
{
  do
  {
    sem_wait(&full);                              // Irá aguardar se o valor de full for 0
    pthread_mutex_lock(&mutex);                   // faz o lock do mutex para adentrar a região crítica
    int emptyPosition = findFullPosition(buffer); // encontra uma posição ocupada no buffer

    if (emptyPosition == -1)
    {
      printf("Algo de errado ocorreu! Revise o código!");
    }

    int value = buffer[emptyPosition];
    buffer[emptyPosition] = 0; // seta o valor da posição consumida em zero, "liberando" a posição
    if (printPrime > 0)
    {
      printf("valor %d %s\n", value, checkPrime(value)); // checa se é primo
    }

    consumedCounter++;

    pthread_mutex_unlock(&mutex); // desbloqueia o lock
    sem_post(&empty);             // aumenta em 1 o valor do semaforo 'empty'

  } while (consumedCounter < M);
  sem_post(&threadsConsumer); // aumenta em 1 o valor do semáforo 'threadsConsumer', indicando que pode finalizar as threads consumidoras
}

void finishMutexAndSem()
{
  pthread_mutex_destroy(&mutex);
  sem_destroy(&empty);
  sem_destroy(&full);
  sem_destroy(&threadsProducer);
  sem_destroy(&threadsConsumer);
}

int main(int argc, char *argv[])
{
  srand(time(NULL));
  struct timeval t1, t2;
  double elapsedTime;

  if (argc != 5)
  {
    printf("Argumentos inválidos, deve passar np, nc, n, printPrime(se deve printar o primo)");

    return 1;
  }

  np = atoi(argv[1]);
  nc = atoi(argv[2]);
  n = atoi(argv[3]);
  printPrime = atoi(argv[4]);

  pthread_t producers[np];
  pthread_t consumers[nc];
  int producersId[np];
  int consumersId[nc];

  initializeSemaphores();

  buffer = initializeVector(n); // inicalizando vetor!

  gettimeofday(&t1, NULL); // tempo de início

  // cria threads produtoras e consumidoras
  for (int i = 0; i < np; i++)
  {
    producersId[i] = i;
    pthread_create(&producers[i], NULL, (void *)producer, (void *)&producersId[i]);
  }

  for (int i = 0; i < nc; i++)
  {
    consumersId[i] = i;
    pthread_create(&consumers[i], NULL, (void *)consumer, (void *)&consumersId[i]);
  }

  sem_wait(&threadsProducer); // aguardando as threads produtoras enviarem 10⁵ números.
  for (int i = 0; i < np; i++)
  {
    pthread_cancel(producers[i]);
  }

  sem_wait(&threadsConsumer); // aguardando as threads consumidoras receberem 10⁵ números.
  for (int i = 0; i < nc; i++)
  {
    pthread_cancel(consumers[i]);
  }

  gettimeofday(&t2, NULL); // tempo final

  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0 + (t2.tv_usec - t1.tv_usec) / 1000.0;
  printf("%.0f", elapsedTime);

  finishMutexAndSem();

  free(buffer);

  return 0;
}