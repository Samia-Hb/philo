# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>

int var = 0;
pthread_mutex_t mutex;
void *F1()
{
    int i = 0;
    while (i < 1000000)
    {
        pthread_mutex_lock(&mutex);
        var++;
        i++;
        pthread_mutex_unlock(&mutex);
    }
    return (NULL);
}
int main()
{
    pthread_t p1, p2;
    pthread_mutex_init(&mutex, NULL);
    if (pthread_create(&p1, NULL, &F1, NULL) != 0)
        return (1);
    if (pthread_create(&p2,NULL, &F1,NULL) != 0)
        return (1);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("%d\n",var);
    // pthread_mutex_destroy(&mutex);
}