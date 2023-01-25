// 6809-6953
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <queue>
#include <string.h>
#include <sstream>
#include <string>
#include <math.h>
using namespace std;
#define max_lines 100
#define Maximum 30

 void drawTable(char array[],int x,int y){
    for (int i=0;i<y;i++)
        array[i]=' ';
    for (int i=11;i<=(x*6)+11;i=i+6)
        array[i]='|';
}

float rounded(float x){
    float val = (int)(x*100 + 0.5);
    return (float)val/100;
}

 void drawTable2(char array[],int x,int number){
    if(x==0){
    for (int i=0;i<number;i++)
        array[i]=' ';
    }
    else if(x==1){
    for (int i=0;i<number-1;i++)
        array[i]='-';
    }
}

//PCP
typedef struct PCP{ 
	int arrival; 
	char name;
	int process_time;
	int finish_time;
	int turnaround_time;
	double Normalized_turn;
    float ratio;
    int level;
    int priority;
    int origPriority;
}element; 

//Some needed Functions
typedef struct compareTime{
    bool operator()(element* const& p1, element* const& p2)
    {
    return p1->process_time > p2->process_time;
    }
}compareTime;

typedef struct compareRatio{
    bool operator()(element* const& p1, element* const& p2)
    {
    return p1->ratio < p2->ratio;
    }
}compareRatio;

typedef struct comparePriority{
    bool operator()(element* const& p1, element* const& p2)
    {
    return p1->priority < p2->priority;
    }
}comparePriority;

int inQueue(queue<element*> q, element *pro_data){
    int found=0;
    queue<element*> tempq;
    while(!q.empty()){
        if(q.front() == pro_data)
            found=1;
        tempq.push(q.front());
        q.pop();
    }
    while(!tempq.empty()){
        q.push(tempq.front());
        tempq.pop();
    }
    return found;
}

int inPriorityQueue(priority_queue<element*, vector<element *>, compareTime> pq, element *pro_data){
    int found=0;
    priority_queue<element*> tempq;
    while(!pq.empty()){
        if(pq.top() == pro_data)
            found=1;
        tempq.push(pq.top());
        pq.pop();
    }
    while(!tempq.empty()){
        pq.push(tempq.top());
        tempq.pop();
    }
    return found;
}

int inPriorityQueue2(priority_queue<element*, vector<element *>, compareRatio> pq, element *pro_data){
    int found=0;
    priority_queue<element*, vector<element *>, compareRatio> tempq;
    while(!pq.empty()){
        if(pq.top() == pro_data)
            found=1;
        tempq.push(pq.top());
        pq.pop();
    }
    while(!tempq.empty()){
        pq.push(tempq.top());
        tempq.pop();
    }
    return found;
}

int inPriorityQueue3(priority_queue<element*, vector<element *>, comparePriority> pq, element *pro_data){
    int found=0;
    priority_queue<element*, vector<element *>, comparePriority> tempq;
    while(!pq.empty()){
        if(pq.top() == pro_data)
            found=1;
        tempq.push(pq.top());
        pq.pop();
    }
    while(!tempq.empty()){
        pq.push(tempq.top());
        tempq.pop();
    }
    return found;
}

void incrementPriority(priority_queue<element*, vector<element *>, comparePriority> pq){
    priority_queue<element*, vector<element *>, comparePriority> tempq;
    while(!pq.empty()){
        pq.top()->priority++;
        tempq.push(pq.top());
        pq.pop();
    }
    while(!tempq.empty()){
        pq.push(tempq.top());
        tempq.pop();
    }
}

int minProcessTime(element* pro_data[]){
    int i = 0;
    int minn = pro_data[0]->arrival;
    while (pro_data[i]->name)
    {
        minn = min(minn, pro_data[i]->arrival);
        i++;
    }
    return minn;
}

int minProcessTimeIndex(element* pro_data[]){
    int minn = 0;
    int i = 1;
    while (pro_data[i]->name)
    {
        if (pro_data[minn]->arrival > pro_data[i]->arrival)
            minn = i;
        i++;
    }
    return minn;
}

