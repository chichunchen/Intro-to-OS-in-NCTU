#include <iostream>
#include <fstream>
#include <pthread.h>
#include <sys/shm.h>
using namespace std;

//----------------------------------------------------
/* Initialization */
//----------------------------------------------------

string filename[4]={"testdata1.txt", 
                    "testdata2.txt", 
                    "testdata3.txt", 
                    "testdata4.txt"};

string SHARE_FILE = "Share.txt";
string RESULT_FILE = "Result.txt";


int const thread_count = 5;                            /* 4 read 1 write */

pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;        /* Initialize mutex lock */

ofstream sharefile;                                    /* Declare share file */
ofstream resultfile;                                   /* Declare result file */

pthread_key_t key;                                     /* Thread local Storage Key */

int global_sum = 0;

//----------------------------------------------------
/* Function Protocol */
//----------------------------------------------------

void *read(void*);
void *write(void*);

//----------------------------------------------------
/* Main function */
//----------------------------------------------------

int main(int argc, const char *argv[])
{
    /* Prepare for pthread_create */
    pthread_t *threads = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_key_create(&key, NULL);

    /* open shared memory file */
    sharefile.open(SHARE_FILE);

    /* open result file */
    resultfile.open(RESULT_FILE);

    /* use long in case of a 64-bit system */
    long thread;
    for(thread = 0; thread < thread_count; thread++) {
        if(thread < thread_count - 1)
            pthread_create(&threads[thread], &attr, read, (void*) thread);
        else
            pthread_create(&threads[thread], &attr, write, (void*) thread);
    }

    for (thread= 0; thread < thread_count; thread++) {
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

    /* Thread Local Storage: sum */
    int *sum = (int *)malloc(sizeof(int));
    *sum = 0;
    pthread_setspecific(key, sum);

    ifstream testfile(filename[tid]);

    if (testfile.is_open()) 
    {
        while(getline(testfile, line)) 
        {
            *sum += atoi(line.c_str());

            /* if line equals wait */
            if(line.compare("wait") == 0) {
                
                pthread_mutex_lock(&mu);
                cout << "Thread " << tid + 1 << ": sum is " << *sum << endl;
                sharefile << "Thread" << tid + 1 << " :  " << *sum << endl;
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

void *write(void* thread_id) 
{
    long tid = (long)thread_id;
    string line;

    ifstream sharememory("Share.txt");

    int i = 0;
    if(sharememory.is_open()) 
    {
        while(getline(sharememory, line)) {
            cout << "NO. : " << i << "output : " << line << endl;
            resultfile<< "NO. : " << i << "output : " << line << endl;
            i++;
        }
    }
    else {
        cout << "Unable to open file, check out the name of file.\n";        
    }

    return NULL;
}
