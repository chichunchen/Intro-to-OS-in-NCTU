#include <iostream>
#include <cstdlib>
#include <fstream>
#include <pthread.h> 
#include <semaphore.h> 
using namespace std;

//----------------------------------------------------
//* Initialization */
//----------------------------------------------------

string input[4]={"testdata1.txt","testdata2.txt","testdata3.txt","testdata4.txt"};

sem_t sem[4];             // semaphores for the four thread
sem_t total_sem;          // semaphore for the summing thread
sem_t file_sem;           // semaphore for the global varible writing

long long final_sum=0;
int flag=1;

//----------------------------------------------------
/* Function Protocol */
//----------------------------------------------------

void *readEntity(void*);
void *writeEntity(void*);

//----------------------------------------------------
/* Main function */
//----------------------------------------------------

int main()
{
    /* Declare 4 read threads and one write thread */
    pthread_t threads[4]; 
    pthread_t write_thread; 

    void *status;
    int taskids[4]={0,1,2,3};
    int rc; //error recorder
    for(int i = 0; i < 4; i++){
        sem_init(&sem[i], 0, 0); //init the semaphores to 0
    }
    sem_init(&total_sem, 0, 0);
    sem_init(&file_sem, 0, 1); //only one can write the file

    /* Create thread */
    for(int t=0;t<4;t++){
        rc = pthread_create(&threads[t], NULL, readEntity, taskids+t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }
    pthread_create(&write_thread, NULL, writeEntity, NULL);
    pthread_exit((void *)0);

    /* Destroy Semaphores */
    for(int i = 0; i < 4; i++) 
        sem_destroy(&sem[i]); 
    sem_destroy(&file_sem);
    sem_destroy(&total_sem);
    return 0;
}

//----------------------------------------------------
/* Implementation */
//----------------------------------------------------

void *readEntity(void *threadid)
{ 
    /* function for sum up before wait */
    int    *id_ptr, 
           taskid;

    id_ptr = (int*) threadid;
    taskid = *id_ptr;

    /* Init local sum */
    long long sum = 0;
    
    /* Open testdata#.txt */
    fstream testfile;
    testfile.open(input[taskid].c_str());
    
    if(testfile){
        string tmp;
        while(getline(testfile, tmp))
        {
            if(tmp == "wait" || tmp == "wait\r")
            {
                /* Only one read entity can write the global sum at a time */
                sem_wait(&file_sem); 
                final_sum += sum; 
                sem_post(&file_sem);

                /* set local sum to zero */
                sum = 0; 

                /* Signal write entity to write global sum to Result file */
                sem_post(&total_sem); 
                /* Wait for other threads and the total summing thread */
                sem_wait(&sem[taskid]); 
            }
            else
            {
                sum += atoi(tmp.c_str());
            }
        }
    }
    else 
        cout << "Unable to open file, check out the name of file.\n";
    
    flag = 0;
    pthread_exit((void *)0);
}

void *writeEntity(void*)
{
    int    sem_value = 0,
           count=1;

    /* Open Result.txt */
    fstream file;
    file.open("Result.txt",ios::out);

    while(flag)
    {
        sem_getvalue(&total_sem, &sem_value);
        if(sem_value == 4)
        {
            /* local summing is done,write the file! */
            file << "No. " << count << " output : " << final_sum << endl;
            count++;
            final_sum = 0;
            for(int i = 0; i < 4; i++)
            {
                /* Four of the reading threads are ready to go */
                sem_post(&sem[i]); 
                /* Wait for reading threads */
                sem_wait(&total_sem); 
            }
        }
    }

    file.close();
    pthread_exit((void *)0);
}