int getFinishTime(element* pro_data[]){
    int i=0;
    int sum=0;
    while(pro_data[i]->name){
        sum=sum+pro_data[i]->process_time;
        i++;
    }
    return sum;
}



void FCFS(element* pro_data[],int ftime, char* excecutionArr){
    // get start time
    int CurrentTime = minProcessTime(pro_data);
    // get all processes where arrival time <= shorter time
    int i,k=0;
    std::queue<element*> q;
    //int ftime=getFinishTime(pro_data)+CurrentTime;
    element* curr_process;
    while(CurrentTime < ftime){
        i=0;
        while(pro_data[i]->name){
            if(pro_data[i]->arrival <= CurrentTime && pro_data[i]->process_time > 0 && inQueue(q, pro_data[i]) == 0)
                q.push(pro_data[i]);
            i++;
        }
        curr_process = q.front();
        q.pop();
        CurrentTime=CurrentTime+curr_process->process_time;
        curr_process->finish_time=CurrentTime;
        curr_process->process_time=0;
        while(k<CurrentTime){
            excecutionArr[k]=curr_process->name;
            k++;
        }
    }
}





void RR(element* pro_data[], int qn, int ftime, char* excecutionArr){
    std::queue<element*> q;
    int idx = minProcessTimeIndex(pro_data);
    int CurrentTime = pro_data[idx]->arrival;
    // store the original value of qn as it will be changed
    int qnOrig=qn;
    // initialize queue
    q.push(pro_data[idx]);
    int i, k = 0;
    element* curr_process;
    while(CurrentTime < ftime){
        // restore old value of quantum
        qn=qnOrig;
        curr_process=q.front();
        while(curr_process->process_time < 1){
            q.pop();
            curr_process=q.front();
        }
        q.pop();
        if(qn > curr_process->process_time){
            qn=curr_process->process_time;
        }
        CurrentTime=CurrentTime+qn;
        curr_process->process_time=curr_process->process_time-qn;
        curr_process->finish_time=CurrentTime;
        while(k<CurrentTime){
            excecutionArr[k]=curr_process->name;
            k++;
        }
        i=0;
        while(pro_data[i]->name){
            if((pro_data[i]->name != curr_process->name) && (inQueue(q, pro_data[i]) == 0) && (pro_data[i]->arrival <= CurrentTime) && (pro_data[i]->process_time > 0)){
                q.push(pro_data[i]);
            }
            i++;
        }
        if(curr_process->process_time > 0)
            q.push(curr_process);
    }
}

void SPN(element* pro_data[], int ftime, char* excecutionArr){
    // get start time
    int CurrentTime = minProcessTime(pro_data);
    // get all processes where arrival time <= shorter time
    int i,k=0;
    std::priority_queue<element*, vector<element *>, compareTime> pq;
    element* curr_process;
    while(CurrentTime < ftime){
        i=0;
        while(pro_data[i]->name){
            if(pro_data[i]->arrival <= CurrentTime && pro_data[i]->process_time > 0 && inPriorityQueue(pq, pro_data[i]) == 0)
                pq.push(pro_data[i]);
            i++;
        }
        curr_process = pq.top();
        pq.pop();  
        CurrentTime=CurrentTime+curr_process->process_time;
        curr_process->finish_time=CurrentTime;
        curr_process->process_time=0;
        while(k<CurrentTime){
            excecutionArr[k]=curr_process->name;
            k++;
        }
    }
}

void SRT(element* pro_data[], int ftime, char* excecutionArr){
    // get start time
    int CurrentTime = minProcessTime(pro_data);
    std::priority_queue<element*, vector<element *>, compareTime> pq;
    int i,k=0;
    element* curr_process;
    while(CurrentTime < ftime){
        i=0;
        while(pro_data[i]->name){
            if(pro_data[i]->arrival <= CurrentTime && pro_data[i]->process_time > 0 && inPriorityQueue(pq, pro_data[i]) == 0)
                pq.push(pro_data[i]);
            i++;
        }
        curr_process=pq.top();
        pq.pop();
        CurrentTime++;
        curr_process->finish_time=CurrentTime;
        curr_process->process_time--;
        while(k<CurrentTime){
            excecutionArr[k]=curr_process->name;
            k++;
        }
        // pop elements to maintain ordered values
        while(!pq.empty())
            pq.pop();
    }
}

