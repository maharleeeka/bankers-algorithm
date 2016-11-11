#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

class Process{
public:
    int processNo;
    string name;
    int allocation;
    bool isDone;
    bool isDeadlock;
    vector<int> resource_allocation;
    vector<int> maximum_demand;
    vector<int> needed_resources;

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
void showMatrices(vector<Process>, int, int);
void deadlockAvoidance(vector<Process>, vector<int>, int[]);
bool checkAllProcess(vector<Process>, int*);
bool checkAllocation(vector<int>, int*);
int* addResources(vector<int>, int*);
bool checkIfAllDone(vector<Process>);

int main(){

    int choice, n, tmp, resources;
    vector <Process> process;
    vector <int> resource_allocation;
    vector <int> maximum_demand;

    cout<<endl;
    cout<<endl;
    cout<<"*****BANKER'S ALGORITHM SIMULATION*****\n\n";
    cout<<"1. Deadlock Prevention\n2. Deadlock Avoidance\n3. Exit\n\nEnter choice number: ";
    cin>>choice;

    while(choice != 0){
        while(choice != 1 && choice != 2 && choice!= 3){
            cout<<"\nInvalid input. Please choose either 1, 2 or 3.\nEnter choice number: ";
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
                cout<<"PROCESS NO "<<tmp<<endl;
                cout<<"\tname: ";
                cin>>pr.name;
                cout<<"\ttime: ";
                cin>>pr.allocation;
                cout<<endl;
                cout<<endl;
                tmp++;

                process.push_back(pr);
            }

            cout<<"AVAILABLE RESOURCES: ";
            cin>>resources;

            deadlockPrevention(process, resources);
        }if(choice == 2){
            int noOfResources;
            cout<<"*****DEADLOCK AVOIDANCE*****\n\n";
            cout<<"Enter number of processes: ";
            cin>>n;
            cout<<"Enter number of resources: ";
            cin>>noOfResources;
            vector<int> res_alloc;
            vector<int> max_demand;
            vector<int> needed;
            int avail_resources[noOfResources];
            int input;


            cout<<"\n---RESOURCE ALLOCATION---"<<endl;
            for(int i = 1; i<=n; i++){
                Process pr;
                cout<<endl;
                cout<<"PROCESS NO "<<i<<endl;
                for(int j = 1; j <= noOfResources; j++){
                    cout<<"\tresource allocation "<<j<<": ";
                    cin>>input;
                    res_alloc.push_back(input);
                }
                pr.processNo = i;
                pr.resource_allocation = res_alloc;
                process.push_back(pr);
                res_alloc.clear();

            }

            cout<<"\n---MAXIMUM DEMAND---"<<endl;

            for(int i = 1; i<=n; i++){
                cout<<endl;
                cout<<"PROCESS NO "<<i<<endl;
                for(int j = 1; j <= noOfResources; j++){
                    cout<<"\tmaximum demand "<<j<<": ";
                    cin>>input;
                    max_demand.push_back(input);
                }
                process.at(i-1).maximum_demand = max_demand;
                max_demand.clear();
            }

            cout<<"\n---AVAILABLE RESOURCES---"<<endl;
            for(int i = 1; i <= noOfResources; i++){
                cout<<"\tresource "<<i<<": ";
                cin>>avail_resources[i-1];
            }

            for(int i = 1; i<=n; i++){
                cout<<endl;
                for(int j = 1; j <= noOfResources; j++){
                    input = process.at(i-1).maximum_demand.at(j-1) -
                            process.at(i-1).resource_allocation.at(j-1);
                    needed.push_back(input);
                }
                process.at(i-1).needed_resources = needed;
                needed.clear();
            }


        showMatrices(process,noOfResources, 1);
        showMatrices(process,noOfResources, 2);
        showMatrices(process,noOfResources, 3);
        deadlockAvoidance(process, needed, avail_resources);
        }if(choice == 3){
            cout<<"*****BYE BYE BYE*****\n\n";
            break;
        }
        process.clear();
        cout<<endl;
        cout<<endl;
        cout<<"*****BANKER'S ALGORITHM SIMULATION*****\n\n";
        cout<<"1. Deadlock Prevention\n2. Deadlock Avoidance\n3. Exit\n\nEnter choice number: ";
        cin>>choice;
    }

return 0;
}

bool checkDeadlock(vector<Process> process, int resources){
    for(int i = 0; i < process.size(); i++){
        Process pr = process.at(i);
        if(pr.allocation <= resources){
            return false;
        }
    }
    return true;
}

