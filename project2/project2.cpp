#include <iostream>
#include <fstream>
#include <pthread.h>
#include <sys/shm.h>
using namespace std;

/* Initialization */

string filename[4]={"testdata1.txt", "testdata2.txt", "testdata3.txt", "testdata4.txt"};
int const thread_count = 4;
pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
/* declare share memory file */
ofstream sharefile;
/* thread local storage key */
pthread_key_t key;

/* Function Declartions */

void *read(void*);

/* Main function */

int main(int argc, const char *argv[])
{
    /* Preparation for pthread_create */
    pthread_t *threads = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_key_create(&key, NULL);

    /* open shared memory file */
    sharefile.open("Share.txt");

    /* open result file */
    ofstream resultfile;
    resultfile.open("Result.txt");

    /* use long in case of a 64-bit system */
    long thread;
    for(thread = 0; thread < thread_count; thread++) {
        pthread_create(&threads[thread], &attr, read, (void*) thread);
    }

    for (thread= 0; thread < 1; thread++) {
        pthread_join(threads[thread], NULL);
    }

    free(threads);
    return 0;
}

/* Implementation */

void *read(void* thread_id)
{
    long tid = (long)thread_id;
    string line;

    /* Thread Local Storage for sum */
    int *sum = (int *)malloc(sizeof(int));
    *sum = 0;
    pthread_setspecific(key, sum);

    ifstream testfile(filename[tid]);

    if (testfile.is_open()) 
    {
        while(getline(testfile, line)) 
        {
            pthread_mutex_lock(&mu);
            /* write to share file */
            sharefile << "Thread" << tid + 1 << " :  " << line << endl;
            pthread_mutex_unlock(&mu);

            *sum += atoi(line.c_str());
            cout << "sum is " << *sum << " from thread " << tid << endl;

            if(line.compare("wait") == 0) {
                pthread_mutex_lock(&mu);
                sharefile << "Thread" << tid + 1 << " :  " << line << endl;
                pthread_mutex_unlock(&mu);

                /* after wait, set sum to 0 */
                *sum = 0;
            }
        }
        pthread_setspecific(key, NULL);
        free(sum);
        testfile.close();
    }
    else {
        cout << "Unable to open file, check out the name of file.\n";
    }

    return NULL;
}
