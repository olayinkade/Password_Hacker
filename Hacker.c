#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "hash_test.h"

#define NOFTHREADS 10

bool noMoreWork = false;
char currLine[34];
bool workAvailable = false;

FILE *fp;

pthread_mutex_t signalLock;
pthread_cond_t signalCond;

pthread_mutex_t threadAvailableLock;
pthread_cond_t threadAvailableCond;

char *unsalt(char password[], int *index)
{
  char *unsalted = (char *)malloc(sizeof(char) * 22);
  unsalted[0] = 'a';
  unsalted[1] = '\0';
  int count = 0;
  bool equal = false;
  for (int i = 0; i <= 20 && !equal; i++)
  {
    equal = same(unsalted, password);
    count++;
    int posi = i;
    while (!equal && (int)unsalted[0] != 123 && posi >= 0)
    {
      if ((int)unsalted[posi] + 1 != 123)
      {
        count++;
      }
      else
      {
        posi--;
      }
      unsalted[posi] += 1;
      equal = same(unsalted, password);
    }
    for (int j = 0; j <= i + 1 && !equal; j++)
    {
      unsalted[j] = 'a';
    }
    unsalted[i + 2] = '\0';
  }
  *index = count;
  return unsalted;
}

void *threading_task(void *ignored)
{

  while (!noMoreWork)
  {

    char password[34];
    pthread_mutex_lock(&signalLock);
    while (!workAvailable)
    {
      pthread_cond_wait(&signalCond, &signalLock);
      if (noMoreWork)
      {
        pthread_mutex_unlock(&signalLock);
        pthread_exit(0);
      }
    }
    strcpy(password, currLine);
    workAvailable = false;
    pthread_mutex_unlock(&signalLock);
    int num = 0;
    int *index = &num;
    char *b = unsalt(password, index);

    pthread_mutex_lock(&threadAvailableLock);
    printf("%s plaintext %s took %d comparison\n", password, b, *index);
    pthread_cond_signal(&threadAvailableCond);
    pthread_mutex_unlock(&threadAvailableLock);
  }
  pthread_exit(0);
}

int main()
{
  fp = fopen("passwords.txt", "r");
  if (fp == NULL)
  {
    printf("Could not open file test.c");
    return 1;
  }

  pthread_t threads[NOFTHREADS];

  pthread_cond_init(&signalCond, NULL);
  pthread_mutex_init(&signalLock, NULL);
  pthread_cond_init(&threadAvailableCond, NULL);
  pthread_mutex_init(&threadAvailableLock, NULL);

  for (int i = 0; i < NOFTHREADS; i++)
  {
    pthread_create(&threads[i], NULL, threading_task, NULL);
  }

  char password[34];
  while (fgets(password, 36, fp) != 0)
  {
    strtok(password, "\n");
    pthread_mutex_lock(&signalLock);
    strcpy(currLine, password);
    workAvailable = true;
    pthread_cond_signal(&signalCond);
    pthread_mutex_unlock(&signalLock);

    pthread_mutex_lock(&threadAvailableLock);
    while (workAvailable)
    {
      pthread_cond_wait(&threadAvailableCond, &threadAvailableLock);
    }
    pthread_mutex_unlock(&threadAvailableLock);
  }

  pthread_mutex_lock(&signalLock);
  noMoreWork = true;
  pthread_cond_broadcast(&signalCond);

  pthread_mutex_unlock(&signalLock);

  for (int i = 0; i < NOFTHREADS; i++)
  {
    pthread_join(threads[i], NULL);
  }

  fclose(fp);
  pthread_mutex_destroy(&signalLock);
  pthread_cond_destroy(&signalCond);
  pthread_mutex_destroy(&threadAvailableLock);
  pthread_cond_destroy(&threadAvailableCond);
}