void deadlockPrevention(vector<Process> process, int resources){
    string safe_sequence = "< ";
    Process pr, temp_pr;
    vector<Process> temp_process;
    bool isDeadlock =  false;
    int i = 0, ctr = process.size();

    while(!isDeadlock){
        temp_process.clear();
        for(int j = 0; j < process.size(); j++){
            temp_pr = process.at(j);
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
    cout<<"SAFE SEQUENCE: "<<safe_sequence <<">";
    cout<<endl;
}

void showMatrices(vector<Process> proc_list, int n, int c){

    int i;

    if(c == 1){
        cout<<"\nResource Allocation"<<endl;
        i = 0;
        while(i < proc_list.size()){
            Process pr;
            pr = proc_list.at(i);
            cout<<"P"<<pr.processNo<<" ";
            for(int j = 0; j < pr.resource_allocation.size(); j++){
                cout<<" | "<< pr.resource_allocation.at(j);
            }
            i++;
            cout<<endl;
        }
    }else if(c == 2){
        i = 0;
        cout<<"\nMaximum Demand"<<endl;
        while(i < proc_list.size()){
            Process pr;
            pr = proc_list.at(i);
            cout<<"P"<<pr.processNo<<" ";
            for(int j = 0; j < pr.maximum_demand.size(); j++){
                cout<<" | "<< pr.maximum_demand.at(j);
            }
            i++;
            cout<<endl;
        }
    }else if(c == 3){
        i = 0;
        cout<<"\nNeeded Resources"<<endl;
        while(i < proc_list.size()){
            Process pr;
            pr = proc_list.at(i);
            cout<<"P"<<pr.processNo<<" ";
            for(int j = 0; j < pr.needed_resources.size(); j++){
                cout<<" | "<< pr.needed_resources.at(j);
            }
            i++;
            cout<<endl;
        }
    }

}

void deadlockAvoidance(vector<Process> proc_list, vector<int> needed_resources, int* avail_resources){
    bool processing = true;
    int i = 0;
    vector<int> temp;
    bool state;
    vector<int> safe_sequence;

    while(processing){
        temp = proc_list.at(i).resource_allocation;
        if(checkAllProcess(proc_list, avail_resources)){
            cout<<"\nSTATE: DEADLOCK\n";
            state = false;
            processing = false;
        }else{
            if(checkAllocation(temp, avail_resources)){
                proc_list.at(i).isDone = true;
                avail_resources = addResources(temp, avail_resources);
                cout<<"PROCESS "<<i+1<<": FINISHED\n";
                safe_sequence.push_back(proc_list.at(i).processNo);

            }
        }
        if(checkIfAllDone(proc_list)){
            cout<<"\n\nSTATE: SAFE\n";
            state = true;
            processing = false;
        }
        i++;
        if(i >= proc_list.size()){
            i = 0;
        }
    }
    cout<<"\nSAFE SEQUENCE: < ";
    for(int i = 0; i < safe_sequence.size(); i++){
        cout<<safe_sequence.at(i)<<" ";
    }
    cout<<" >";
    safe_sequence.clear();

    /*******************/
    int choice, newP;
    if(state){
        cout<<"\n\nADD NEW REQUEST?\n1. Yes\n2. No\nEnter choice: ";
        cin>>choice;
        while(choice!=1 && choice!=2){
            cout<<"\nADD NEW REQUEST?\n1. Yes\n2. No\nEnter choice: ";
            cin>>choice;
        }

        if(choice == 1){
            cout<<"\nWHICH PROCESS TO CHANGE?\n";
            showMatrices(proc_list, proc_list.size(), 1);
            cout<<"\nProcess <enter only a number>: ";
            cin>>newP;


            i = 0;
            cout<<"\nPROCESS NO "<<newP<<endl;
            while(i < proc_list.at(newP-1).resource_allocation.size()){
                cout<<"\tresource allocation "<<i+1<<": ";
                cin>>proc_list.at(newP-1).resource_allocation.at(i);
                i++;
            }
            showMatrices(proc_list, proc_list.size(), 1);
            showMatrices(proc_list, proc_list.size(), 2);
            showMatrices(proc_list, proc_list.size(), 3);
            deadlockAvoidance(proc_list, needed_resources, avail_resources);
        }
    }

}

bool checkAllProcess(vector<Process> proc_list, int* avail_resources){
    vector<int> temp;
    int ctr = 0;
    for(int i = 0; i < proc_list.size(); i++){
        cout<<"...";
        temp = proc_list.at(i).needed_resources;
        if(checkAllocation(temp, avail_resources)){
            return false;
        }
    }
    return true;
}

bool checkAllocation(vector<int> proc_allocation, int* avail_resources){
    int ctr = 0;
    for(int i = 0; i < proc_allocation.size(); i++){
        cout<<"...";
        if(proc_allocation.at(i)<=avail_resources[i]){
            ctr++;
        }
    }
    if(ctr == proc_allocation.size()){
        return true;
    }else{
        return false;
    }
}
int* addResources(vector<int> resource, int* available){
    for(int i = 0; i < resource.size(); i++){
        available[i] += resource.at(i);
    }
    return available;
}
bool checkIfAllDone(vector<Process> proc_list){

    for(int i = 0; i < proc_list.size(); i++){
        if(!proc_list.at(i).isDone){
            return false;
        }
    }

    return true;
}
