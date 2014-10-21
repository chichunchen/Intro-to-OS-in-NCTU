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
}p1, p2, p3; 

void readfile(string);
void writefile();
void schedule();

int main()
{
    string filename;
    cout << "Enter file name: ";
    cin >> filename;
    readfile(filename);
    schedule();
    writefile();

    return 0;
}


void readfile(string filename)
{    
    string line;
    ifstream myfile (filename);
    if (myfile.is_open()) {
        // while ( getline (myfile,line) )
            // cout << line << '\n';
        
        getline(myfile, line);
        name_of_algo = line.erase(0, 22);
        getline(myfile, line);
        getline(myfile, line);
        getline(myfile, line);
        p1.arrive_t = atoi(line.erase(0, 14).c_str());
        getline(myfile, line);
        p1.burst_t = atoi(line.erase(0, 12).c_str());

        getline(myfile, line);
        getline(myfile, line);
        getline(myfile, line);
        p2.arrive_t = atoi(line.erase(0, 14).c_str());
        getline(myfile, line);
        p2.burst_t = atoi(line.erase(0, 12).c_str());

        getline(myfile, line);
        getline(myfile, line);
        getline(myfile, line);
        p3.arrive_t = atoi(line.erase(0, 14).c_str());
        getline(myfile, line);
        p3.burst_t = atoi(line.erase(0, 12).c_str());

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