void HRRN(element* pro_data[], int ftime, char* excecutionArr){
    // get start time
    int CurrentTime = minProcessTime(pro_data);
    std::priority_queue<element*, vector<element *>, compareRatio> pq;
    int i, timeWaiting, duration;
    int k=0;
    float r;
    element* curr_process;
    while(CurrentTime < ftime){
        i=0;
        while(pro_data[i]->name){
            timeWaiting=CurrentTime-pro_data[i]->arrival;
            duration=pro_data[i]->process_time;
            r=(timeWaiting+duration)/(duration*1.0);
            pro_data[i]->ratio=r;
            if(pro_data[i]->arrival <= CurrentTime && pro_data[i]->process_time > 0 && inPriorityQueue2(pq, pro_data[i]) == 0){
                pq.push(pro_data[i]);
                }
            i++;
        }
        curr_process=pq.top();
        pq.pop();
        CurrentTime=CurrentTime+curr_process->process_time;
        curr_process->finish_time=CurrentTime;
        curr_process->process_time=0;
        while(k<CurrentTime){
            excecutionArr[k]=curr_process->name;
            k++;
        }
        // pop elements to maintain ordered values
        while(!pq.empty())
            pq.pop();
}
}

void FB(element* pro_data[], int mode, int ftime, char* excecutionArr){
    int qCount=0;
    // get start time
    int CurrentTime = minProcessTime(pro_data);
    element* curr_process;
    int lastLevel = ftime;
    // initialize our array of queues
    std::queue<element*> q[ftime];
    int i,l,k=0;
    int qn=1;
    while(CurrentTime < ftime){
        l=0;
        while(l < lastLevel-1){
            i=0;
            // when a new process comes, it gets admitted to q[0]
            while(pro_data[i]->name){
                if(pro_data[i]->arrival <= CurrentTime && pro_data[i]->process_time > 0 && pro_data[i]->level == 0 && inQueue(q[0], pro_data[i]) == 0){
                    q[0].push(pro_data[i]);
                    l=0;
                    }
                i++;
            }
            // if there are no other processes in prev q
            // then that process shouldn't have moved
            if(l == 1 && q[l-1].empty() && !q[l].empty() && q[l].size() == 1 ){
                q[l-1].swap(q[l]);
                l--;
            }
            if(!q[l].empty()){
                curr_process=q[l].front();
                if(mode == 2){
                    qn=pow(2,l);
                    if(qn > curr_process->process_time)
                        qn=curr_process->process_time;
                    }
                CurrentTime=CurrentTime+qn;
                q[l].pop();
                curr_process->process_time=curr_process->process_time-qn;
                curr_process->finish_time=CurrentTime;
                curr_process->level++;
                while(k<CurrentTime){
                    excecutionArr[k]=curr_process->name;
                    k++;
                }
                // add to  next queue level
                if(curr_process->process_time > 0 && inQueue(q[l+1], curr_process) == 0)
                    q[l+1].push(curr_process);
            }
            else
                l++;
        }
    }
}

void aging(element* pro_data[], int qn, int ftime, char* excecutionArr){
    // get start time
    int CurrentTime = minProcessTime(pro_data);
    std::priority_queue<element*, vector<element *>, comparePriority> pq;
    int i,k=0;
    int qnOrig=qn;
    element* curr_process;
    while(CurrentTime < ftime){
        i=0;
        qn=qnOrig;
        while(pro_data[i]->name){
            if(pro_data[i]->arrival <= CurrentTime && inPriorityQueue3(pq, pro_data[i]) == 0)
                pq.push(pro_data[i]);
            i++;
        }
        curr_process=pq.top();
        pq.pop();
        CurrentTime=CurrentTime+qn;
        curr_process->finish_time=ftime;
        curr_process->priority=curr_process->origPriority;
        incrementPriority(pq);
        while(k<CurrentTime){
            excecutionArr[k]=curr_process->name;
            k++;
        }
        // pop elements to maintain ordered values
        std::priority_queue<element*, vector<element *>, comparePriority> tempQueue;
        while(!pq.empty()){
            tempQueue.push(pq.top());
            pq.pop();
        }
        while(!tempQueue.empty()){
    
        pq.push(tempQueue.top());
            tempQueue.pop();
        }
}
}

