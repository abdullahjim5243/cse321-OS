
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t supplierSem;
sem_t makerA, makerB, makerC;

pthread_mutex_t mutex;

int done = 0;
int N;

void *supplier(void *arg)
{
    for (int i = 0; i < N; i++)
    {
        sem_wait(&supplierSem);

        pthread_mutex_lock(&mutex);

        int choice = i % 3;

        if (choice == 0)
        {
            printf("Supplier places: Bread and Cheese\n");
        }
        else if (choice == 1)
        {
            printf("Supplier places: Cheese and Lettuce\n");
        }
        else
        {
            printf("Supplier places: Bread and Lettuce\n");
        }

        pthread_mutex_unlock(&mutex);
        if (choice == 0)
            sem_post(&makerC);
        else if (choice == 1)
            sem_post(&makerA);
        else
            sem_post(&makerB);
    sem_wait(&supplierSem);

    pthread_mutex_lock(&mutex);
    done = 1;
    pthread_mutex_unlock(&mutex);


    sem_post(&makerA);
    sem_post(&makerB);
    sem_post(&makerC);

    return NULL;
}

void *maker(void *arg)
{
    int id = *(int *)arg;
    sem_t *mySem;

    if (id == 0)
        mySem = &makerA;
    else if (id == 1)
        mySem = &makerB;
    else
        mySem = &makerC;

    while (1)
    {
        sem_wait(mySem);

        pthread_mutex_lock(&mutex);

        if (done)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }

        if (id == 0)
            printf("Maker A picks up Cheese and Lettuce\n");
        else if (id == 1)
            printf("Maker B picks up Bread and Lettuce\n");
        else
            printf("Maker C picks up Bread and Cheese\n");

        pthread_mutex_unlock(&mutex);

        if (id == 0)
            printf("Maker A is making the sandwich...\n");
        else if (id == 1)
            printf("Maker B is making the sandwich...\n");
        else
            printf("Maker C is making the sandwich...\n");

        sleep(1);

        if (id == 0)
            printf("Maker A finished making the sandwich and eats it\n");
        else if (id == 1)
            printf("Maker B finished making the sandwich and eats it\n");
        else
            printf("Maker C finished making the sandwich and eats it\n");

        printf("Maker %c signals Supplier\n", 'A' + id);

        sem_post(&supplierSem);
    }

    return NULL;
}

int main()
{
    pthread_t supplierThread;
    pthread_t makerThreads[3];

    int ids[3] = {0, 1, 2};

    printf("Enter number of times supplier places ingredients: ");
    scanf("%d", &N);

    sem_init(&supplierSem, 0, 1);
    sem_init(&makerA, 0, 0);
    sem_init(&makerB, 0, 0);
    sem_init(&makerC, 0, 0);

    pthread_mutex_init(&mutex, NULL);


    for (int i = 0; i < 3; i++)
    {
        pthread_create(&makerThreads[i], NULL, maker, &ids[i]);
    }


    pthread_create(&supplierThread, NULL, supplier, NULL);


    pthread_join(supplierThread, NULL);

    for (int i = 0; i < 3; i++)
    {
        pthread_join(makerThreads[i], NULL);
    }

    sem_destroy(&supplierSem);
    sem_destroy(&makerA);
    sem_destroy(&makerB);
    sem_destroy(&makerC);

    pthread_mutex_destroy(&mutex);

    return 0;
}

