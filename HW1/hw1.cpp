#include <iostream>
#include <fstream>
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
} p[9]; 

class ProcessQueue {
private:
    int item[MAX_SIZE];
    int rear;
    int front;
public:
    ProcessQueue();
    void enqueue(int);
    int dequeue();
    int size();
    void display();
    bool isEmpty();
    bool isFull();
};
ProcessQueue::ProcessQueue(){
    rear = -1;
    front = 0;
}
void ProcessQueue::enqueue(int data){
    item[++rear] = data;
}
int ProcessQueue::dequeue(){
    return item[front++];
}
void ProcessQueue::display(){
    if(!this->isEmpty()){
        for(int i=front; i<=rear; i++)
            cout<<item[i]<<endl;
    }else{
        cout<<"Queue Underflow"<<endl;
    }
}
int ProcessQueue::size(){
    return (rear - front + 1);
}
bool ProcessQueue::isEmpty(){
    if(front>rear){
        return true;
    }else{
        return false;
    }
}

bool ProcessQueue::isFull(){
    if(this->size()>=MAX_SIZE){
        return true;
    }else{
        return false;
    }
}

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
    // testformat();
    cpu_scheduler();
    // writefile();

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

void writefile()
{
    ofstream myfile;
    myfile.open ("0013428.txt");
    for(int i = 0; i < process_count; i++) {
        myfile << "P" << i << "    " << p[i].begin << "-" << p[i].end << endl;
    }
    myfile << "Average Waiting Time: \n";
    myfile << "Average Turnaround Time: \n";
    myfile << "Gantt Chart: \n";
    myfile.close();
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
    
}

void srtf()
{
    
}

void rr(int quantum)
{
    ofstream myfile;
    myfile.open ("0013428.txt");

    int time, i; 
    int remain = process_count;
    int flag = 0;
    for(time = 0, i = 1; remain != 0; ) {

        if(p[i].remain_t <= quantum && p[i].remain_t > 0)
        {
            myfile << "P" << i << "    " << time << "-";

            time += p[i].remain_t;
            p[i].remain_t = 0;
            flag = 1;
            // cout << "end of " << i << " : " << p[i].remain_t << endl;
            
            myfile << time << endl;
        }
        else if(p[i].remain_t > 0)
        {
            myfile << "P" << i << "    " << time << "-";

            p[i].remain_t -= quantum;
            time += quantum;
            // cout << i << " : " << p[i].remain_t << endl;
            myfile << time << endl;
        }

        if(p[i].remain_t == 0 && flag == 1)
        {
            remain--;
            waiting_time += time - p[i].burst_t - p[i].arrive_t;
            turnaround_time += time - p[i].arrive_t;
            flag = 0;
        }

        if(i == process_count)
            i = 1;
        else if(p[i+1].arrive_t <= time)
            i++;
    }
    myfile << "Average Waiting Time: " << waiting_time / process_count << endl;
    myfile << "Average Turnaround Time: " << turnaround_time / process_count << endl;
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