void trace(int number_of_processes,int MaxWedth,element *pro_data[],int scheduling_number,char *Quantum_number,int LenghOfGraph,char *exec){
int graph_height=number_of_processes+3;
    char x[graph_height][MaxWedth];
    for (int i=0;i<graph_height;i++)
        drawTable2(x[i],0,MaxWedth);
        drawTable2(x[1],1,MaxWedth);
        drawTable2(x[graph_height-1],1,MaxWedth);

    for(int i=2;i<graph_height-1;i++){
        for (int j=6;j<MaxWedth-1;j=j+2){
           x[i][j]='|';
        }
        x[i][0]=pro_data[i-2]->name;
    }
int mins=0;
if(scheduling_number==1){
        x[0][0]='F';x[0][1]='C';x[0][2]='F';x[0][3]='S';
    }
    else if(scheduling_number==2){
        x[0][0]='R';x[0][1]='R';x[0][2]='-';x[0][3]=Quantum_number[0];
    }
    else if(scheduling_number==3){
        x[0][0]='S';x[0][1]='P';x[0][2]='N';
    }
    else if(scheduling_number==4){
        x[0][0]='S';x[0][1]='R';x[0][2]='T';
    }
    else if(scheduling_number==5){
        x[0][0]='H';x[0][1]='R';x[0][2]='R';x[0][3]='N';
    }
    else if(scheduling_number==6){
        x[0][0]='F';x[0][1]='B';x[0][2]='-';x[0][3]='1';
    }
    else if(scheduling_number==7){
        x[0][0]='F';x[0][1]='B';x[0][2]='-';x[0][3]='2';x[0][4]='i';
    }
    else if(scheduling_number==8){
        x[0][0]='A';x[0][1]='g';x[0][2]='i';x[0][3]='n';x[0][4]='g';
    }

    for (int i=6;i<MaxWedth-1;i=i+2)
    {
        if(((i-6)/2)/10==0){
            x[0][i-mins]=((i-6)/2)+'0';
        }
        else
            x[0][i-mins]=(((i-6)/2))%10+'0';
    }
    int movingtime=0;

    for (int i=7+pro_data[0]->arrival*2;i<(pro_data[0]->finish_time-pro_data[0]->arrival)*2+(7+pro_data[0]->arrival*2);i=i+2){
        x[2][i]='.';
    }

    for(int j=1;j<number_of_processes;j++){
        for (int i=7+pro_data[j]->arrival*2;i<=(pro_data[j]->turnaround_time*2)+(7+pro_data[j]->arrival*2)-2;i=i+2){
             x[j+2][i]='.';
        }
    }
    for(int i=7;i<MaxWedth-1;i=i+2){
            for(int j=2;j<graph_height-1;j++)
            {
                if(exec[movingtime]==pro_data[j-2]->name)
                x[j][i]='*';
            }
           movingtime++;
    }

    for (int i=0;i<graph_height;i++){
        for(int j=0;j<LenghOfGraph*2+8;j++)
            printf("%c",x[i][j]);
        printf("\n");
    }
    printf("\n");

    drawTable2(x[graph_height],1,MaxWedth);
    }

