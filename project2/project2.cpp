#include <iostream>
#include <fstream>
#include <pthread.h>
using namespace std;

/* Initialization */

string filename[4]={"testdata1.txt", "testdata2.txt", "testdata3.txt", "testdata4.txt"};
int const thread_count = 4;
pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;

/* Function Declartions */

void *read(void*);

/* Main function */

int main(int argc, const char *argv[])
{
    /* Preparation for pthread_create */
    pthread_t *threads = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    /* open shared memory file */
    ofstream sharefile;
    sharefile.open("Shared.txt");

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

    ifstream testfile(filename[tid]);
    fstream sharedfile;
    sharedfile.open("Share.txt", fstream::app);

    if (testfile.is_open()) 
    {
        while(getline(testfile, line)) 
        {
            pthread_mutex_lock(&mu);
            /* write to share file */
            sharedfile << "Thread" << tid + 1 << " :  " << line << endl;
            pthread_mutex_unlock(&mu);
        }
        testfile.close();
    }
    else {
        cout << "Unable to open file, check out the name of file.\n";
    }

    return NULL;
}
