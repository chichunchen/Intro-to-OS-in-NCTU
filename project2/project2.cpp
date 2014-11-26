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

int main(int argc, const char *argv[])
{
    /* Preparation for pthread_create */
    pthread_t *threads = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    /* open shared memory file */
    ofstream sharefile;
    sharefile.open("Shared.txt");

    /* use long in case of a 64-bit system */
    long i;
    for(i = 0; i < thread_count; i++) {
        pthread_create(&threads[i], &attr, read, (void*) i);
    }

    for (i = 0; i < 1; i++) {
        pthread_join(threads[i], NULL);
    }


    free(threads);
    return 0;
}

void *read(void* id)
{
    long thread_id = (long)id;
    string line;

    ifstream testfile(filename[thread_id]);
    fstream sharedfile;
    sharedfile.open("Share.txt", fstream::app);

    if (testfile.is_open()) 
    {
        while(getline(testfile, line)) 
        {
            pthread_mutex_lock(&mu);
            cout << "Thread" << thread_id + 1 << " :  " << line << endl;   
            sharedfile << "Thread" << thread_id + 1 << " :  " << line << endl;
            pthread_mutex_unlock(&mu);
        }
        testfile.close();
    }
    else {
        cout << "Unable to open file, check out the name of file.\n";
    }


    return NULL;
}