void stats(int number_of_processes,element *pro_data[],int scheduling_number,char *Quantum_number)
{
    int max_len=11-5+(6*(number_of_processes+2));
    char x[6][max_len];
    for (int i=0;i<6;i++)
        drawTable(x[i],number_of_processes,max_len);
    char process[8]="Process";
    for(int i=0;i<7;i++){
        x[0][i]=process[i];
    }
    char Arrival[8]="Arrival";
    for(int i=0;i<7;i++)
        x[1][i]=Arrival[i];

    char Service[8]="Service";
    for(int i=0;i<7;i++)
        x[2][i]=Service[i];

    char Finish[7]="Finish";
    for(int i=0;i<6;i++)
        x[3][i]=Finish[i];

    char Turnaround[11]="Turnaround";
    for(int i=0;i<10;i++)
        x[4][i]=Turnaround[i];

     char NormTurn[9]="NormTurn";
    for(int i=0;i<8;i++)
        x[5][i]=NormTurn[i];

    for (int i=14;i<number_of_processes*6+14;i=i+6){
       x[0][i]=pro_data[(i-14)/6]->name;
    }

        for (int i=14;i<number_of_processes*6+14;i=i+6){
        if(pro_data[(i-14)/6]->arrival/10==0)
            x[1][i]=pro_data[(i-14)/6]->arrival+'0';
        else if(pro_data[(i-14)/6]->arrival/100==0){
            x[1][i-1]=(pro_data[(i-14)/6]->arrival/10)%10+'0';
            x[1][i]=pro_data[(i-14)/6]->arrival%10+'0';
        }else if(pro_data[(i-14)/6]->arrival/1000==0){
            x[1][i-2]=pro_data[(i-14)/6]->arrival%10+'0';
            x[1][i-1]=(pro_data[(i-14)/6]->arrival/10)%10+'0';
            x[1][i]=(pro_data[(i-14)/6]->arrival/100)%10+'0';
        }
    }
    for (int i=14;i<number_of_processes*6+14;i=i+6){
        if(pro_data[(i-14)/6]->process_time/10==0)
            x[2][i]=pro_data[(i-14)/6]->process_time+'0';
        else if(pro_data[(i-14)/6]->process_time/100==0){
            x[2][i-1]=(pro_data[(i-14)/6]->process_time/10)%10+'0';
            x[2][i]=pro_data[(i-14)/6]->process_time%10+'0';
        }else if(pro_data[(i-14)/6]->process_time/1000==0){
            x[2][i-2]=pro_data[(i-14)/6]->process_time%10+'0';
            x[2][i-1]=(pro_data[(i-14)/6]->process_time/10)%10+'0';
            x[2][i]=(pro_data[(i-14)/6]->process_time/100)%10+'0';
        }
    }

    for (int i=14;i<number_of_processes*6+14;i=i+6){
        if(pro_data[(i-14)/6]->finish_time/10==0)
            x[3][i]=pro_data[(i-14)/6]->finish_time+'0';
        else if(pro_data[(i-14)/6]->finish_time/100==0){
            x[3][i-1]=(pro_data[(i-14)/6]->finish_time/10)%10+'0';
            x[3][i]=pro_data[(i-14)/6]->finish_time%10+'0';
        }else if(pro_data[(i-14)/6]->finish_time/1000==0){
            x[3][i-2]=pro_data[(i-14)/6]->finish_time%10+'0';
            x[3][i-1]=(pro_data[(i-14)/6]->finish_time/10)%10+'0';
            x[3][i]=(pro_data[(i-14)/6]->finish_time/100)%10+'0';
        }
    }

    for (int i=14;i<number_of_processes*6+14;i=i+6){
        if(pro_data[(i-14)/6]->turnaround_time/10==0)
            x[4][i]=pro_data[(i-14)/6]->turnaround_time+'0';
        else if(pro_data[(i-14)/6]->turnaround_time/100==0){
            x[4][i-1]=(pro_data[(i-14)/6]->turnaround_time/10)%10+'0';
            x[4][i]=pro_data[(i-14)/6]->turnaround_time%10+'0';
        }else if(pro_data[(i-14)/6]->turnaround_time/1000==0){
            x[4][i-2]=pro_data[(i-14)/6]->turnaround_time%10+'0';
            x[4][i-1]=(pro_data[(i-14)/6]->turnaround_time/10)%10+'0';
            x[4][i]=(pro_data[(i-14)/6]->turnaround_time/100)%10+'0';
        }
    }

    double sum=0.0;
    for(int i=0;i<number_of_processes;i++){
        sum=sum+pro_data[i]->turnaround_time;
    }
    double Meann2=rounded(sum/number_of_processes);
    ostringstream s;
    s << Meann2;
    string strr=s.str();
    int fawzy=strr.length();
    while (fawzy>5 && strr[2]=='.'){
        strr.erase(strr.size() - 1);
        fawzy--;
    }
    while (fawzy>=5 && strr[1]=='.'){
        strr.erase(strr.size() - 1);
        fawzy--;
    }
    if (fawzy==4 && strr[2]=='.')
        strr.append("0");

    if (fawzy<=2)
        strr.append(".00");

    if (fawzy==3)
        strr.append("0");

    fawzy=strr.length();

    char mean[6]="Mean|";
    for(int i=((number_of_processes+1)*6)+6+1;i<((number_of_processes+1)*6)+6+6;i=i+1)
        x[2][i]=mean[i-(((number_of_processes+1)*6)+6+1)];

    fawzy=strr.length();

    int counter1=1;
    x[4][16+(number_of_processes*6)+1]='|';
    x[5][16+(number_of_processes*6)+1]='|';
    for(int i=16+(number_of_processes*6);i!=(16+(number_of_processes*6))-5;i--){
        x[4][i]=strr[fawzy-counter1];
        counter1++;
    }
    counter1=1;

    char dashes[7]="-----|";
    for(int i=(number_of_processes+1)*6+6;i<((number_of_processes+1)*6)+6+5+1;i=i+1)
        x[3][i]=dashes[i-((number_of_processes+1)*6+6)];

    double Meann3[number_of_processes];

    for (int i=0;i<number_of_processes;i++){
        Meann3[i]=pro_data[i]->Normalized_turn;
    }
    ostringstream st[number_of_processes];
    for (int i=0;i<number_of_processes;i++){
        st[i] << rounded(Meann3[i]);
    }

    string* pStr = new string[number_of_processes];

    for(int i=0;i<number_of_processes;i++){

    string strring=st[i].str();

    fawzy=strring.length();

    while (fawzy>5 && strring[2]=='.'){
        strring.erase(strring.size() - 1);
        fawzy--;
    }
    while (fawzy>=5 && strring[1]=='.'){
        strring.erase(strring.size() - 1);
        fawzy--;
    }
    if (fawzy==4 && strring[2]=='.')
        strring.append("0");

    if (fawzy<=2)
        strring.append(".00");

    if (fawzy==3)
        strring.append("0");

    pStr[i]=strring;

    }
        int var1=16;
        int var2=11;
        counter1=1;
    for(int i=0;i<number_of_processes;i++){
        fawzy=pStr[i].length();
        for(int j=var1;j>var2;j--){
            x[5][j]=pStr[i][fawzy-counter1];
            counter1++;
    }
        counter1=1;
        var1 = var1 +6;
        var2 = var2 +6;
    }
    sum=0.0;
    for(int i=0;i<number_of_processes;i++){
        sum=sum+pro_data[i]->Normalized_turn;
    }

    Meann2=rounded(sum/number_of_processes);
    ostringstream sd;
    sd << Meann2;

    string strringOfMean=sd.str();

    fawzy=strringOfMean.length();

    while (fawzy>5 && strringOfMean[2]=='.'){
        strringOfMean.erase(strringOfMean.size() - 1);
        fawzy--;
    }
    while (fawzy>=5 && strringOfMean[1]=='.'){
        strringOfMean.erase(strringOfMean.size() - 1);
        fawzy--;
    }
    if (fawzy==4 && strringOfMean[2]=='.')
        strringOfMean.append("0");

    if (fawzy<=2)
        strringOfMean.append(".00");

    if (fawzy==3)
        strringOfMean.append("0");

    counter1=1;
    fawzy=strringOfMean.length();
    for(int i=16+(number_of_processes*6);i!=(16+(number_of_processes*6))-5;i--){
        x[5][i]=strringOfMean[fawzy-counter1];
        counter1++;
    }

    if(scheduling_number==1){
        printf("FCFS\n");
    }
    else if(scheduling_number==2){
        printf("RR-");
        printf("%d\n",atoi(Quantum_number));
    }
    else if(scheduling_number==3){
        printf("SPN\n");
    }
    else if(scheduling_number==4){
        printf("SRT\n");
    }
    else if(scheduling_number==5){
        printf("HRRN\n");
    }
    else if(scheduling_number==6){
        printf("FB-1\n");
    }
    else if(scheduling_number==7){
        printf("FB-2i\n");
    }
    else if(scheduling_number==8){
        printf("aging\n");
    }

    for(int j=0;j<=1;j++){
        for(int i=0;i<=(6*number_of_processes+5)+6;i++)
            printf("%c",x[j][i]);
        printf("\n");
        }

        for (int i=0;i<max_len;i++){
            if (x[5][i]==NULL)
                x[5][i]=' ';

        }
        for (int i=0;i<max_len;i++){
            if (x[4][i]==NULL)
                x[4][i]=' ';
        }

    for(int j=2;j<=5;j++){
        for(int i=0;i<max_len;i++)
            printf("%c",x[j][i]);
        printf("\n");
        }
    printf("\n");
    }

