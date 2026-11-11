#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, s;
int *search;
int *result;

int *fib;


void *generate_fibonacci(void *arg)
{
    fib = (int *)malloc((n + 1) * sizeof(int));

    if (fib == NULL)
    {
        printf("Memory allocation failed!\n");
        pthread_exit(NULL);
    }

    fib[0] = 0;

    if (n >= 1)
        fib[1] = 1;

    for (int i = 2; i <= n; i++)
    {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    pthread_exit(NULL);
}


void *search_fibonacci(void *arg)
{
    result = (int *)malloc(s * sizeof(int));

    if (result == NULL)
    {
        printf("Memory allocation failed!\n");
        pthread_exit(NULL);
    }

    for (int i = 0; i < s; i++)
    {
        if (search[i] >= 0 && search[i] <= n)
            result[i] = fib[search[i]];
        else
            result[i] = -1;
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t t1, t2;

    printf("Enter the term of fibonacci sequence:\n");
    scanf("%d", &n);

    if (n < 0 || n > 40)
    {
        printf("Invalid input!\n");
        return 1;
    }

    printf("How many numbers you are willing to search?:\n");
    scanf("%d", &s);

    if (s <= 0)
    {
        printf("Invalid number of searches!\n");
        return 1;
    }

    search = (int *)malloc(s * sizeof(int));

    if (search == NULL)
        return 1;

    for (int i = 0; i < s; i++)
    {
        printf("Enter search %d:\n", i + 1);
        scanf("%d", &search[i]);
    }


    pthread_create(&t1, NULL, generate_fibonacci, NULL);


    pthread_join(t1, NULL);

    pthread_create(&t2, NULL, search_fibonacci, NULL);


    pthread_join(t2, NULL);


    for (int i = 0; i <= n; i++)
    {
        printf("a[%d] = %d\n", i, fib[i]);
    }


    for (int i = 0; i < s; i++)
    {
        printf("result of search #%d = %d\n", i + 1, result[i]);
    }

    free(fib);
    free(search);
    free(result);

    return 0;
}
