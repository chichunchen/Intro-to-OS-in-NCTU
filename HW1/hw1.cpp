#include <iostream>
#include <fstream>
using namespace std;

string name_of_algo;

class Process { 
public:
    int arrive_t;
    int burst_t;

    Process() {
        arrive_t = 0;
        burst_t = 0;
    }
} p[9]; 

void readfile(string);
void writefile();
void schedule();
void testformat();

int main()
{
    string filename;
    cout << "Enter file name: ";
    cin >> filename;
    readfile(filename);
    testformat();
    writefile();

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
            p[i].burst_t = atoi(line.erase(0, 12).c_str());
            
            i++;
        }

        myfile.close();
    }
    else cout << "Unable to open file, check out the name of file."; 
}

void writefile()
{
    ofstream myfile;
    myfile.open ("0013428.txt");
    myfile << "P1\n";
    myfile << "P2\n";
    myfile << "p3\n";
    myfile << "Average Waiting Time: \n";
    myfile << "Average Turnaround Time: \n";
    myfile.close();
}

void schedule()
{
    if(name_of_algo.compare("FCFS") == 0)
    {
        cout << "kjafsdfdsa fcfs" << endl;    
    }

    if(name_of_algo.compare("SRTF") == 0)
    {

    }

    if(name_of_algo.compare("RR") == 0)
    {

    } 
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