int main()
{
    //First opening File and taking data
    char data[max_lines][Maximum];

    //needed variables
    int i;
    char mode[6];
    scanf("%s",&mode);
    char Pol[40];
    scanf("%s",&Pol);
    char nos5a[40];
    strcpy(nos5a,Pol);

    char *token;
    char *scheduling[30];
    char *quantum[30];
    token=strtok(nos5a,",");
    char temp300[30][30];
    i=0;
    while(token!=NULL){
        strcpy(temp300[i],token);
        token=strtok(NULL,",");
        i++;
    }
    int MaximumNumberOfGraphs=0;
   while(i!=0){
   scheduling[MaximumNumberOfGraphs]=strtok(temp300[MaximumNumberOfGraphs],"-");
   quantum[MaximumNumberOfGraphs]=strtok(NULL,"-");
   i--;
   MaximumNumberOfGraphs++;
   }
    char data2[6];
    scanf("%s",&data2);
    char str4[6];
    scanf("%s",&str4);
    char *modeString=strtok(mode," ");
    char *LenghOfGraph=strtok(data2, " ");

    char *number_of_processes=strtok(str4, " ");;

    //Initializing structure size
    element *pro_data[atoi(number_of_processes)+1];
    for (int i=0; i<atoi(number_of_processes)+1; i++){
        pro_data[i] = (element*)malloc(sizeof(element));
    }

    char process_ids;
    char *total_time=data2;
    int arrival_time[atoi(number_of_processes)];
    char process_id[5];
    char arrayofLines[atoi(number_of_processes)][20];
    string processdata;
    //Saving data needed for processes in array of Lines string
    for(i=4;i<atoi(number_of_processes)+4;i++){
        cin >> processdata;
        strcpy(arrayofLines[i-4], processdata.c_str());
    }

    //Taking data needed in the PCP
    char *str;char *temp;
    int processTimeArray[atoi(number_of_processes)];
    for (i=0;i<atoi(number_of_processes);i++){
        str = arrayofLines[i];
        temp=strtok(str, ",");
        pro_data[i]->name=temp[0];
        temp=strtok(NULL, ",");
        pro_data[i]->arrival=atoi(temp);
        temp=strtok(NULL, ",");
        processTimeArray[i]=atoi(temp);
        pro_data[i]->process_time=atoi(temp);
        pro_data[i]->finish_time=0;
	    pro_data[i]->turnaround_time=0;
	    pro_data[i]->Normalized_turn=0;
        pro_data[i]->level=0;
        pro_data[i]->ratio=0;
        pro_data[i]->priority=atoi(temp);
        pro_data[i]->origPriority=atoi(temp);
    }

    int MaxWedth=atoi(LenghOfGraph)*2+9;
    char exec[MaxWedth];
    if (atoi(scheduling[0])==1)
        FCFS(pro_data,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[0])==2)
        RR(pro_data,atoi(quantum[0]),atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[0])==3)
        SPN(pro_data,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[0])==4)
        SRT(pro_data,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[0])==5)
        HRRN(pro_data,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[0])==6)
        FB(pro_data,1,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[0])==7)
        FB(pro_data,2,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[0])==8)
        aging(pro_data,atoi(quantum[0]),atoi(LenghOfGraph),exec);

    //Restore Process Time for each structure of process and turnaround Time
    char *str2;
    for (i=0;i<atoi(number_of_processes);i++){
        str2 = arrayofLines[i];
        temp=strtok(str2, ",");
        temp=strtok(NULL, ",");
        temp=strtok(NULL, ",");
        pro_data[i]->process_time=processTimeArray[i];
        pro_data[i]->turnaround_time=pro_data[i]->finish_time-pro_data[i]->arrival;
        pro_data[i]->Normalized_turn=pro_data[i]->turnaround_time*(1.0)/pro_data[i]->process_time*(1.0);
    }

    //Make table to represent what happened
    if (modeString[0]=='s'){
    stats(atoi(number_of_processes),pro_data,atoi(scheduling[0]),quantum[0]);

    for (int x=1;x<MaximumNumberOfGraphs;x++){
        for (i=0;i<atoi(number_of_processes);i++){
        pro_data[i]->finish_time=0;
	    pro_data[i]->turnaround_time=0;
	    pro_data[i]->Normalized_turn=0;
        pro_data[i]->level=0;
    }

    if (atoi(scheduling[x])==1)
        FCFS(pro_data,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[x])==2)
        RR(pro_data,atoi(quantum[x]),atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[x])==3)
        SPN(pro_data,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[x])==4)
        SRT(pro_data,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[x])==5)
        HRRN(pro_data,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[x])==6)
        FB(pro_data,1,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[x])==7)
        FB(pro_data,2,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[x])==8)
        aging(pro_data,atoi(quantum[x]),atoi(LenghOfGraph),exec);

    //Restore Process Time for each structure of process and turnaround Time
    for (i=0;i<atoi(number_of_processes);i++){
        str2 = arrayofLines[i];
        temp=strtok(str2, ",");
        temp=strtok(NULL, ",");
        temp=strtok(NULL, ",");
        pro_data[i]->process_time=processTimeArray[i];
        pro_data[i]->turnaround_time=pro_data[i]->finish_time-pro_data[i]->arrival;
        pro_data[i]->Normalized_turn=pro_data[i]->turnaround_time*(1.0)/pro_data[i]->process_time*(1.0);
    }

    stats(atoi(number_of_processes),pro_data,atoi(scheduling[x]),quantum[x]);

    }
    } else if (modeString[0]=='t'){

trace(atoi(number_of_processes),MaxWedth,pro_data,atoi(scheduling[0]),quantum[0],atoi(LenghOfGraph),exec);

for (int x=1;x<MaximumNumberOfGraphs;x++){
    for (i=0;i<atoi(number_of_processes);i++){
        pro_data[i]->finish_time=0;
        pro_data[i]->turnaround_time=0;
        pro_data[i]->Normalized_turn=0;
        pro_data[i]->level=0;
    }

    if (atoi(scheduling[x])==1)
        FCFS(pro_data,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[x])==2)
        RR(pro_data,atoi(quantum[x]),atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[x])==3)
        SPN(pro_data,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[x])==4)
        SRT(pro_data,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[x])==5)
        HRRN(pro_data,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[x])==6)
        FB(pro_data,1,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[x])==7)
        FB(pro_data,2,atoi(LenghOfGraph),exec);
    else if (atoi(scheduling[x])==8)
        aging(pro_data,atoi(quantum[x]),atoi(LenghOfGraph),exec);

    //Restore Process Time for each structure of process and turnaround Time
    for (i=0;i<atoi(number_of_processes);i++){
        pro_data[i]->process_time=processTimeArray[i];
        pro_data[i]->turnaround_time=pro_data[i]->finish_time-pro_data[i]->arrival;
        pro_data[i]->Normalized_turn=pro_data[i]->turnaround_time*(1.0)/pro_data[i]->process_time*(1.0);
    }

trace(atoi(number_of_processes),MaxWedth,pro_data,atoi(scheduling[x]),quantum[x],atoi(LenghOfGraph),exec);

    }
    }
    return 0;
}
