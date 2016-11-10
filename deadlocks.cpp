#include <iostream>
#include <vector>

using namespace std;

class Process{
public:
    int processNo;
    string name;
    int allocation;
    bool isDone;
    bool isDeadlock;

    Process(){
        processNo = 0;
        name = "";
        allocation = 0;
        isDone = false;
        isDeadlock = false;
    }
    Process(int processNo, string name, int allocation){
        this->processNo = processNo;
        this->name = name;
        this->allocation = allocation;
        isDone = false;
        isDeadlock = false;
    }

    void setStatus(bool a){
        this->isDone = a;
    }
};

void deadlockPrevention(vector<Process>, int);

int main(){

    int choice, n, tmp, resources;
    vector <Process> process;
    cout<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"*****BANKER'S ALGORITHM SIMULATION*****\n\n";
    cout<<"1. Deadlock Prevention\n2. Deadlock Avoidance\nEnter choice number: ";
    cin>>choice;

    while(choice != 1 && choice != 2 ){
        cout<<"\nInvalid input. Please choose either 1 or 2.\nEnter choice number: ";
        cin>>choice;
    }
    cout<<"\n\n";
    if(choice == 1){
        cout<<"*****DEADLOCK PREVENTION*****\n\n";
        cout<<"Enter number of processes: ";
        cin>>n;
        tmp = 1;
        cout<<endl;
        while(tmp<=n){
            Process pr;
            cout<<"NAME for process number "<<tmp <<": ";
            cin>>pr.name;
            cout<<"TIME for process number "<<tmp <<": ";
            cin>>pr.allocation;
            cout<<endl;
            cout<<endl;
            tmp++;

            process.push_back(pr);
        }

        cout<<"Enter value of available resources: ";
        cin>>resources;
    }else{
        cout<<"*****DEADLOCK AVOIDANCE*****\n\n";
    }
    deadlockPrevention(process, resources);

return 0;
}

bool checkDeadlock(vector<Process> process, int resources){
    for(int i = 0; i < process.size(); i++){
        Process pr = process.at(i);
        if(pr.allocation < resources){
            return false;
        }
    }
    return true;
}

void deadlockPrevention(vector<Process> process, int resources){
    string safe_sequence = "";
    Process pr, temp_pr;
    vector<Process> temp_process;
    bool isDeadlock =  false;
    int i = 0, ctr = process.size();

    while(!isDeadlock){

        temp_process.clear();
        for(int j = 0; j < process.size(); j++){
            if(!(temp_pr.isDone)){
                temp_process.push_back(temp_pr);
            }
        }

        if(temp_process.empty()){
            isDeadlock = true;
            break;
        }

        if(!checkDeadlock(temp_process, resources)){
            if(!(process.at(i).isDone)){
                if(process.at(i).allocation <= resources){
                    safe_sequence+=process.at(i).name + " ";
                    resources+=process.at(i).allocation;
                    process.at(i).isDone = true;
                }
            }
        }else{
            isDeadlock = true;
        }
        i++;
        if(i >= ctr){
            i = 0;
        }

    }
    cout<<safe_sequence;
}