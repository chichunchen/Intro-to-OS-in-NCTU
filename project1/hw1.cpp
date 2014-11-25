#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

const int MAX_SIZE = 10; 
string name_of_algo;
int process_count = 0;
int waiting_time = 0;
int turnaround_time = 0;

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

void readfile(string);
void writefile();
void cpu_scheduler();
void testformat();
void fcfs();
void srtf();
void rr(int);

int main()
{
    string filename;
    cout << "Enter file name: ";
    cin >> filename;
    readfile(filename);
    
    cpu_scheduler();

    return 0;
}


void readfile(string filename)
{    
    string line;
    ifstream myfile (filename);
    if (myfile.is_open()) {
        
        getline(myfile, line);
        name_of_algo = line.erase(0, 22);
       
        int i = 1;
        while(getline(myfile, line) && i <= 9) {            
            getline(myfile, line);
            getline(myfile, line);
            p[i].arrive_t = atoi(line.erase(0, 14).c_str());
            getline(myfile, line);
            p[i].get_burst( atoi(line.erase(0, 12).c_str()) );
            
            i++;
        }
        process_count = i - 1;
        myfile.close();
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

void fcfs()
{
    ofstream myfile;
    myfile.open ("0013428.txt");

    int count = 0, time = 0, smallest; 
    p[10].arrive_t = 9999;

    while(count != process_count) {
        smallest = 10;
    
        for(int i = 1; i <= process_count; i++) {
            if(p[i].arrive_t < p[smallest].arrive_t && p[i].remain_t > 0) {
                smallest = i;
            }
        }

        if(time < p[smallest].arrive_t)
            time = p[smallest].arrive_t;

        myfile << "P" << smallest << "\t" << time << "-";

        time += p[smallest].burst_t;
        waiting_time += time - p[smallest].arrive_t - p[smallest].burst_t;
        turnaround_time += time - p[smallest].arrive_t;

        myfile << time << endl;

        p[smallest].remain_t = 0;
        count++;
    }
    myfile << fixed << setprecision(2);
    myfile << "Average Waiting Time: " << (double)waiting_time / process_count << endl;
    myfile << "Average Turnaround Time: " << (double)turnaround_time / process_count << endl;
}

void srtf()
{
    ofstream myfile;
    myfile.open ("0013428.txt");

    int smallest;
    int smallest_last;  // store the last smallest for comparing
    int remain = 0, time;

    p[10].remain_t = 9999;
    for(time = 0; remain != process_count; time++) {
        smallest = 10;
        // find the shortest burst time process
        for(int i = 1; i <= process_count; i++) {
            if(p[i].arrive_t < time && p[i].remain_t < p[smallest].remain_t && p[i].remain_t > 0) {
                smallest = i;
            }
        }
        p[smallest].remain_t--;

        if(smallest != 10 && smallest != smallest_last) {
            if(time-1 != 0)
                myfile << time-1 << endl;
            
            myfile << "P" << smallest << "\t" << time-1 << "-";
        }

        if(p[smallest].remain_t == 0) {
            remain++;
            
            waiting_time += time - p[smallest].burst_t - p[smallest].arrive_t;
            turnaround_time += time - p[smallest].arrive_t;
        }
        
        // store the smallest to compare 
        smallest_last = smallest;
    }
    myfile << time - 1 << endl;
    myfile << setprecision(2);
    myfile << "Average Waiting Time: " << (double) waiting_time / process_count << endl;
    myfile << "Average Turnaround Time: " << (double) turnaround_time / process_count << endl;
}

void rr(int quantum)
{
    ofstream myfile;
    myfile.open ("0013428.txt");

    int time, i; 
    int remain = process_count;
    int flag = 0;   // flag = 1 means a process is finished

    // each iteration do a time quatum task
    for(time = 0, i = 1; remain != 0; ) 
    {
        // for(int j = 1; j <= process_count; j++)
        // {
        //     if(p[j].arrive_t <= time)
        //         cout << "process " << j << " ";
        // }
        // cout << endl;
        // process burst time < time quantum
        if(p[i].remain_t <= quantum && p[i].remain_t > 0)
        {
            myfile << "P" << i << "\t" << time << "-";

            time += p[i].remain_t;
            p[i].remain_t = 0;
            flag = 1;
            
            myfile << time << endl;
        }
        // process burst time > time quantum
        else if(p[i].remain_t > 0)
        {
            myfile << "P" << i << "\t" << time << "-";

            p[i].remain_t -= quantum;
            time += quantum;
            myfile << time << endl;
        }

        // when one process is done
        if(p[i].remain_t == 0 && flag == 1)
        {
            remain--;
            waiting_time += time - p[i].burst_t - p[i].arrive_t;
            turnaround_time += time - p[i].arrive_t;
            flag = 0;
        }

        // round
        if(i == process_count)
            i = 1;
        else if(p[i+1].arrive_t <= time)
            i++;
        else
            i = 1;
    }

    myfile << fixed << setprecision(2);
    myfile << "Average Waiting Time: " << (double)waiting_time / process_count << endl;
    myfile << "Average Turnaround Time: " << (double)turnaround_time / process_count << endl;
}


// testing the input file format
void testformat()
{
    cout << name_of_algo << endl;
    for(int i = 1; i <= 9; i++) {
        cout << "process" << i << endl;
        cout << "p[" << i << "] arrive time: " << p[i].arrive_t << endl;
        cout << "p[" << i << "] burst time: " << p[i].burst_t << endl;
    }
}
