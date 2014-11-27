#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

/* Initialization */

ofstream myfile;        /* init of output file */
const int MAX_SIZE = 10; 
string name_of_algo;
int process_count = 0;
int waiting_time = 0;
int turnaround_time = 0;

/* Function Declartion */

void readfile(string);
void writefile();
void cpu_scheduler();
void testformat();
void printAvg();
void fcfs();
void srtf();
void rr(int);

/* Process class */

class Process {
public:
    int arrive_t;
    int burst_t;
    int remain_t;
    int begin;
    int end;

    Process() {
        arrive_t = 0;
        burst_t = 0;
        begin = 0;
        end = 0;
    }

    void get_burst(int time)
    {
        burst_t = time;
        remain_t = time;
    }
} p[11]; 


int main()
{
    string filename;
    cout << "Enter file name: ";
    cin >> filename;
    readfile(filename);
    
    myfile.open("0013428.txt");
    cpu_scheduler();
    printAvg();

    return 0;
}

/* Implementation */

void readfile(string filename)
{    
    string line;
    ifstream testfile (filename);
    if (testfile.is_open()) {
        
        getline(testfile, line);
        name_of_algo = line.erase(0, 22);
       
        int i = 1;
        while(getline(testfile, line) && i <= 9) {            
            getline(testfile, line);
            getline(testfile, line);
            p[i].arrive_t = atoi(line.erase(0, 14).c_str());
            getline(testfile, line);
            p[i].get_burst( atoi(line.erase(0, 12).c_str()) );
            
            i++;
        }
        process_count = i - 1;
        testfile.close();
    }
    else cout << "Unable to open file, check out the name of file.\n"; 
}

void cpu_scheduler()
{
    if(name_of_algo.substr(0, 4).compare("FCFS") == 0)
    {
        fcfs();
    }

    if(name_of_algo.substr(0, 4).compare("SRTF") == 0)
    {
        srtf();
    }

    if(name_of_algo.substr(0, 2).compare("RR") == 0)
    {
        int quantum = atoi(name_of_algo.substr(3, 1).c_str());
        rr(quantum);
    } 
}

/* First-Come First-Served */

void fcfs()
{
    int    count = 0,       /* number of finished process */ 
           time = 0,        /* current time */  
           smallest;        /* First-Come process index */ 

    p[10].arrive_t = 9999;

    while(count != process_count) {
        smallest = 10;      /* Assume that process range from 1~9
                               , init process index to 10 in convenience */
    
        for(int i = 1; i <= process_count; i++) {
            if(p[i].arrive_t < p[smallest].arrive_t             /* first-come */
                                && p[i].remain_t > 0) {         /* check available */
                smallest = i;
            }
        }

        /* switch based time  */
        if(time < p[smallest].arrive_t)
            time = p[smallest].arrive_t;

        myfile << "P" << smallest << "\t" << time << "-";

        time += p[smallest].burst_t;            /* Increment current time */
        waiting_time += time - p[smallest].arrive_t - p[smallest].burst_t;
        turnaround_time += time - p[smallest].arrive_t;

        /* print current time */
        myfile << time << endl;

        /* process finished */
        p[smallest].remain_t = 0;
        count++;
    }
}

/* Shortest-Remaining Time First */

void srtf()
{
    int     smallest,
            smallest_last,      // store the last smallest for comparing
            remain = 0, 
            time;

    p[10].remain_t = 9999;
    for(time = 0; remain != process_count; time++) {
        smallest = 10;
        /* find the shortest time process */
        for(int i = 1; i <= process_count; i++) {
            if(p[i].arrive_t < time                         /* Available process */
                && p[i].remain_t < p[smallest].remain_t     /* find the smallest time process */
                && p[i].remain_t > 0)                       /* not finished */ 
            {
                smallest = i;
            }
        }
        p[smallest].remain_t--;

        /* if the process is preempted, 
         * print current time for previous process i
         * */
        if(smallest != 10 && smallest != smallest_last) {
            if(time-1 != 0)
                myfile << time-1 << endl;
            
            myfile << "P" << smallest << "\t" << time-1 << "-";
        }

        /* if current process finish */
        if(p[smallest].remain_t == 0) {
            remain++;
            
            waiting_time += time - p[smallest].burst_t - p[smallest].arrive_t;
            turnaround_time += time - p[smallest].arrive_t;
        }
        
        /* store the smallest process index for comparing */
        smallest_last = smallest;
    }
    myfile << time - 1 << endl;
}

/* Round Robin */

void rr(int quantum)
{
    int     time,            /* time unit of this algorithm */
            i,               /* index of process */ 
            flag = 0,        /* flag = 1 means process is finished */
            queue_flag,                    /* if queue flag is 0 means there are 
                                               available process after process[i]  */
            remain = process_count;        /* remaining process count */


    /* each iteration do the task in one time unit */
    for(time = 0, i = 1; remain != 0; ) 
    {
        /* set queue_flag = 0 every time unit */
        queue_flag = 0;

        if(p[i].remain_t <= quantum && p[i].remain_t > 0)
        {
            myfile << "P" << i << "\t" << time << "-";

            time += p[i].remain_t;
            p[i].remain_t = 0;

            /* process finished */
            flag = 1;
            
            /* when process is finished, print current time */
            myfile << time << endl;
        }
        /* process burst time > time quantum */
        else if(p[i].remain_t > 0)
        {
            myfile << "P" << i << "\t" << time << "-";

            p[i].remain_t -= quantum;
            time += quantum;
            /* time quantum is over, print current time */
            myfile << time << endl;
        }

        /* do when one of the process is done */
        if(p[i].remain_t == 0 && flag == 1)
        {
            remain--;
            waiting_time += time - p[i].burst_t - p[i].arrive_t;
            turnaround_time += time - p[i].arrive_t;
            flag = 0;

            /* check whether turnaround or not */
            queue_flag = 1;
            for (int j = i; j <= process_count; j++) {
                /* turn a round if there are available process */
                if(p[j].remain_t > 0 && p[j].arrive_t < time) {
                    queue_flag = 0;
                }
            }
        }

        /* round */
        if(i == process_count || queue_flag == 1)
            i = 1;
        else if(p[i+1].arrive_t <= time)
            i++;
        else
            i = 1;
    }
}


/* Helper Method */

void printAvg()
{
    double avg_waiting = (double) waiting_time / process_count;
    double avg_turnaround = (double) turnaround_time / process_count;

    myfile << fixed << setprecision(2);
    myfile << "Average Waiting Time: " << (double)waiting_time / process_count << endl;
    myfile << "Average Turnaround Time: " << (double)turnaround_time / process_count << endl;
}

void testformat()
{
    cout << name_of_algo << endl;
    for(int i = 1; i <= 9; i++) {
        cout << "process" << i << endl;
        cout << "p[" << i << "] arrive time: " << p[i].arrive_t << endl;
        cout << "p[" << i << "] burst time: " << p[i].burst_t << endl;
    }
}
